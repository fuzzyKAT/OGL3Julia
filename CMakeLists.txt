cmake_minimum_required(VERSION 3.0.2)
set (PROJECT OGL3Julia)

project (${PROJECT})

set (HEADERS
    FractalRendererGL.h)

set (SOURCES
    FractalRendererGL.cpp
    main.cpp)

# find GLFW
find_package(glfw3 REQUIRED)
# find OpenGL
find_package(OpenGL REQUIRED)
# find GLEW
find_package(GLEW REQUIRED)

add_executable (${PROJECT} ${HEADERS} ${SOURCES})

# link libraries
target_link_libraries(${PROJECT} glfw)
target_link_libraries(${PROJECT} ${GLEW_LIBRARIES})
target_link_libraries(${PROJECT} ${OPENGL_LIBRARIES})