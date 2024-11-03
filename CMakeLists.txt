cmake_minimum_required(VERSION 3.29)
project(OpenglPlayground)

set(CMAKE_CXX_STANDARD 20)
set(PROJECT_NAME OpenglPlayground)

find_package(GLEW REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Stb REQUIRED)

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/mesh/mesh.cpp
        src/mesh/mesh.h
        src/shader/Shader.cpp
        src/shader/Shader.h
        src/window/window.cpp
        src/window/window.h
        src/camera/camera.cpp
        src/camera/camera.h
        src/texture/texture.cpp
        src/texture/texture.h
        src/light/light.cpp
        src/light/light.h
        src/material/material.cpp
        src/material/material.h
        src/light/directionallight.cpp
        src/light/directionallight.h
        src/light/pointlight.cpp
        src/light/pointlight.h)

target_link_libraries(${PROJECT_NAME} PRIVATE GLEW::GLEW)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm)
target_include_directories(${PROJECT_NAME} PRIVATE ${Stb_INCLUDE_DIR})

# Post-build command to copy shaders/ folder
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shaders
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders
        COMMENT "Copying shaders/ directory to build folder"
)

# Post-build command to copy textures/ folder
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/textures
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/textures
        COMMENT "Copying textures/ directory to build folder"
)