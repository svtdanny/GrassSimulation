#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "glad/glad.h"
#include <GLFW/glfw3.h>



class Camera
{
public:
    void Init();

    glm::mat4 Projection;
    glm::mat4 View;

    glm::mat4 PV;
};