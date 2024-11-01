cmake_minimum_required(VERSION 3.29)
project(OpenglPlayground)

set(CMAKE_CXX_STANDARD 20)
set(PROJECT_NAME OpenglPlayground)

find_package(GLEW REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE GLEW::GLEW)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
