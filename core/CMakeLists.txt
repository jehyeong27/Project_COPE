cmake_minimum_required(VERSION 3.20)

project(cope_core
    VERSION 0.1.0
    DESCRIPTION "COPE C++ core library"
    LANGUAGES CXX
)

# ---------------------------------------------------------------------------
# Options
# ---------------------------------------------------------------------------
option(COPE_BUILD_SHARED "Build core as a shared library (for C# P/Invoke)" ON)
option(COPE_BUILD_TESTS  "Build core unit tests" OFF)

# ---------------------------------------------------------------------------
# Global settings
# ---------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# Put all binaries in one place so the C# GUI can find the DLL easily
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# ---------------------------------------------------------------------------
# Library target
# ---------------------------------------------------------------------------
file(GLOB_RECURSE COPE_CORE_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

if(COPE_BUILD_SHARED)
    add_library(cope_core SHARED ${COPE_CORE_SOURCES})
    target_compile_definitions(cope_core PRIVATE COPE_CORE_EXPORTS)
else()
    add_library(cope_core STATIC ${COPE_CORE_SOURCES})
    target_compile_definitions(cope_core PUBLIC COPE_CORE_STATIC)
endif()

add_library(cope::core ALIAS cope_core)

target_include_directories(cope_core
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

if(MSVC)
    target_compile_options(cope_core PRIVATE /W4 /permissive- /utf-8)
else()
    target_compile_options(cope_core PRIVATE -Wall -Wextra -Wpedantic)
endif()

# ---------------------------------------------------------------------------
# Tests
# ---------------------------------------------------------------------------
if(COPE_BUILD_TESTS AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/tests/CMakeLists.txt)
    enable_testing()
    add_subdirectory(tests)
endif()

# ---------------------------------------------------------------------------
# Install
# ---------------------------------------------------------------------------
install(TARGETS cope_core
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
install(DIRECTORY include/ DESTINATION include)
