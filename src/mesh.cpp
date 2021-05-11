
#include <iostream>
#include "shader_utils.h"
#include "mesh.h"

void Mesh::Init(int width, int height,  float windowWidth, float windowHeight)
{   
    float stepH = windowWidth / (width-1);
    float stepV = windowHeight / (height-1);

    float stepTexH = 1.0f / (width-1);
    float stepTexV = 1.0f / (height-1);

    float xStart = windowWidth/2;
    float yStart = windowHeight;

    numVert = width * height;
    numTriangles = (width - 1) * (height - 1) * 2;

    // 3 vert, 2 texture coords
    int stepLen = 5;
    verticesLen = (stepLen) * numVert * sizeof(vertices[0]);
    indicesLen = (stepLen) * numTriangles * sizeof(indices[0]);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    // vertices = new float[15] {
    //     0.5f, 0.5f, 0.0f,   // top right
    //     0.5f, -0.5f, 0.0f,  // bottom right
    //     -0.5f, -0.5f, 0.0f, // bottom left
    //     -0.5f, 0.5f, 0.0f,  // top left
    //     -0.8f, 0.8f, 0.0f};

    // indices = new unsigned int[9] {
    //     // note that we start from 0!
    //     0, 1, 3,
    //     1, 2, 3,
    //     1, 2, 4};

    vertices = new float[verticesLen];

    int x = 0, y = 0;
    for (int i = 0; i < stepLen * numVert; i += stepLen)
    {
        x = (i / stepLen) % width;
        y = (i / stepLen) / width;

        //vertices
        // from top left
        vertices[i] = -xStart + stepH * x;
        

        if (this->orient == 0) // vertical position
        {
        vertices[i + 1] = +yStart - stepV * y;
        vertices[i + 2] = 0.0f;
        }
        else // horizontal position
        {
        vertices[i + 1] = 0.0f;
        vertices[i + 2] = +yStart - stepV * y;
        }

        //texture coords
        vertices[i + 3] = 0.0f + stepTexH * x; //x
        vertices[i + 4] = 0.0f + stepTexV * y; //y

        //std::cout << vertices[i] << ' ' << vertices[(i+1)] << ' ' << vertices[(i+2)] << ' ' << vertices[(i+3)] << ' ' << vertices[(i+4)] << " || " << x << y << std::endl;
    }

    indices = new unsigned int[indicesLen];

    int block_x = 0, block_y = 0;
    for (int i = 0; i < 3 * numTriangles; i += 3)
    {
        block_x = (i/3 / 2) % (width-1);
        block_y = (i/3 / 2) / (width-1);

        //std::cout << i << std::endl;

        if (i % 2 == 0) // triangle |/
        {
            indices[i] = block_x + block_y * width;
            indices[i + 1] = indices[i] + 1;
            indices[i + 2] = block_x + (block_y + 1) * width;
        }
        else // triangle /|
        {
            indices[i] =  (block_x + 1) + (block_y) * width;
            indices[i + 1] = (block_x + 1) + (block_y + 1) * width;
            indices[i + 2] = indices[i + 1] - 1;
        }

        //std::cout << indices[i] << ' ' << indices[(i+1)] << ' ' << indices[(i+2)] << " || " << block_x << block_y << std::endl;
    }
}

void Mesh::linkShaders(const char * vertSh, const char * fragSh){
    
    this->shaderBuffer1 = shaderUtils::add_shader(vertSh);
    const char * vertexShaderSource = this->shaderBuffer1.c_str();
    
    this->shaderBuffer2 = shaderUtils::add_shader(fragSh);
    const char * fragmentShaderSource = this->shaderBuffer2.c_str();

    shaderUtils::buildShaderProgram(vertexShaderSource, fragmentShaderSource, this->shaderProgram);
    
    glGenVertexArrays(1, &(this->VAO));
    glGenBuffers(1, &(this->VBO));
    glGenBuffers(1, &(this->EBO));

    return;
}

void Mesh::setMesh(GLuint VBO, GLuint VAO, GLuint EBO)
{   
    float * vertices = this->getVertices();
    unsigned int * indices = this->getIndices();

    int vertices_length = this->getVeriticesLen();
    int indices_length = this->getIndicesLen();
    std::cout << vertices_length << std::endl;
    std::cout << indices_length << std::endl;
    
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices_length, vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_length, indices, GL_STATIC_DRAW);

    //vertexes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    //texture coords
    glVertexAttribPointer(1, 2, GL_FLOAT,GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);  

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

GLuint Mesh::loadTexture(const char * texturePath){
    // texture
    this->textureID = shaderUtils::createTexture(texturePath);

    return this->textureID;
}

void Mesh::freeResources(){
      // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &(this->VAO));
    glDeleteBuffers(1, &(this->VBO));
    glDeleteBuffers(1, &(this->EBO));
    glDeleteProgram(this->shaderProgram);
}