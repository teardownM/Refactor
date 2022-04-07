---@diagnostic disable: undefined-global
project "Launcher"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"
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