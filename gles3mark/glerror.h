
#pragma once

#include <string>
#include <stdexcept>

#include "glinclude.h"


#ifdef _DEBUG
#define GL_CHECK(stmt) do { \
    stmt; \
    GLError::CheckOpenGLError(#stmt, __FILE__, __LINE__); \
        } while (0)
#else
#define GL_CHECK(stmt) stmt
#endif

struct GLError {
    static void CheckOpenGLError(const char* stmt, const char* fname, int line);
    static std::string GetErrorString(GLenum error);

    struct GL_Exception : public std::runtime_error {
        GL_Exception(const GLenum error = glGetError()) throw()
            : std::runtime_error("OpenGL: " + GetErrorString(error)) {}
        GL_Exception(const std::string& text, const GLenum error = glGetError()) throw()
            : std::runtime_error("OpenGL: " + text + " : " + GetErrorString(error))
        {} 
    };
};
