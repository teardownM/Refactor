---@diagnostic disable: undefined-global
require('vstudio')

project "Launcher"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"
    characterset "Unicode"
    callingconvention "Cdecl"

    targetdir ("Build/%{cfg.platform}/%{cfg.buildcfg}/%{prj.name}")
    objdir ("Build/Intermediate/%{prj.name}")
    debugdir("Build/x64/Debug/Launcher")

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
        "../Shared/Lib/%{cfg.buildcfg}"
    }

    links {
        "detours"
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