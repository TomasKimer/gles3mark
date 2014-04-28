
#pragma once

#include "glinclude.h"
//#include "shaderprogram.h"

// TODO Mesh + MeshRenderer?
class QuadRenderer {
    const int COUNT = 4;

    GLuint smallQuad, smallQuadTex, VAO;

public:
    QuadRenderer();
    ~QuadRenderer();

    void Render(float offsetX, float offsetY, float size);
};