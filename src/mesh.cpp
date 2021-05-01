#include "mesh.h"
#include <iostream>

void Mesh::Init(int width, int height)
{
    float stepH = 1.0f / (width-1);
    float stepV = 1.0f / (height-1);

    numVert = width * height;
    numTriangles = (width - 1) * (height - 1) * 2;

    verticesLen = 3 * numVert * sizeof(vertices[0]);
    indicesLen = 3 * numTriangles * sizeof(indices[0]);

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
    for (int i = 0; i < 3 * numVert; i += 3)
    {
        x = (i / 3) % width;
        y = (i / 3) / width;

        // from top left
        vertices[i] = -0.5f + stepH * x;
        vertices[i + 1] = +0.5f - stepV * y;
        vertices[i + 2] = 0.0f;

        //std::cout << vertices[i] << ' ' << vertices[(i+1)] << ' ' << vertices[(i+2)] << " || " << x << y << std::endl;
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