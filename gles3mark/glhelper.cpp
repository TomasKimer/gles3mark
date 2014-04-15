

#include <vector>

#include "glhelper.h"
#include <cassert>


static std::string GLString(GLenum name) {
    return std::string(reinterpret_cast<const char*>(glGetString(name)));
}

void GLHelper::GLInfo() {
    Log::Msg("GL_VENDOR: "                   + GLString(GL_VENDOR                  ));
    Log::Msg("GL_RENDERER: "                 + GLString(GL_RENDERER                ));
    Log::Msg("GL_VERSION: "                  + GLString(GL_VERSION                 ));
    Log::Msg("GL_SHADING_LANGUAGE_VERSION: " + GLString(GL_SHADING_LANGUAGE_VERSION));
    //Log::Msg("GL_EXTENSIONS: " + GLString(GL_EXTENSIONS));  glGetStringi

    // glGet
    // https://www.khronos.org/opengles/sdk/docs/man/xhtml/glGet.xml
    // http://www.opengl.org/sdk/docs/man/html/glGet.xhtml
}


GLuint GLHelper::compileShader(GLenum type, const std::string& source) {
    GLuint shader = glCreateShader(type);
    if (shader == 0)
        throw std::runtime_error("glCreateShader failed");

    std::string verSrc = SHADER_VERSION + "\n" + source;
    const char *c_str = verSrc.c_str();

    glShaderSource(shader, 1, &c_str, nullptr);
    glCompileShader(shader);

    std::string infoLog(getInfoLog(shader));
    Log::V() << "Compile shader: " << (infoLog[0] == 0 ? "OK" : infoLog);

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

    // Attach arguments
    for (GLuint shader : shaders) {
        glAttachShader(program, shader);
    }

    // Link program and check for errors
    glLinkProgram(program);

    std::string infoLog(getInfoLog(program));
    Log::V() << "Link shaders: " << (infoLog[0] == 0 ? "OK" : infoLog);
    
    int linkStatus;
    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);

    if (linkStatus == GL_FALSE) {
        glDeleteProgram(program);
        throw std::runtime_error("shader linking failed");
    }

    return program;
}

// Info logs contain errors and warnings from shader compilation and linking
std::string GLHelper::getInfoLog(GLuint shaderOrProgram) {
    GLint length;
    std::vector<GLchar> log;
    
    if (glIsShader(shaderOrProgram)) {
        glGetShaderiv(shaderOrProgram, GL_INFO_LOG_LENGTH, &length);
        log.resize(length);
        glGetShaderInfoLog(shaderOrProgram, length, nullptr, &log[0]);
    }
    else if (glIsProgram) {
        glGetProgramiv(shaderOrProgram, GL_INFO_LOG_LENGTH, &length);
        log.resize(length);
        glGetProgramInfoLog(shaderOrProgram, length, nullptr, &log[0]);
    }
    else
        throw std::invalid_argument("Argument is neither shader nor program.");

    assert(glGetError() == GL_NO_ERROR);

    return std::string(&log[0], log.size());
}


std::string GLHelper::getGlErrorString(GLenum error) {
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
