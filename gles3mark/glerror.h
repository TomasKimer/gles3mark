
#pragma once

#include <string>
#include <stdexcept>

#include "glinclude.h"
#include "log.h"

#ifdef _DEBUG
#define GL_CHECK(stmt) do { \
    stmt; \
    GLError::CheckOpenGLError(#stmt, __FILE__, __LINE__); \
        } while (0)
#else
#define GL_CHECK(stmt) stmt
#endif

class GLError {
public:
    // http://stackoverflow.com/questions/11256470/define-a-macro-to-facilitate-opengl-command-debugging
    static void CheckOpenGLError(const char* stmt, const char* fname, int line) {
        GLenum err = glGetError();
        if (err != GL_NO_ERROR) {
            Log::E() << "OpenGL error " << GetErrorString(err) << ", at " << fname << ":" << line << " - for " << stmt;
            //abort();
        }
    }

    struct GL_Exception : public std::runtime_error {
        GL_Exception(const GLenum error = glGetError()) throw()
            : std::runtime_error("OpenGL: " + GetErrorString(error)) {}
        GL_Exception(const std::string& text, const GLenum error = glGetError()) throw()
            : std::runtime_error("OpenGL: " + text + " : " + GetErrorString(error))
        {} 
    };

    static std::string GetErrorString(GLenum error) {
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
};
