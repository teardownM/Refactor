---@diagnostic disable: undefined-global
project "lua-5.1.4"
    kind "StaticLib"
    language "C"
    cdialect "Default"
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
        "*.h",
        "*.c",
    }

    -- includedirs {
        -- "Include",
    -- }

    -- libdirs {
        -- "../Shared/Lib"
    -- }


	-- links {
		-- "detours"
	-- }

    -- postbuildcommands {
        -- "copy \"$(SolutionDir)Build\\$(Configuration)\\Teardown\\Teardown.dll\" \"$(SolutionDir)Build\\$(Configuration)\\Launcher\\Teardown.dll\""
    -- }

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