cmake_minimum_required(VERSION 3.10)

if(APPLE)
    set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64" CACHE STRING "" FORCE)
endif()

project(manymouse_osc C)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# add_library(manymouse SHARED "")
add_executable(manymouse_osc)

if(APPLE)
    target_link_libraries(manymouse_osc "-framework IOKit")
    target_link_libraries(manymouse_osc "-framework CoreFoundation")
endif()

if(WIN32)
    target_link_libraries(manymouse_osc PRIVATE Ws2_32)
endif()

file(GLOB_RECURSE cfiles RELATIVE ${PROJECT_SOURCE_DIR} ./src/*.c)

set_property(TARGET manymouse_osc PROPERTY C_STANDARD 99)
target_include_directories(manymouse_osc PUBLIC ./include)
target_sources(manymouse_osc PRIVATE ${cfiles})
