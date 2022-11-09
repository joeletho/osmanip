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

# Library name
set(LIB_NAME "${PROJECT_NAME}")

# Binary names
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

# Compiler
set(MIN_GCC_VERSION "8")
set(BOOL IS_GCC_ABOVE_MIN_VERSION FALSE)
if (${CMAKE_CXX_COMPILER_VERSION} GREATER_EQUAL ${MIN_GCC_VERSION})
    set(BOOL IS_GCC_ABOVE_MIN_VERSION TRUE)
endif ()

#====================================================
#     Directories
#====================================================

# Binary directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/bin")

# Library / Archive directory
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY  "${CMAKE_CURRENT_LIST_DIR}/lib")

# Source directories
set(SRC_DIR "${CMAKE_CURRENT_LIST_DIR}/src")
set(INC_DIR "${CMAKE_CURRENT_LIST_DIR}/include")
set(EX_DIR "${CMAKE_CURRENT_LIST_DIR}/examples")
set(TEST_DIR "${CMAKE_CURRENT_LIST_DIR}/test/unit_tests")

#====================================================
#     Source files
#====================================================

file(GLOB_RECURSE SRC_FILES
        "${SRC_DIR}/*.cpp"
        )
file(GLOB_RECURSE INC_FILES
        "${INC_DIR}/*.hpp"
        )
file(GLOB_RECURSE TEST_SRC_FILES
        "${SRC_DIR}"
        "${INC_FILES}"
        "${TEST_DIR}/*.cpp"
        )

#====================================================
#     Binaries
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
#     Libraries
#====================================================

# Create the library
add_library(${LIB_NAME} SHARED
        "${INC_FILES}"
        "${SRC_FILES}"
        )

# Link with any additional dependencies
set(LIB_DEPS
        "pthread"
        "arsenalgear"
        )

if (UNIX AND NOT APPLE AND NOT ${IS_GCC_ABOVE_MIN_VERSION})
    list(APPEND LIB_DEPS
            "stdc++fs"
            )
endif ()

target_link_libraries("${LIB_NAME}"
        ${LIB_DEPS}
        )

# Link the library with the binaries
target_link_libraries(${EX_GRAPH_EXEC}
        ${LIB_NAME}
        )
target_link_libraries(${EX_PB_EXEC}
        ${LIB_NAME}
        )
target_link_libraries(${EX_REDIR_EXEC}
        ${LIB_NAME}
        )
target_link_libraries(${EX_MANIP_EXEC}
        ${LIB_NAME}
        )
target_link_libraries(${TEST_EXEC}
        ${LIB_NAME}
        )