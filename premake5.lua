---@diagnostic disable: undefined-global
------------------------------
------ TeardownM Client ------
------------------------------
local username = os.getenv('USERNAME');
local premakeDir = "C:\\Users\\" .. username .. "\\.premake\\"

if os.isdir(premakeDir .. "cmake") then
    require "cmake"
end

if os.isdir(premakeDir .. "clion") then
    require "clion"
end

workspace "TDM"
    architecture "x64"
    toolset "msc"
    characterset "Unicode"

    configurations { "Debug", "Release" }
    platforms { "x64" }

    --flags "FatalWarnings"
    --warnings "Extra"

include "lua-5.1.4"
include "glm"
include "Teardown"
include "Launcher"