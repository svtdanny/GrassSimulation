#pragma once

class Mesh
{
public:
    void Init(int width, int height);
    
    float * getVertices() {return vertices;};
    unsigned int * getIndices() {return indices;};

    // getters
    int getVeriticesLen() {return verticesLen;};
    int getIndicesLen() {return indicesLen;};
    int getNumVert() {return numVert;}
    int getNumTriangles() {return numTriangles;}

private:
    float * vertices;
    unsigned int * indices;

    int numVert;
    int numTriangles;

    int verticesLen;
    int indicesLen;
};