cmake_minimum_required(VERSION 3.15)
project(ShaderPractice VERSION 1.0.0)

add_executable(ShaderPractice 
    src/main.cpp
    src/glad.c
    src/Input.cpp
)

# Include directories (e.g., include/, include/GLFW, include/glad)
target_include_directories(ShaderPractice PRIVATE include)

# Link directories where glfw3.lib is located
link_directories(lib)

# Link the executable to GLFW and OpenGL
# target_link_libraries(ShaderPractice PRIVATE glfw3 opengl32)
# directly declare glfw
target_link_libraries(ShaderPractice
    PRIVATE
    ${CMAKE_SOURCE_DIR}/lib/libglfw3.a
    opengl32
)