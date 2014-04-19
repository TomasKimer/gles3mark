
#include <string>
#include <vector>
#include <map>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "glinclude.h"

class ShaderProgram {
    GLuint vertexShader, fragmentShader, shaderProgram;

    std::map<std::string, GLuint> uniforms;

public:
    ShaderProgram(const std::string& vertexSrc, const std::string& fragmentSrc) {
        vertexShader   = compileShader(GL_VERTEX_SHADER, vertexSrc);
        fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSrc);

        shaderProgram = linkShader({vertexShader, fragmentShader});

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }
    
    ~ShaderProgram() {
        glUseProgram(0);
        glDeleteProgram(shaderProgram);
    }

    void Use() {
        glUseProgram(shaderProgram);
    }

    void AddUniform(const std::string& name) {
        uniforms[name] = glGetUniformLocation(shaderProgram, name.c_str());    
    }

    GLuint GetUniform(const std::string& name) {    
        return uniforms[name];
    }

    void SetUniform(const std::string& s, int i             ) { glUniform1i       (uniforms[s], i                             ); }
    void SetUniform(const std::string& s, const glm::vec3& v) { glUniform3fv      (uniforms[s], 1,           glm::value_ptr(v)); }
    void SetUniform(const std::string& s, const glm::vec4& v) { glUniform4fv      (uniforms[s], 1,           glm::value_ptr(v)); }
    void SetUniform(const std::string& s, const glm::mat4& m) { glUniformMatrix4fv(uniforms[s], 1, GL_FALSE, glm::value_ptr(m)); }


protected:
    static GLuint compileShader(GLenum type, const std::string& source);
    static GLuint linkShader(std::initializer_list<GLuint> shaders);
    static std::string getInfoLog(GLuint shaderOrProgram);
};