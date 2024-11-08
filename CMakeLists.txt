cmake_minimum_required(VERSION 3.29)
project(DwarfGameEngine)

set(CMAKE_CXX_STANDARD 20)

add_executable(DwarfGameEngine
        main.cpp

        #DwarfEngine
        DwarfEngine/DwarfEngine.cpp
        DwarfEngine/DwarfInput.cpp
        DwarfEngine/DwarfInput.h

        #DwarfGL
        DwarfGL/ShaderProgramCode.h
        DwarfGL/DwarfShader.cpp
        DwarfGL/DwarfShader.h
        DwarfGL/ShaderProgramCode.cpp

        #DwarfGL/Mesh
        DwarfGL/Mesh/DwarfMesh.cpp
        DwarfGL/Mesh/DwarfMesh.h)

find_package(Boost REQUIRED COMPONENTS filesystem)
target_link_libraries(DwarfGameEngine PRIVATE Boost::filesystem)

find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(DwarfGameEngine
        PRIVATE
        $<TARGET_NAME_IF_EXISTS::SDL2::SDL2main>
        $<IF:$<TARGET_EXISTS:SDL2::SDL2>, SDL2::SDL2, SDL2::SDL2-static>
)

find_package(glad CONFIG REQUIRED)
target_link_libraries(DwarfGameEngine PRIVATE glad::glad)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(DwarfGameEngine PRIVATE glfw)

find_package(glm CONFIG REQUIRED)
target_link_libraries(DwarfGameEngine PRIVATE glm::glm-header-only)


