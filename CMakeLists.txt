cmake_minimum_required(VERSION 3.26)
project(XorFilterEval)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "Vcpkg toolchain file")

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

set(VCPKG_TARGET_TRIPLET "arm64-osx" CACHE STRING "")

find_package(xxhash CONFIG REQUIRED)

add_executable(XorFilterEval main.cpp)
target_link_libraries(XorFilterEval PRIVATE xxhash)
