#pragma once
#include <SOIL/SOIL.h>

#include <string>
#include "glad/glad.h"
#include <GLFW/glfw3.h>

namespace shaderUtils
{
    std::string add_shader(std::string fileName);
    void buildShaderProgram(const char * vertexShaderSource, const char * fragmentShaderSource, GLuint shaderProgram);
    GLuint createTexture(const char* textureFile);
}