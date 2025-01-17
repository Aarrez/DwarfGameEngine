cmake_minimum_required(VERSION 3.29)
project(GameEngine)

set(CMAKE_CXX_STANDARD 20)


add_executable(GameEngine

        Stb/stb_image.cpp

        #DwarfGL
        GL/ShaderProgramCode.h
        GL/Shader.cpp
        GL/Shader.h
        GL/ShaderProgramCode.cpp

        #DwarfEngine
        Engine/GameEngine.cpp
        Engine/Input.cpp
        Engine/Input.h

        #DwarfGL/Mesh2D
        GL/Mesh.cpp
        GL/Mesh.h

        #DwarfMisc
        Misc/TestCode.cpp
        Misc/TestCode.h
        Misc/Path.h
        Misc/Memory.cpp
        Misc/Memory.h
        Misc/PublicStructs.h

        main.cpp


        Stb/GetImage.cpp
        Stb/GetImage.h

        Engine/Camera.cpp
        Engine/Camera.h
        GL/VirtualObject.cpp
        GL/VirtualObject.h
        Engine/EntityManager.cpp
        Engine/EntityManager.h
        FileLoader/OBJLoader.cpp
        FileLoader/OBJLoader.h
        Misc/PublicEnums.h
        FileLoader/IterativeOBJParser.cpp
        FileLoader/IterativeOBJParser.h
        Threads/ThreadManager.cpp
        Threads/ThreadManager.h
        Engine/MeshManager.cpp
        Engine/MeshManager.h
        Engine/TextureManager.cpp
        Engine/TextureManager.h
        Message/Message.cpp
        Message/Message.h
        Message/MeshMessage.cpp
        Message/MeshMessage.h
        Message/MessageQueue.cpp
        Message/MessageQueue.h
        Threads/ThreadQueue.cpp
        Threads/ThreadQueue.h
        Threads/ThreadPool.cpp
        Threads/ThreadPool.h
)


find_package(assimp CONFIG REQUIRED)
target_link_libraries(GameEngine PRIVATE assimp::assimp)

find_package(imgui CONFIG REQUIRED)
target_link_libraries(GameEngine PRIVATE imgui::imgui)

find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(GameEngine PRIVATE
        $<TARGET_NAME_IF_EXISTS::SDL2::SDL2main>
        $<IF:$<TARGET_EXISTS:SDL2::SDL2>, SDL2::SDL2, SDL2::SDL2-static>
)

find_package(SDL2_image CONFIG REQUIRED)
target_link_libraries(GameEngine PRIVATE
        $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,
        SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>)

find_package(Stb REQUIRED)
target_include_directories(GameEngine PRIVATE ${Stb_INCLUDE_DIR})

find_package(glad CONFIG REQUIRED)
target_link_libraries(GameEngine PRIVATE glad::glad)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(GameEngine PRIVATE glfw)

find_package(glm CONFIG REQUIRED)
target_link_libraries(GameEngine PRIVATE glm::glm-header-only)

