cmake_minimum_required(VERSION 3.29)
project(DwarfGameEngine)

set(CMAKE_CXX_STANDARD 20)

add_executable(DwarfGameEngine
        DwarfEngine/DwarfEngine.cpp
        main.cpp)

find_package(glad CONFIG REQUIRED)
target_link_libraries(DwarfGameEngine PRIVATE glad::glad)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(DwarfGameEngine PRIVATE glfw)


