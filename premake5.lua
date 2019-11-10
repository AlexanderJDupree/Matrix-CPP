-- File: premake5.lua

-- Brief: Build script for the premake build system. Run 'premake5 gmake' to 
--        build GNU specific makefiles. 

-- Author: Alexander DuPree

-- WORKSPACE CONFIGURATION --
workspace "Matrix-CPP"
    configurations { "debug", "release" }

    if _ACTION == "clean" then
        os.rmdir("bin/")
        os.rmdir("lib/")
        os.rmdir("gmake/")
        os.rmdir("gmake2/")
    end

    local project_action = "UNDEFINED"
    if _ACTION ~= nill then project_action = _ACTION end

    location (project_action)

    -- PLATFORM CONFIGURATIONS --

    -- COMPILER/LINKER CONFIG --
    flags "FatalWarnings"
    warnings "Extra"

    filter "configurations:debug*"   
        buildoptions { "-fprofile-arcs", "-ftest-coverage" }
        defines { "DEBUG" }
        links "gcov"
        symbols "On"

    filter "configurations:release*" 
        defines { "NDEBUG" } 
        optimize "On"

    filter "toolset:gcc"
        buildoptions { 
            "-Wall", "-Wextra", "-Werror", "-std=c++17"
        }

    filter {} -- close filter

project "Matrix-CPP"
    kind "StaticLib"
    language "C++"
    targetdir "lib/%{cfg.buildcfg}/"
    targetname "Matrix-CPP"

    local include = "include/"

    includedirs (include)

project "Tests"
    kind "ConsoleApp"
    language "C++"
    links "Matrix-CPP"
    targetdir "bin/tests/"
    targetname "%{cfg.buildcfg}_tests"

    local include  = "include/"
    local test_src = "tests/"
    local test_inc = "third_party/"

    files (test_src .. "**.cpp")

    includedirs { test_inc, include }

    --postbuildcommands ".././bin/tests/%{cfg.buildcfg}_tests"

    filter {} -- close filter

