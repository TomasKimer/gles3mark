
#pragma once

#include "glinclude.h"
//#include "shaderprogram.h"

// TODO Mesh + MeshRenderer?
class QuadRenderer {
    const int COUNT = 4;

    GLuint smallQuad, smallQuadTex, VAO;

public:
    QuadRenderer() {
        GLfloat texattribs[] = {
            1.0, 0.0,
            1.0, 1.0,
            0.0, 0.0,
            0.0, 1.0
        };
        
        glGenBuffers(1, &smallQuad);
        glBindBuffer(GL_ARRAY_BUFFER, smallQuad);
        glBufferData(GL_ARRAY_BUFFER, COUNT * 2 * sizeof(GLfloat), nullptr, GL_DYNAMIC_DRAW);

        glGenBuffers(1, &smallQuadTex);
        glBindBuffer(GL_ARRAY_BUFFER, smallQuadTex);
        glBufferData(GL_ARRAY_BUFFER, COUNT * 2 * sizeof(GLfloat), &texattribs, GL_STATIC_DRAW);
        

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        
        glBindBuffer(GL_ARRAY_BUFFER, smallQuad);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
       
        glBindBuffer(GL_ARRAY_BUFFER, smallQuadTex);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0); 

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);    

        glBindVertexArray(0);
    }

    ~QuadRenderer() {
        glDeleteBuffers(1, &smallQuad);
        glDeleteBuffers(1, &smallQuadTex); 
        glDeleteVertexArrays(1, &VAO);
    }

    void Render(float offsetX, float offsetY, float size) {
        // update vertex attributes
        float left   = -1.0f  + offsetX;
        float right  = left   + size;
        float bottom = -1.0f  + offsetY;
        float top    = bottom + size;

        GLfloat vertattribs[] = {    // { 1.0,-1.0, 1.0,1.0, -1.0,-1.0, -1.0,1.0 };
            right, bottom,
            right, top,
            left , bottom,
            left , top
        };

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, smallQuad);
        glBufferData(GL_ARRAY_BUFFER, COUNT * 2 * sizeof(GLfloat), &vertattribs, GL_DYNAMIC_DRAW);   // update vertex data
        glDrawArrays(GL_TRIANGLE_STRIP, 0, COUNT);

        glBindVertexArray(0);
    }
};