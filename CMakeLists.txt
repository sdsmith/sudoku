# TODO(stewarts): preprocessor defines:
# - BUILD={debug,release}
# - TRAP_FP_FAULTS ?= false
# - ARCH={x86,x64}

cmake_minimum_required(VERSION 3.10.2)
project(sudoku)

set(source_dir src)
set(test_dir test)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ..)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Source files
#
file(GLOB_RECURSE source_files "${source_dir}/*.cpp")

# Executable
#
add_executable(sudoku ${source_files})

# Include paths
#
target_include_directories(sudoku PUBLIC src)

# Compiler config
#
# TODO(Sdsmith): gtest doesn't play well with -Wundef, -Wswitch-enum
set(GENERIC_CXX_FLAGS "-std=c++17 -fPIC -Werror -Wall -Wextra -Weffc++ -Wfloat-equal -Werror-implicit-function-declaration  -Wpointer-arith -Wcast-align -Wstrict-overflow=4 -Wwrite-strings -Wno-unused-result")
# Remove noisy warnings
set(GENERIC_CXX_FLAGS "${GENERIC_CXX_FLAGS} -Wcast-qual -Wswitch-default -Wno-switch-enum -Wconversion -Wunreachable-code")

set(CLANG_CXX_FLAGS "${GENERIC_CXX_FLAGS}")

set(GCC_CXX_FLAGS "${GENERIC_CXX_FLAGS}")

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")  
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CLANG_CXX_FLAGS}")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_CXX_FLAGS}")
endif()

## Googletest
#

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
  ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
  EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
  include_directories("${gtest_SOURCE_DIR}/include")
endif()

file(GLOB_RECURSE test_files "${test_dir}/*.cpp")
add_executable(sudoku_test ${test_files})
target_link_libraries(sudoku_test gtest_main)
add_test(NAME sudoku_test COMMAND sudoku_test)
