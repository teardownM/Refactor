---@diagnostic disable: undefined-global
------------------------------
------ TeardownM Client ------
------------------------------
workspace "TDM"
    architecture "x64"
    -- startproject "Teardown"

    configurations { "Debug", "Release" }
    platforms { "x64" }

    flags "FatalWarnings"
    warnings "Extra"

include "Teardown"
include "Launcher"