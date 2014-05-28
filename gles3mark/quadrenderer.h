/**
  *  gles3mark v1.0
  *
  *  \date   2014-05-28
  *  \author Tomas Kimer <xkimer00@stud.fit.vutbr.cz>
  */

#pragma once

#include "glinclude.h"
//#include "shaderprogram.h"

// TODO Mesh + MeshRenderer?
/**
 * \brief Quad renderer.
 */
class QuadRenderer {
    const int COUNT = 4;

    GLuint smallQuad, smallQuadTex, VAO;

public:
    QuadRenderer();
    ~QuadRenderer();

    void Render(float offsetX, float offsetY, float size);
};