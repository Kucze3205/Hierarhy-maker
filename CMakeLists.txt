cmake_minimum_required(VERSION 3.10)
project(PokerCalculator)

set(CMAKE_CXX_STANDARD 11)

# Set up vcpkg
if(DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
else()
    message(FATAL_ERROR "VCPKG_ROOT environment variable not set")
endif()

find_package(nlohmann_json REQUIRED)

add_executable(PokerCalculator "Poker Calculator.cpp")
target_link_libraries(PokerCalculator PRIVATE nlohmann_json::nlohmann_json)
