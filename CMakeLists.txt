cmake_minimum_required(VERSION 3.25)    # uoft UG machines
project(matching_engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)     # stops with error if compiler unable to call C++20
set(CMAKE_CXX_EXTENSIONS OFF)           # use -std=c++20 and not -std=gnu++20 (GCC's extras on top of std)

set(CMAKE_CXX_FLAGS_DEBUG "-O1 -g -fno-omit-frame-pointer -fsanitize=address,undefined")    # debug flags (ASan)
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "-fsanitize=address,undefined")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")     # O3 optimization w/ no asserts
if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64")
  string(APPEND CMAKE_CXX_FLAGS_RELEASE " -march=native")   # x86 only: use every instruction this exact CPU has
endif()


include(FetchContent)   
FetchContent_Declare(googletest   # GoogleTest test framework to ensure correctness after changes handling different scenarios and edge cases
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.15.2)
FetchContent_MakeAvailable(googletest)


add_library(lob INTERFACE)
target_include_directories(lob INTERFACE ${PROJECT_SOURCE_DIR}/include)   # anything that links lob can write #include "lob/..."

enable_testing()
include(GoogleTest)

add_executable(platform_test tests/platform_test.cpp)     # compile .cpp into .o and link into a test program
target_compile_options(platform_test PRIVATE -Wall -Wextra -Wpedantic)     # extra warnings for this program
target_link_libraries(platform_test PRIVATE lob GTest::gtest_main)
gtest_discover_tests(platform_test)     # register each TEST with ctest
