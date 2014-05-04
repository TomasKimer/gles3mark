
#pragma once

#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "glinclude.h"

#include "shaderprogram.h"
#include "materialdatabase.h"

class Model;

class ModelRenderer {
    
    Model* owner;  // TODO shared_ptr
    
    GLuint instanceVBO;
    unsigned instanceCount;

    void RenderFunc(const ShaderProgram& shader, const MaterialDatabase& matDB, bool instanced, int count);

public:
    ModelRenderer() : instanceCount(0), instanceVBO(0) {
    }

    void Init(Model* model);
    void InitInstanceData(const std::vector<glm::mat4>& data);

    void Render(const ShaderProgram& shader, const MaterialDatabase& matDB);
    void RenderInstanced(const ShaderProgram& shader, const MaterialDatabase& matDB, int count);
    
    void Destroy();

    unsigned GetInstanceCount() {
        return instanceCount;
    }
};