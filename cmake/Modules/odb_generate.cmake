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

include(CMakeParseArguments)

find_program(ODB_EXECUTABLE NAMES odb)
mark_as_advanced(ODB_EXECUTABLE)

#
# Generate pre-target ODB schema
#
function(odb_generate target_name)
    if(NOT ODB_EXECUTABLE)
        message(FATAL_ERROR "ODB compiler not found!")
    endif()

    set(options
        ODB_GENERATE_QUERY
        ODB_GENERATE_SESSION
        ODB_GENERATE_SCHEMA
        )
    set(oneValueParams
        ODB_DB
        ODB_PROFILE
        ODB_SCHEMA_FORMAT
        ODB_OUTPUT_DIRECTORY
        ODB_HEADER_SUFFIX
        ODB_SOURCE_SUFFIX
        ODB_DEFAULT_POINTER
        )
    set(multiValueParams
        ODB_HEADER_FILES
        ODB_PROLOGUE_FILES
        ODB_EPILOGUE_FILES
        ODB_INCLUDE_DIRS
        )
    cmake_parse_arguments(PARAM "${options}" "${oneValueParams}" "${multiValueParams}" ${ARGN})

    if(PARAM_UNPARSED_ARGUMENTS)
        message(FATAL_ERROR "Unknown keywords given to odb_generate(): \"${PARAM_UNPARSED_ARGUMENTS}\"")
    endif()

    if(NOT PARAM_ODB_HEADER_FILES)
        message(FATAL_ERROR "HEADERS parameter missing")
    endif()

    set(odb_output_directory "${PROJECT_BINARY_DIR}/odb")
    set(odb_header_suffix ".h")
    set(odb_source_suffix ".cxx")
    set(odb_cxx_standard c++11)
    set(odb_profile qt)
    set(odb_database pgsql)

    set(ODB_ARGS)

    if(PARAM_ODB_GENERATE_QUERY)
        list(APPEND ODB_ARGS --generate-query)
    endif()

    if(PARAM_ODB_GENERATE_SESSION)
        list(APPEND ODB_ARGS --generate-session)
    endif()

    if(PARAM_ODB_GENERATE_SCHEMA)
        list(APPEND ODB_ARGS --generate-schema)
    endif()

    if(PARAM_ODB_DB)
        set(odb_database ${PARAM_ODB_DB})
    endif()

    if(PARAM_ODB_PROFILE)
        set(odb_profile ${PARAM_ODB_PROFILE})
    endif()

    if(PARAM_ODB_SCHEMA_FORMAT)
        list(APPEND ODB_ARGS --schema-format ${PARAM_ODB_SCHEMA_FORMAT})
    endif()

    if(PARAM_ODB_OUTPUT_DIRECTORY)
        set(odb_output_directory ${PARAM_ODB_OUTPUT_DIRECTORY})
    endif()

    if(PARAM_ODB_HEADER_SUFFIX)
        set(odb_header_suffix ${PARAM_ODB_HEADER_SUFFIX})
    endif()

    if(PARAM_ODB_SOURCE_SUFFIX)
        set(odb_source_suffix ${PARAM_ODB_SOURCE_SUFFIX})
    endif()

    if(PARAM_ODB_DEFAULT_POINTER)
        list(APPEND ODB_ARGS --default-pointer ${PARAM_ODB_DEFAULT_POINTER})
    endif()

    if(PARAM_ODB_PROLOGUE_FILES)
        foreach(file ${PARAM_ODB_PROLOGUE_FILES})
            list(APPEND ODB_ARGS --hxx-prologue-file ${file})
        endforeach()
    endif()

    foreach(dir ${PARAM_ODB_INCLUDE_DIRS})
        list(APPEND ODB_ARGS -I${dir})
    endforeach()

    list(APPEND ODB_ARGS --output-dir ${odb_output_directory})
    list(APPEND ODB_ARGS --hxx-suffix ${odb_header_suffix})
    list(APPEND ODB_ARGS --cxx-suffix ${odb_source_suffix})
    list(APPEND ODB_ARGS --std ${odb_cxx_standard})
    list(APPEND ODB_ARGS --profile ${odb_profile})
    list(APPEND ODB_ARGS --database ${odb_database})
    list(APPEND ODB_ARGS -x -fPIC) # for Qt

    file(REMOVE_RECURSE ${odb_output_directory})
    file(MAKE_DIRECTORY ${odb_output_directory})

    set(odb_hxx_files)
    set(odb_cxx_files)


    list(LENGTH PARAM_ODB_HEADER_FILES headers_count)
    math(EXPR headers_count "${headers_count} - 1")

    foreach(indx RANGE ${headers_count})
        list(GET PARAM_ODB_HEADER_FILES ${indx} header)
        get_filename_component(header_name "${header}" NAME_WE)
        set(cxxFile "${odb_output_directory}/${header_name}-odb${odb_source_suffix}")
        set(hxxFile "${odb_output_directory}/${header_name}-odb${odb_header_suffix}")

        list(APPEND odb_hxx_files ${hxxFile})
        list(APPEND odb_cxx_files ${cxxFile})

        set(args ${ODB_ARGS})

        # Add corresponding epilogue file
        if(PARAM_ODB_EPILOGUE_FILES)
            list(GET PARAM_ODB_EPILOGUE_FILES ${indx} epilogueFile)
            list(APPEND args --odb-epilogue-file ${epilogueFile})
        endif()

        add_custom_command(
            OUTPUT ${cxxFile} ${hxxFile}
            COMMAND ${ODB_EXECUTABLE} ${args} ${header}
            DEPENDS ${header}
            COMMENT "Generate odb schema for ${header}"
            )
    endforeach()

    target_sources(${target_name}
        PRIVATE
        ${odb_hxx_files}
        ${odb_cxx_files}
        )

    target_link_libraries(${target_name}
        PRIVATE
        odb
        odb-${odb_profile}
        odb-${odb_database}
        )
endfunction(odb_generate)
