
#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "glinclude.h"
#include "glerror.h"

class ShaderProgram {
    GLuint shaderProgram;

    std::unordered_map<std::string, GLuint> uniforms;

public:
    ShaderProgram(const std::string& vertexSrc, const std::string& fragmentSrc) {
        GLuint vertexShader   = compileShader(GL_VERTEX_SHADER, vertexSrc);
        GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSrc);

        shaderProgram = linkShader({vertexShader, fragmentShader});

        GL_CHECK( glDeleteShader(vertexShader) );
        GL_CHECK( glDeleteShader(fragmentShader) );
    }

    ShaderProgram(GLuint vertexShader, GLuint fragmentShader) {
        shaderProgram = linkShader({vertexShader, fragmentShader});
    }
    
    ~ShaderProgram() {
        GL_CHECK( glUseProgram(0) );
        GL_CHECK( glDeleteProgram(shaderProgram) );
    }

    void Use() const {
        GL_CHECK( glUseProgram(shaderProgram) );
    }

    void AddUniform(const std::string& name) {
        GL_CHECK( uniforms[name] = glGetUniformLocation(shaderProgram, name.c_str()) );    
    }

    GLuint GetUniform(const std::string& name) const {    
        return uniforms.at(name);
    }

    void SetUniform(const std::string& s, int    i           ) const { GL_CHECK( glUniform1i       (uniforms.at(s), i                             ) ); }
    void SetUniform(const std::string& s, float  f           ) const { GL_CHECK( glUniform1f       (uniforms.at(s), f                             ) ); }
    void SetUniform(const std::string& s, const  glm::vec2& v) const { GL_CHECK( glUniform2fv      (uniforms.at(s), 1,           glm::value_ptr(v)) ); }
    void SetUniform(const std::string& s, const  glm::vec3& v) const { GL_CHECK( glUniform3fv      (uniforms.at(s), 1,           glm::value_ptr(v)) ); }
    void SetUniform(const std::string& s, const  glm::vec4& v) const { GL_CHECK( glUniform4fv      (uniforms.at(s), 1,           glm::value_ptr(v)) ); }
    void SetUniform(const std::string& s, const  glm::mat4& m) const { GL_CHECK( glUniformMatrix4fv(uniforms.at(s), 1, GL_FALSE, glm::value_ptr(m)) ); }

    void SetUniform(const std::string& s, const std::vector<glm::vec3>& v) const { GL_CHECK( glUniform3fv(uniforms.at(s), 3, &v[0].x) ); }

    static GLuint compileShader(GLenum type, const std::string& source);    

protected:
    static GLuint linkShader(std::initializer_list<GLuint> shaders);
    static std::string getInfoLog(GLuint shaderOrProgram);
};