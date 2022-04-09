---@diagnostic disable: undefined-global
project "Launcher"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "off"
    characterset "Unicode"
    callingconvention "Cdecl"

    targetdir ("../Build/%{cfg.buildcfg}/%{prj.name}")
    objdir ("../Build/Intermediate/%{cfg.buildcfg}/%{prj.name}")
    debugdir("../Build/%{cfg.buildcfg}/%{prj.name}")

    files {
        "Include/**.h",
        "../Shared/**.h",
        "../Shared/**.cpp",
        "Source/**.cpp",
    }

    includedirs {
        "Include",
        "../Shared/Include",
        "../Vendor/spdlog/include"
    }

    libdirs {
        "../Shared/Lib"
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
		"nakama-cpp%{cfg.buildcfg}"
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