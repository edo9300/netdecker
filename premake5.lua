workspace "net"
location "build"
language "C++"
cppdialect "C++14"
objdir "obj"
staticruntime "on"
startproject "netdecker"

configurations { "Debug", "Release" }

filter "system:windows"
defines { "WIN32", "_WIN32" }

filter "system:bsd"
includedirs "/usr/local/include"
libdirs "/usr/local/lib"

filter "system:macosx"
toolset "clang"
buildoptions { "-fms-extensions" }
includedirs { "/usr/local/include" }
libdirs { "/usr/local/lib" }

filter "action:vs*"
vectorextensions "SSE2"
defines "_CRT_SECURE_NO_WARNINGS"

filter "action:not vs*"
buildoptions { "-fno-strict-aliasing", "-Wno-multichar" }

filter { "action:not vs*", "system:windows" }
buildoptions { "-static-libgcc" }

filter "configurations:Debug"
symbols "On"
defines "_DEBUG"
targetdir "bin/debug"
runtime "Debug"

filter { "configurations:Release*" , "action:not vs*" }
symbols "On"
defines "NDEBUG"
buildoptions "-march=native"

filter "configurations:Release"
optimize "Size"
targetdir "bin/release"
project "netdecker"
kind "ConsoleApp"
links { "clzma" }
includedirs "lib"
files { "src/*.cpp", "src/*.h" }

include "lib/lzma/."
