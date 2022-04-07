---@diagnostic disable: undefined-global
project "Teardown"
    kind "SharedLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"
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
        "../Vendor/spdlog/include"
    }

    libdirs {
        "../Shared/Lib"
    }

    links {
        "detours"
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