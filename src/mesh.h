#pragma once
#include "glm/glm.hpp"

#include "glad/glad.h"
#include <GLFW/glfw3.h>


class Mesh
{
public:
    void Init(int width, int height, float windowWidth, float windowHeight);
    void linkShaders(const char * vertSh, const char * fragSh);
    void setMesh(GLuint VBO, GLuint VAO, GLuint EBO);

    void freeResources();

    GLuint loadTexture(const char * texturePath);
    
    float * getVertices() {return vertices;};
    unsigned int * getIndices() {return indices;};

    // getters
    int getVeriticesLen() {return verticesLen;};
    int getIndicesLen() {return indicesLen;};
    int getNumVert() {return numVert;}
    int getNumTriangles() {return numTriangles;}

    GLuint shaderTime;
    GLuint shaderProgram;
    GLuint textureID;
    GLuint PVID;

    GLuint VBO, VAO, EBO;

    std::string shaderBuffer1, shaderBuffer2;

    int orient = 0;
private:
    float windowWidth;
    float windowHeight;

    float * vertices;
    unsigned int * indices;

    int numVert;
    int numTriangles;

    int verticesLen;
    int indicesLen;

    glm::mat4 Model;
};