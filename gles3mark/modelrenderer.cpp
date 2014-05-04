
#include "modelrenderer.h"
#include "model.h"

void ModelRenderer::Init(Model* model) {
    owner = model;
    
    for (Mesh* m : owner->meshes) {
       m->InitRenderer();
       m->FreeMemory();
   }
}

void ModelRenderer::InitInstanceData(const std::vector<glm::mat4>& data) {
    instanceCount = data.size();
    
    instanceVBO = MeshRenderer::AllocInstanceData(data);

    for (Mesh* m : owner->meshes) {
       m->renderer.AttachInstanceData(3, instanceVBO);
   }
}

void ModelRenderer::Render(const ShaderProgram& shader, const MaterialDatabase& matDB) {
    RenderFunc(shader, matDB, false, 0);
}

void ModelRenderer::RenderInstanced(const ShaderProgram& shader, const MaterialDatabase& matDB, int count) {
    RenderFunc(shader, matDB, true, count);
}

void ModelRenderer::RenderFunc(const ShaderProgram& shader, const MaterialDatabase& matDB, bool instanced, int count) {
    shader.SetUniform("isInstanced", instanced);
        
    for (Mesh* m : owner->meshes) {  //if (i == 5 || i == 7) continue; // chair realTime_quality bad mesh    
        m->renderer.PreRender(shader, *matDB.Get(m->materialID), owner->matrix);

        if (instanced)
            m->renderer.RenderInstanced(count);
        else
            m->renderer.Render();
    }
}

void ModelRenderer::Destroy() {
    if (instanceVBO)
        glDeleteBuffers(1, &instanceVBO);
}