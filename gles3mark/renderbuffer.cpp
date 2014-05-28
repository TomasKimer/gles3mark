/**
  *  gles3mark v1.0
  *
  *  \date   2014-05-28
  *  \author Tomas Kimer <xkimer00@stud.fit.vutbr.cz>
  */

#include "renderbuffer.h"
#include "glerror.h"
#include "glquery.h"

RenderBuffer::RenderBuffer() {
    GL_CHECK( glGenRenderbuffers(1, &rb) );
    maxSize = GLQuery::Get<GLint>(GL_MAX_RENDERBUFFER_SIZE);
    maxSamples = GLQuery::Get<GLint>(GL_MAX_SAMPLES);
}

RenderBuffer::~RenderBuffer() {
    GL_CHECK( glDeleteRenderbuffers(1, &rb) );
}

void RenderBuffer::InitStorage(GLenum internalFormat, GLsizei width, GLsizei height, GLsizei samples) {
    if (samples > maxSamples) samples = maxSamples;
    if (width > maxSize) width = maxSize;
    if (height > maxSize) height = maxSize;
    
    GL_CHECK( glBindRenderbuffer(GL_RENDERBUFFER, rb) );
    GL_CHECK( glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, internalFormat, width, height) );
}

void RenderBuffer::Bind() {
    GL_CHECK( glBindRenderbuffer(GL_RENDERBUFFER, rb) );
}