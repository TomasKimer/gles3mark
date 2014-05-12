
#pragma once

#include "rendertarget.h"
#include "glinclude.h"

// note: The width and height of the off-screen renderbuffers do not have to be a power of 2.
class RenderBuffer : public RenderTarget {
    friend class Framebuffer;
    
    GLuint rb;
    int maxSamples;

public:
    RenderBuffer();
    ~RenderBuffer();

    void InitStorage(GLenum internalFormat, GLsizei width, GLsizei height, GLsizei samples = 0);
    void Bind();
};