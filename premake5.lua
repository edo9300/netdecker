workspace "ygopen-netdecker"
    location "build"
    language "C++"
    cppdialect "C++14"
    objdir "obj"
    staticruntime "on"
    startproject "ygopen-netdecker"

    configurations { "Debug", "Release" }

    filter "system:windows"
        defines { "WIN32", "_WIN32", "UNICODE" }

    filter "action:vs*"
        vectorextensions "SSE2"
        defines "_CRT_SECURE_NO_WARNINGS"

    filter { "action:not vs*", "system:windows" }
        buildoptions { "-static-libgcc" }

    filter "action:not vs*"
        buildoptions { "-fno-strict-aliasing", "-Wno-multichar" }

    filter "configurations:Debug"
        symbols "On"
        defines "_DEBUG"
        targetdir "bin/debug"
        runtime "Debug"

    filter "configurations:Release"
        optimize "Size"
        targetdir "bin/release"

    filter { "configurations:Release*" , "action:not vs*" }
        symbols "On"
        defines "NDEBUG"
        buildoptions "-march=native"

include "lib/lzma/."
project "ygopen-netdecker"
    kind "ConsoleApp"
    includedirs "lib"
    files { "src/*.cpp", "src/*.h" }
    links "clzma"
