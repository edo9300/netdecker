cmake_minimum_required(VERSION 3.12.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Debug")
endif()
option(NETDECKER_MSVC_STATIC "Static CRT on Visual Studio" ON)

project(ygopen-netdecker)
add_executable(ygopen-netdecker)
set_property (DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT "ygopen-netdecker")

add_subdirectory(lib)
add_subdirectory(src)

if(WIN32)
	add_compile_definitions(UNICODE)
endif()
target_include_directories(ygopen-netdecker PRIVATE lib)
target_link_libraries(ygopen-netdecker PRIVATE clzma)

set(CLANG_OR_GNU $<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>)
target_compile_options(ygopen-netdecker PRIVATE
	# Clang and GNU compilers
	$<${CLANG_OR_GNU}:-Wall -Wextra -pedantic>
	$<$<AND:$<CONFIG:Debug>,${CLANG_OR_GNU}>:-ggdb3 -Og -D_DEBUG>
	$<$<AND:$<CONFIG:Release>,${CLANG_OR_GNU}>:-O3>
	$<$<AND:$<CONFIG:RelWithDebInfo>,${CLANG_OR_GNU}>:-ggdb3 -O3 -D_DEBUG>
	# Microsoft compiler
	$<$<CXX_COMPILER_ID:MSVC>:/W4>
)

if(NETDECKER_MSVC_STATIC AND MSVC)
	set(CompilerFlags
        CMAKE_CXX_FLAGS
        CMAKE_CXX_FLAGS_DEBUG
        CMAKE_CXX_FLAGS_RELEASE
        CMAKE_C_FLAGS
        CMAKE_C_FLAGS_DEBUG
        CMAKE_C_FLAGS_RELEASE
        )
	foreach(CompilerFlag ${CompilerFlags})
  		string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
	endforeach()
	# CMake 3.15
	# set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()
