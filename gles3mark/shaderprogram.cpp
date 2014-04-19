
#include <cassert>

#include "shaderprogram.h"
#include "log.h"

#ifdef _WIN32
const std::string SHADER_VERSION("#version 330 core");  // 430?
#else
const std::string SHADER_VERSION("#version 300 es");
#endif

GLuint ShaderProgram::compileShader(GLenum type, const std::string& source) {
    GLuint shader = glCreateShader(type);
    if (shader == 0)
        throw std::runtime_error("glCreateShader failed");

    std::string verSrc = SHADER_VERSION + "\n" + source;
    const char *c_str = verSrc.c_str();

    glShaderSource(shader, 1, &c_str, nullptr);
    glCompileShader(shader);

    std::string infoLog(getInfoLog(shader));
    if (infoLog[0] != 0)
        Log::W() << "Compile shader: " << infoLog;

    int compileStatus;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);

    if (compileStatus == GL_FALSE) {
        glDeleteShader(shader);
        throw std::runtime_error("shader compilation failed");
    }

    return shader;
}

GLuint ShaderProgram::linkShader(std::initializer_list<GLuint> shaders) {  // size_t count, ...
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
    if (infoLog[0] != 0)
        Log::W() << "Link shaders: " << infoLog;
    
    int linkStatus;
    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);

    if (linkStatus == GL_FALSE) {
        glDeleteProgram(program);
        throw std::runtime_error("shader linking failed");
    }

    return program;
}

// Info logs contain errors and warnings from shader compilation and linking
std::string ShaderProgram::getInfoLog(GLuint shaderOrProgram) {
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