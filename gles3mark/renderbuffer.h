

#pragma once

#include "glerror.h"
#include "glquery.h"
#include "rendertarget.h"

// note: The width and height of the off-screen renderbuffers do not have to be a power of 2.
class RenderBuffer : public RenderTarget {
    friend class Framebuffer;
    
    GLuint rb;
    int maxSamples;

public:
    RenderBuffer() {
        GL_CHECK( glGenRenderbuffers(1, &rb) );
        maxSize = GLQuery::Get<GLint>(GL_MAX_RENDERBUFFER_SIZE);
        maxSamples = GLQuery::Get<GLint>(GL_MAX_SAMPLES);
    }

    ~RenderBuffer() {
        GL_CHECK( glDeleteRenderbuffers(1, &rb) );
    }

    void InitStorage(GLenum internalFormat, GLsizei width, GLsizei height, GLsizei samples = 0) {
        if (samples > maxSamples) samples = maxSamples;
        if (width > maxSize) width = maxSize;
        if (height > maxSize) height = maxSize;
        
        GL_CHECK( glBindRenderbuffer(GL_RENDERBUFFER, rb) );
        GL_CHECK( glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, internalFormat, width, height) );
    }

    void Bind() {
        GL_CHECK( glBindRenderbuffer(GL_RENDERBUFFER, rb) );
    }
};