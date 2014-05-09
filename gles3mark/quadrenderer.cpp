
#include "quadrenderer.h"
#include "glerror.h"

QuadRenderer::QuadRenderer() {
    GLfloat texattribs[] = {
        1.0, 0.0,
        1.0, 1.0,
        0.0, 0.0,
        0.0, 1.0
    };
    
    GL_CHECK( glGenBuffers(1, &smallQuad) );
    GL_CHECK( glBindBuffer(GL_ARRAY_BUFFER, smallQuad) );
    GL_CHECK( glBufferData(GL_ARRAY_BUFFER, COUNT * 2 * sizeof(GLfloat), nullptr, GL_DYNAMIC_DRAW) );

    GL_CHECK( glGenBuffers(1, &smallQuadTex) );
    GL_CHECK( glBindBuffer(GL_ARRAY_BUFFER, smallQuadTex) );
    GL_CHECK( glBufferData(GL_ARRAY_BUFFER, COUNT * 2 * sizeof(GLfloat), &texattribs, GL_STATIC_DRAW) );
    

    GL_CHECK( glGenVertexArrays(1, &VAO) );
    GL_CHECK( glBindVertexArray(VAO) );
    
    GL_CHECK( glBindBuffer(GL_ARRAY_BUFFER, smallQuad) );
    GL_CHECK( glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0) );
   
    GL_CHECK( glBindBuffer(GL_ARRAY_BUFFER, smallQuadTex) );
    GL_CHECK( glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0) ); 

    GL_CHECK( glEnableVertexAttribArray(0) );
    GL_CHECK( glEnableVertexAttribArray(1) );    

    GL_CHECK( glBindVertexArray(0) );
}

QuadRenderer::~QuadRenderer() {
    GL_CHECK( glDeleteBuffers(1, &smallQuad) );
    GL_CHECK( glDeleteBuffers(1, &smallQuadTex) ); 
    GL_CHECK( glDeleteVertexArrays(1, &VAO) );
}

void QuadRenderer::Render(float offsetX, float offsetY, float size) {
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

    GL_CHECK( glBindVertexArray(VAO) );

    GL_CHECK( glBindBuffer(GL_ARRAY_BUFFER, smallQuad) );
    GL_CHECK( glBufferData(GL_ARRAY_BUFFER, COUNT * 2 * sizeof(GLfloat), &vertattribs, GL_DYNAMIC_DRAW) );   // update vertex data
    GL_CHECK( glDrawArrays(GL_TRIANGLE_STRIP, 0, COUNT) );

    GL_CHECK( glBindVertexArray(0) );
}