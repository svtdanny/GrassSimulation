cmake_minimum_required(VERSION 3.7)

project(main)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)

message(opgllib = ${OPENGL_LIBRARY} ${GLEW_LIBRARIES}  ${CMAKE_DL_LIBS})
message(include =  ${OPENGL_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O1 ")
set(TINYLINK "-lGL -lGLEW -lstdc++ -lglfw -lGL -lm -lX11 -lpthread -lXi -lXrandr -ldl")
set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${TINYLINK}")

include_directories(include)
add_subdirectory(lib/soil)

include_directories(${OPENGL_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})
file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.cpp")
#file(GLOB_RECURSE C_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.c")
file(GLOB_RECURSE GLAD_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/glad/glad.o")

add_executable(main ${SOURCES} ${C_SOURCES} ${GLAD_SOURCES})
target_link_libraries(main ${OPENGL_LIBRARY} ${GLEW_LIBRARIES}  ${CMAKE_DL_LIBS}   glfw)
