# MIT License

# Copyright (c) 2022 Iman Ahmadvand

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

cmake_minimum_required(VERSION 3.13)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules")

project(OdbArena
    VERSION 1.0.0
    LANGUAGES C CXX
    )

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME} src/main.cpp)

# Source
file(GLOB_RECURSE hxxFiles ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)
file(GLOB_RECURSE cxxFiles ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
target_sources(${PROJECT_NAME} PRIVATE ${hxxFiles} ${cxxFiles})
target_include_directories(${PROJECT_NAME} PUBLIC src ${CMAKE_CURRENT_BINARY_DIR})

# ODB
include(odb_generate)
set(odbHeaderFiles
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Data.h
    )
file(GLOB_RECURSE odbPrologueFiles ${CMAKE_CURRENT_SOURCE_DIR}/src/Traits/Traits.h)
set(odbEpilogueFiles
    ${CMAKE_CURRENT_SOURCE_DIR}/src/DataPragma.h
    )
get_target_property(odbIncludeDirs ${PROJECT_NAME} INCLUDE_DIRECTORIES)
odb_generate(
    ${PROJECT_NAME}
    ODB_HEADER_FILES "${odbHeaderFiles}"
    ODB_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/odb
    ODB_PROLOGUE_FILES "${odbPrologueFiles}"
    ODB_INCLUDE_DIRS "${odbIncludeDirs}"
    ODB_EPILOGUE_FILES "${odbEpilogueFiles}"
    ODB_GENERATE_QUERY
    ODB_GENERATE_SESSION
    ODB_GENERATE_SCHEMA
    ODB_SCHEMA_FORMAT sql
    ODB_PROFILE boost
    ODB_DB pgsql
    )
