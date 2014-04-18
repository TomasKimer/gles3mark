
#pragma once

#ifdef _WIN32
#include <GL/glew.h>
#else
#include "gl3stub_egl.h"
#endif

#include <string>
#include <stdexcept>

#include "log.h"

#ifdef _DEBUG
#define GL_CHECK(stmt) do { \
    stmt; \
    GLHelper::CheckOpenGLError(#stmt, __FILE__, __LINE__); \
        } while (0)
#else
#define GL_CHECK(stmt) stmt
#endif


class GLHelper {
public:
    static void GLInfo();

    // http://stackoverflow.com/questions/11256470/define-a-macro-to-facilitate-opengl-command-debugging
    static void CheckOpenGLError(const char* stmt, const char* fname, int line) {
        GLenum err = glGetError();
        if (err != GL_NO_ERROR) {
            Log::E() << "OpenGL error " << getGlErrorString(err) << ", at " << fname << ":" << line << " - for " << stmt;
            //abort();
        }
    }

    static std::string getGlErrorString(GLenum error);
    // TODO https://www.khronos.org/opengles/sdk/docs/man/xhtml/glCheckFramebufferStatus.xml
    // https://github.com/g-truc/ogl-samples/blob/master/framework/test.cpp

    struct GL_Exception : public std::runtime_error {
        GL_Exception(const GLenum error = glGetError()) throw()
            : std::runtime_error("OpenGL: " + getGlErrorString(error)) {}
        GL_Exception(const std::string& text, const GLenum error = glGetError()) throw()
            : std::runtime_error("OpenGL: " + text + " : " + getGlErrorString(error))
        {} 
    };
};
