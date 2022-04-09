---@diagnostic disable: undefined-global
project "Teardown"
    kind "SharedLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "off"
    characterset "Unicode"
    callingconvention "Cdecl"

    defines {
        "_WINDOWS",
        "_USRDLL",
        "_WINDLL",
        "_UNICODE",
        "UNICODE",
        "TDM_EXPORTS",
    }

    targetdir ("../Build/%{cfg.buildcfg}/%{prj.name}")
    objdir ("../Build/Intermediate/%{cfg.buildcfg}/%{prj.name}")
    debugdir("../Build/%{cfg.buildcfg}/Launcher")
    debugcommand("../Build/%{cfg.buildcfg}/Launcher/Launcher.exe")

    files {
        "Include/**.h",
        "../Shared/**.h",
        "../Shared/**.cpp",
        "Source/**.cpp",
    }

    includedirs {
        "Include",
        "../Shared/Include",
        "../Vendor/spdlog/include",
		"../lua-5.1.4",
		"../glm"
    }

    libdirs {
        "../Shared/Lib",
		"../Build/%{cfg.buildcfg}/lua-5.1.4"
    }


	links {
		"detours",
		"wsock32",
		"Crypt32",
		"bcrypt",
		"winhttp",
		"crypto%{cfg.buildcfg}",
		"ssl%{cfg.buildcfg}",
		"libprotobuf%{cfg.buildcfg}",
		"cpprest%{cfg.buildcfg}",
		"nakama-cpp%{cfg.buildcfg}",
		"lua-5.1.4"
	}

    postbuildcommands {
        "copy \"$(SolutionDir)Build\\$(Configuration)\\Teardown\\Teardown.dll\" \"$(SolutionDir)Build\\$(Configuration)\\Launcher\\Teardown.dll\""
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        defines "DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "NDEBUG"
        runtime "Release"

    filter {}