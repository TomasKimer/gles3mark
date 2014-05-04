
#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "glinclude.h"

class ShaderProgram {
    GLuint shaderProgram;

    std::unordered_map<std::string, GLuint> uniforms;

public:
    ShaderProgram(const std::string& vertexSrc, const std::string& fragmentSrc) {
        GLuint vertexShader   = compileShader(GL_VERTEX_SHADER, vertexSrc);
        GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSrc);

        shaderProgram = linkShader({vertexShader, fragmentShader});

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    ShaderProgram(GLuint vertexShader, GLuint fragmentShader) {
        shaderProgram = linkShader({vertexShader, fragmentShader});
    }
    
    ~ShaderProgram() {
        glUseProgram(0);
        glDeleteProgram(shaderProgram);
    }

    void Use() const {
        glUseProgram(shaderProgram);
    }

    void AddUniform(const std::string& name) {
        uniforms[name] = glGetUniformLocation(shaderProgram, name.c_str());    
    }

    GLuint GetUniform(const std::string& name) const {    
        return uniforms.at(name);
    }

    void SetUniform(const std::string& s, int i             ) const { glUniform1i       (uniforms.at(s), i                             ); }
    void SetUniform(const std::string& s, const glm::vec3& v) const { glUniform3fv      (uniforms.at(s), 1,           glm::value_ptr(v)); }
    void SetUniform(const std::string& s, const glm::vec4& v) const { glUniform4fv      (uniforms.at(s), 1,           glm::value_ptr(v)); }
    void SetUniform(const std::string& s, const glm::mat4& m) const { glUniformMatrix4fv(uniforms.at(s), 1, GL_FALSE, glm::value_ptr(m)); }

    static GLuint compileShader(GLenum type, const std::string& source);    

protected:
    static GLuint linkShader(std::initializer_list<GLuint> shaders);
    static std::string getInfoLog(GLuint shaderOrProgram);
};