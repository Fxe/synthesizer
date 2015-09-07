cmake_minimum_required (VERSION 3.0)
project (Synthesizeasr)

set (Synthesizer_VERSION_MAJOR 0)
set (Synthesizer_VERSION_MINOR 1)

find_package(Boost 1.55 REQUIRED)
find_package(OMG_LIB REQUIRED)

add_subdirectory(src)

#add_executable(Synthesizer main.cpp)
