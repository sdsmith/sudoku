# TODO(stewarts): preprocessor defines:
# - BUILD={debug,release}
# - TRAP_FP_FAULTS ?= false
# - ARCH={x86,x64}

cmake_minimum_required(VERSION 3.10.2)
project(sudoku)

set(source_dir src)
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
set(GENERIC_CXX_FLAGS "-std=c++17 -pthread -fPIC -Wall -Werror -Wextra -Weffc++ -Wfloat-equal -Werror-implicit-function-declaration -Wundef -Wpointer-arith -Wcast-align -Wstrict-overflow=4 -Wwrite-strings -Wno-unused-result")
# Remove noisy warnings
set(GENERIC_CXX_FLAGS "${GENERIC_CXX_FLAGS} -Wcast-qual -Wswitch-default -Wswitch-enum -Wconversion -Wunreachable-code")

set(CLANG_CXX_FLAGS "${GENERIC_CXX_FLAGS}")

set(GCC_CXX_FLAGS "${GENERIC_CXX_FLAGS}")

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")  
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CLANG_CXX_FLAGS}")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_CXX_FLAGS}")
endif()
