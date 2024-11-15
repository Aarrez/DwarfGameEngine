cmake_minimum_required(VERSION 3.29)
project(DwarfGameEngine)

set(CMAKE_CXX_STANDARD 20)


add_executable(DwarfGameEngine

        DwarfStb/stb_image.cpp

        #DwarfGL
        DwarfGL/ShaderProgramCode.h
        DwarfGL/DwarfShader.cpp
        DwarfGL/DwarfShader.h
        DwarfGL/ShaderProgramCode.cpp

        #DwarfEngine
        DwarfEngine/DwarfEngine.cpp
        DwarfEngine/DwarfInput.cpp
        DwarfEngine/DwarfInput.h

        #DwarfGL/Mesh2D
        DwarfGL/Mesh/DwarfMesh.cpp
        DwarfGL/Mesh/DwarfMesh.h

        main.cpp

        DwarfMisc/TestCode.cpp
        DwarfMisc/TestCode.h
        DwarfStb/DwarfGetImage.cpp
        DwarfStb/DwarfGetImage.h
        DwarfMisc/DwarfPath.h
        DwarfEngine/DwarfTransfrom.cpp
        DwarfEngine/DwarfTransfrom.h

)

find_package(Boost REQUIRED COMPONENTS filesystem)
target_link_libraries(DwarfGameEngine PRIVATE Boost::filesystem)

find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(DwarfGameEngine
        PRIVATE
        $<TARGET_NAME_IF_EXISTS::SDL2::SDL2main>
        $<IF:$<TARGET_EXISTS:SDL2::SDL2>, SDL2::SDL2, SDL2::SDL2-static>
)

find_package(SDL2_image CONFIG REQUIRED)
target_link_libraries(DwarfGameEngine PRIVATE
        $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,
        SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>)

find_package(Stb REQUIRED)
target_include_directories(DwarfGameEngine PRIVATE ${Stb_INCLUDE_DIR})

find_package(boost_filesystem CONFIG REQUIRED)
target_link_libraries(DwarfGameEngine PRIVATE Boost::filesystem)

find_package(glad CONFIG REQUIRED)
target_link_libraries(DwarfGameEngine PRIVATE glad::glad)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(DwarfGameEngine PRIVATE glfw)

find_package(glm CONFIG REQUIRED)
target_link_libraries(DwarfGameEngine PRIVATE glm::glm-header-only)


