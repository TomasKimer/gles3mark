/**
  *  gles3mark v1.0
  *
  *  \date   2014-05-28
  *  \author Tomas Kimer <xkimer00@stud.fit.vutbr.cz>
  */

#include "glerror.h"
#include "log.h"
//#include <cassert>

// Source: http://stackoverflow.com/questions/11256470/define-a-macro-to-facilitate-opengl-command-debugging
void GLError::CheckOpenGLError(const char* stmt, const char* fname, int line) {
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        Log::E() << "OpenGL error " << GetErrorString(err) << ", at " << fname << ":" << line << " - for " << stmt;
        //abort();
    }
}

std::string GLError::GetErrorString(GLenum error) {
#define GLERROR(e) case e : return std::string(#e)
    switch (error) {
        GLERROR(GL_NO_ERROR);
        GLERROR(GL_INVALID_ENUM);
        GLERROR(GL_INVALID_VALUE);
        GLERROR(GL_INVALID_OPERATION);
        GLERROR(GL_INVALID_FRAMEBUFFER_OPERATION);
        GLERROR(GL_OUT_OF_MEMORY);
        default: return std::string("GL_UNKNOWN_ERROR");
    }
#undef GLERROR
}