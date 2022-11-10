#[[ Refer to README.md in build/ for use instructions ]]

#====================================================
#     CMake settings
#====================================================

cmake_minimum_required(VERSION 3.16)

#====================================================
#     Compiler / Project settings
#====================================================

set(CMAKE_CXX_COMPILER_ID "GNU")
set(CMAKE_CXX_STANDARD 17)
project(osmanip)

#====================================================
#     Variables
#====================================================

# Compiler
set(MIN_GCC_VERSION "8")

#====================================================
#     Options
#====================================================

option(OSM_EXAMPLES "Build examples that are not required for core library" OFF)

#====================================================
#     Directories
#====================================================

# Binary directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/bin")

# Library / Archive directory
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")

# Source directories
set(SRC_DIR "${CMAKE_CURRENT_LIST_DIR}/src")
set(INC_DIR "${CMAKE_CURRENT_LIST_DIR}/include")
set(EX_DIR "${CMAKE_CURRENT_LIST_DIR}/examples")
set(TEST_DIR "${CMAKE_CURRENT_LIST_DIR}/test/unit_tests")

# Create required directories
add_custom_target(create-directories ALL
        COMMAND ${CMAKE_COMMAND} -E make_directory
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
        )

#====================================================
#     Source files
#====================================================

file(GLOB_RECURSE INC_FILES
        "${INC_DIR}/*.hpp"
        )
file(GLOB_RECURSE SRC_FILES
        "${SRC_DIR}/*.cpp"
        )

#====================================================
#     Static library creation
#====================================================

add_library(${PROJECT_NAME}
        STATIC
        "${INC_FILES}"
        "${SRC_FILES}"
        )

set_target_properties(${PROJECT_NAME}
        PROPERTIES
        LINKER_LANGUAGE CXX
        )

#====================================================
#     Dependencies and flags
#====================================================

set(LIB_DEPS
        "pthread"
        "arsenalgear"
        )

if (UNIX AND NOT APPLE)
    if (${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS ${MIN_GCC_VERSION})
        list(APPEND LIB_DEPS
                "stdc++fs"
                )
    endif ()
endif ()

target_link_libraries(${PROJECT_NAME}
        ${LIB_DEPS}
        )

#====================================================
#     Include
#====================================================

target_include_directories(${PROJECT_NAME} PUBLIC
        ${INC_DIR}
        ${SRC_DIR}
        )

#====================================================
#     Examples
#====================================================

if (${OSM_EXAMPLES})

    #====================================================
    #     Variables
    #====================================================

    if (UNIX)
        set(EX_MANIP_EXEC "manipulators")
        set(EX_PB_EXEC "progressbar")
        set(EX_GRAPH_EXEC "graphics")
        set(EX_REDIR_EXEC "redirection")
        set(TEST_EXEC "tests")
    else ()
        set(EX_MANIP_EXEC "manipulators.exe")
        set(EX_PB_EXEC "progressbar.exe")
        set(EX_GRAPH_EXEC "graphics.exe")
        set(EX_REDIR_EXEC "redirection.exe")
        set(TEST_EXEC "tests.exe")
    endif ()

    #====================================================
    #     Source files
    #====================================================

    file(GLOB_RECURSE TEST_SRC_FILES
            "${SRC_DIR}"
            "${INC_FILES}"
            "${TEST_DIR}/*.cpp"
            )

    #====================================================
    #     Create binaries
    #====================================================

    add_executable(${EX_GRAPH_EXEC}
            "${EX_DIR}/graphics.cpp"
            )
    add_executable(${EX_PB_EXEC}
            "${EX_DIR}/progressbar.cpp"
            )
    add_executable(${EX_REDIR_EXEC}
            "${EX_DIR}/redirection.cpp"
            )
    add_executable(${EX_MANIP_EXEC}
            "${EX_DIR}/manipulators.cpp"
            )
    add_executable(${TEST_EXEC}
            "${TEST_SRC_FILES}"
            )

    #====================================================
    #     Dependencies
    #====================================================
    target_link_libraries(${EX_GRAPH_EXEC}
            "${PROJECT_NAME}"
            )
    target_link_libraries(${EX_PB_EXEC}
            "${PROJECT_NAME}"
            )
    target_link_libraries(${EX_REDIR_EXEC}
            "${PROJECT_NAME}"
            )
    target_link_libraries(${EX_MANIP_EXEC}
            "${PROJECT_NAME}"
            )
    target_link_libraries(${TEST_EXEC}
            "${PROJECT_NAME}"
            )
endif ()
