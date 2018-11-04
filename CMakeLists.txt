cmake_minimum_required(VERSION 3.7 FATAL_ERROR)

set(ApplName Culicidae)

project (${ApplName})

# The version number.
set (Tutorial_VERSION_MAJOR 1)
set (Tutorial_VERSION_MINOR 0)

#find_package(PCL 1.2 REQUIRED)

add_subdirectory(./mosquitto/lib ./mosquitto/lib/cpp EXCLUDE_FROM_ALL)

link_directories(./mosquitto/lib ./mosquitto/lib/cpp)

#add_definitions()
#string resourcePath = string(RESOURCE_PATH) + "file.png";

add_executable (${ApplName} main.cpp mqtt.cpp mqtt.h meta.h)

target_compile_options(${ApplName} PUBLIC -std=c++11 -Wall)

target_include_directories(${ApplName} PUBLIC ./ ./mosquitto/lib ./mosquitto/lib/cpp)

target_link_libraries (${ApplName} PUBLIC libmosquitto libmosquittopp)

# End of CMakeLists.Txt file