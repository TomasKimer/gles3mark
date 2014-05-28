/**
  *  gles3mark v1.0
  *
  *  \date   2014-05-28
  *  \author Tomas Kimer <xkimer00@stud.fit.vutbr.cz>
  */

#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "glinclude.h"

#include "shaderprogram.h"
#include "material.h"

class Mesh;

/**
 * \brief Mesh renderer.
 */
class MeshRenderer {
    friend class ModelRenderer;

    struct VboEntry {
        glm::vec3 pos;          // layout (location=0)
        glm::vec3 normal;       // layout (location=1)
        //glm::vec3 tangent;    
        glm::vec2 texcoord;     // layout (location=2)
    };

    GLuint VBO, EBO, VAO;
    Mesh* owner;
    unsigned elementsCount;

    GLuint instanceVBO;         // layout (location=3)
    unsigned instanceCount;

    static GLuint AllocInstanceData(const std::vector<glm::mat4>& data);
    void AttachInstanceData(GLuint location, GLuint instanceVBO);

public:
    MeshRenderer() : instanceCount(0), instanceVBO(0) {
    }
    
    void Init(Mesh* mesh);
    void InitInstanceData(const std::vector<glm::mat4>& data);
    
    void PreRender(const ShaderProgram& shader, const Material& material, const glm::mat4& parent = glm::mat4());
    void Render();
    void RenderInstanced(int count);
    
    void Destroy();

    unsigned GetInstanceCount() {
        return instanceCount;
    }
};
