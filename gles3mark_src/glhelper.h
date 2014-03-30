
#pragma once


#ifdef _WIN32
#include <GL/glew.h>
#pragma comment(lib, "glew32.lib")
#include <GL/GL.h>
#pragma comment(lib, "opengl32.lib")
#else
//#include <GLES3/gl3.h> // TODO glimports.h
#include "gl3stub.h"
#endif

#include <string>
#include <stdexcept>

#include "log.h"


class GLHelper {
public:
    static void InitGL();
    static void GLInfo();

    static GLuint compileShader(GLenum type, const std::string& source);
    static GLuint linkShader(std::initializer_list<GLuint> shaders);

    // Info logs contain errors and warnings from shader compilation and linking
    static std::string getShaderInfoLog(GLuint shader);
    static std::string getProgramInfoLog(GLuint program);


    struct GL_Exception : public std::runtime_error {
        GL_Exception(const GLenum error = glGetError()) throw()
            : std::runtime_error("OpenGL: " + getGlErrorString(error)) {}
        GL_Exception(const std::string& text, const GLenum error = glGetError()) throw()
            : std::runtime_error("OpenGL: " + text + " : " + getGlErrorString(error)) {}

        std::string getGlErrorString(GLenum error);
        // TODO https://www.khronos.org/opengles/sdk/docs/man/xhtml/glCheckFramebufferStatus.xml
    };
};