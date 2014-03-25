

#include <vector>

#include "glhelper.h"


void GLHelper::InitGL() {
#ifdef _WIN32
    GLenum error = glewInit();
    if (error != GLEW_OK)
        throw std::runtime_error("GLEW init failed");
#else
    if (!gl3stubInit())
        throw std::runtime_error("GL stub init failed");
#endif
}

void GLHelper::GLInfo() {
    Log::Msg(std::string("GL_VENDOR: ") + reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
    Log::Msg(std::string("GL_RENDERER: ") + reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
    Log::Msg(std::string("GL_VERSION: ") + reinterpret_cast<const char*>(glGetString(GL_VERSION)));
    Log::Msg(std::string("GL_SHADING_LANGUAGE_VERSION: ") + reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION)));
    //Log::Msg(std::string("GL_EXTENSIONS: ") + reinterpret_cast<const char*>(glGetString(GL_EXTENSIONS)));

    // glGet
    // https://www.khronos.org/opengles/sdk/docs/man/xhtml/glGet.xml
    // http://www.opengl.org/sdk/docs/man/html/glGet.xhtml
}

GLuint GLHelper::compileShader(GLenum type, const std::string& source) {
    GLuint shader = glCreateShader(type);
    if (shader == 0)
        throw std::runtime_error("glCreateShader failed");

#ifdef _WIN32
    std::string verSrc = "#version 330\n" + source;
#else
    std::string verSrc = "#version 300 es\n" + source;
#endif
    const char *c_str = verSrc.c_str();
    glShaderSource(shader, 1, &c_str, nullptr);
    glCompileShader(shader);

    std::string infoLog(getShaderInfoLog(shader));
    Log::Msg("Compile shader: " + (infoLog[0] == 0 ? std::string("OK") : infoLog));

    int compileStatus;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);

    if (compileStatus == GL_FALSE) {
        glDeleteShader(shader);
        throw std::runtime_error("shader compilation failed");
    }

    return shader;
}

GLuint GLHelper::linkShader(std::initializer_list<GLuint> shaders) {  // size_t count, ...
    GLuint program = glCreateProgram();
    if (program == 0)
        throw std::runtime_error("glCreateProgram failed");

    //    // Attach arguments
    //    va_list args;
    //    va_start(args, count);
    //    for (size_t i = 0; i < count; ++i) {
    //        glAttachShader(program, va_arg(args, GLuint));
    //    }
    //    va_end(args);

    for (GLuint shader : shaders) {
        glAttachShader(program, shader);
    }

    // Link program and check for errors
    glLinkProgram(program);

    std::string infoLog(getProgramInfoLog(program));
    Log::Msg("Link shaders: " + (infoLog[0] == 0 ? std::string("OK") : infoLog));
    
    int linkStatus;
    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);

    if (linkStatus == GL_FALSE) {
        glDeleteProgram(program);
        throw std::runtime_error("shader linking failed");
    }

    return program;
}

// Info logs contain errors and warnings from shader compilation and linking
std::string GLHelper::getShaderInfoLog(GLuint shader) {
    //assert(glIsShader(shader));

    GLint length;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

    std::vector<GLchar> log(length);
    glGetShaderInfoLog(shader, length, nullptr, &log[0]);

    //assert(glGetError() == GL_NO_ERROR);

    return std::string(&log[0], log.size());
}

std::string GLHelper::getProgramInfoLog(GLuint program) {
    //assert(glIsProgram(program));

    GLint length;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

    std::vector<GLchar> log(length);
    glGetProgramInfoLog(program, length, nullptr, &log[0]);

    //assert(glGetError() == GL_NO_ERROR);

    return std::string(&log[0], log.size());
}

std::string GLHelper::GL_Exception::getGlErrorString(GLenum error) {
#define GLERROR(e) case e : return std::string(#e)
    switch (error) {
        GLERROR(GL_NO_ERROR);
        GLERROR(GL_INVALID_ENUM);
        GLERROR(GL_INVALID_VALUE);
        GLERROR(GL_INVALID_OPERATION);
        GLERROR(GL_INVALID_FRAMEBUFFER_OPERATION);
        GLERROR(GL_OUT_OF_MEMORY);
        default: return std::string("UNKNOWN_GL_ERROR");
    }
#undef GLERROR
}
