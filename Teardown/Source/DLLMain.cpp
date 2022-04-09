#include <filesystem>

#include "Globals.h"
#include "Console.h"

#include "Launcher.h"
#include "Teardown/Functions.h"
#include "Teardown/Menu.h"

#include "nakama-cpp/Nakama.h"

using namespace Nakama;

[[maybe_unused]] __declspec(dllexport) int __stdcall _() { return 0; }

///////////////////////////////
//////     Globals     ////////
///////////////////////////////
NClientPtr client;
NRtClientPtr rtClient;
NRtDefaultClientListener listener;
unsigned long long currentUserID = 0;

///////////////////////////////////
///////     Functions     /////////
///////////////////////////////////
unsigned long long GetSteamUserId() {
    HKEY UserKey;
    char SteamUserIdData[8];

    if (RegOpenKeyExA(HKEY_CURRENT_USER, R"(Software\Valve\Steam\ActiveProcess)", 0, KEY_QUERY_VALUE, &UserKey) == ERROR_SUCCESS) {
        DWORD dwLen = MAX_PATH;
        if (RegQueryValueExA(UserKey, "ActiveUser", nullptr, nullptr, reinterpret_cast<LPBYTE>(&SteamUserIdData), &dwLen) == ERROR_SUCCESS) {
            // Success, do nothing for now.
        }
        else {
            return 0;
        }

        RegCloseKey(UserKey);
        unsigned long long SteamUserId;
        memcpy(&SteamUserId, SteamUserIdData, sizeof SteamUserId);
        return SteamUserId;
    }
    else {
        return 0;
    }
}

// Mutli-Threaded Tick Implementation. (Doesn't currently work.)
/*
void Tick() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        client->tick();
        if (rtClient) {
            rtClient->tick();
        }
    }
}
*/

void Shutdown(HMODULE hModule, DWORD reason) {
    Teardown::Menu::Revert();
    LOG_INFO("Goodbye :)\nExit code: {}", reason);

#ifdef DEBUG
    ::Utilities::Console::Close();
#endif
    FreeLibraryAndExitThread(hModule, reason);
}

DWORD WINAPI StartRoutine([[maybe_unused]] HMODULE hModule) {
    g_Module = GetModuleHandle(nullptr);

#ifdef _DEBUG
    ::Utilities::Console::Initialize();
#endif

    if (!Launcher::GetPath()) {
        Shutdown(reinterpret_cast<HMODULE>(hModule), 1);
    }

    Logger::Initialize();

    Teardown::Path = std::filesystem::current_path().string();
    if (Teardown::Menu::Set() != 0) {
        LOG_ERROR("Failed to create menu");
        Shutdown(reinterpret_cast<HMODULE>(g_Module), 1);
        return 1;
    }

    Teardown::GetFunctionAddresses();

    LOG_INFO("We are in bois ;)");
	
    currentUserID = GetSteamUserId();
    if (currentUserID == 0) {
        LOG_ERROR("Failed to authenticate steam user.");
        Shutdown(reinterpret_cast<HMODULE>(g_Module), 1);
        return 1;
    }

    // Create a test connection (Debugging/Testing)
    LOG_DEBUG("Creating a test client connection..");
    NClientParameters parameters;
    parameters.serverKey = "defaultkey";
    parameters.host = "127.0.0.1"; // Server IP (127.0.0.1 = local, 159.89.18.92 = dev server)
    parameters.port = DEFAULT_PORT;
    client = createDefaultClient(parameters);

    // Start Thread (Tick)
    //std::thread tickThread(Tick); // Doesn't currently work. Runs at the end of this thread instead.

    // Wait for tickThread to finish (Debug/Testing)
    //tickThread.join();

    auto errorCallback = [](const NError& error)
    {
        // convert error to readable string
        LOG_ERROR(toString(error));

        // check error code
        if (error.code == ErrorCode::ConnectionError)
        {
            LOG_ERROR("The server is currently unavailable. Check internet connection.");
        }
    };

    auto loginFailedCallback = [](const NError& error)
    {
    };

    auto loginSucceededCallback = [](NSessionPtr session)
    {
        LOG_DEBUG("UserId: " + session->getUserId());
        LOG_DEBUG("Username: " + session->getUsername());
        if (session->isExpired()) {
            LOG_ERROR("Session Expired.");
            Shutdown(reinterpret_cast<HMODULE>(g_Module), 1);
            return 1;
        }
        Nakama::NTimestamp ts = session->getExpireTime();
        LOG_DEBUG("Session Expires At: " + std::to_string(ts));

        bool createStatus = true;
        rtClient = client->createRtClient(DEFAULT_PORT);
        // define listener in your class as NRtDefaultClientListener listener;
        listener.setConnectCallback([]()
            {
                LOG_DEBUG("Socket connected");
            });
        rtClient->setListener(&listener);
        rtClient->connect(session, createStatus);

        // Joining Test Lobby
        listener.setChannelMessageCallback([](const NChannelMessage& message)
            {
                LOG_DEBUG("Received a message on channel " + message.channelId + " : " + message.content);
            });

        std::string roomName = std::to_string(currentUserID) + "-room";

        auto successJoinCallback = [](NChannelPtr channel)
        {
            LOG_DEBUG("Joined chat: " + channel->id);

            // content must be JSON
            std::string content = "{\"message\":\"Hello world\"}";

            rtClient->writeChatMessage(channel->id, content);
        };
        
        /*RtErrorCallback rtErrorCallback = []()
        {
            LOG_ERROR("Error occured while joining/creating room.");
            return RtErrorCallback();
        };*/

        rtClient->joinChat(
            roomName,
            NChannelType::ROOM,
            {},
            {},
            successJoinCallback);

        listener.setStatusPresenceCallback([](const NStatusPresenceEvent& event)
            {
                for (auto& presence : event.joins)
                {
                    LOG_DEBUG("User Joined | ID: " + presence.userId + " | Username: " + presence.username + " | Status: " + presence.status);
                }

                for (auto& presence : event.leaves)
                {
                    LOG_DEBUG("User Left | ID: " + presence.userId + " | Username: " + presence.username + " | Status: " + presence.status);
                }
            });
    };

    std::string deviceId = std::to_string(currentUserID);
    LOG_DEBUG("Authenticating as steam user " + std::to_string(currentUserID));

    client->authenticateDevice(
        deviceId,
        opt::nullopt,
        opt::nullopt,
        {},
        loginSucceededCallback,
        loginFailedCallback);

    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        client->tick();
        if (rtClient) {
            rtClient->tick();
        }
    }

    return 0;
}

///////////////////////////////////
///////     Entry Point     ///////
///////////////////////////////////
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ulReason, [[maybe_unused]] LPVOID lpReserved) {
    switch (ulReason) {
        case DLL_PROCESS_ATTACH:
            DisableThreadLibraryCalls(hModule);
            CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(StartRoutine), hModule, 0, nullptr);
            break;
        case DLL_PROCESS_DETACH: {
            Shutdown(reinterpret_cast<HMODULE>(g_Module), 0);
            break;
        } default: break;
    }

    return TRUE;
}