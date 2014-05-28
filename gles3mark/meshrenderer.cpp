

#include "meshrenderer.h"
#include "mesh.h"
#include "glerror.h"

void MeshRenderer::Init(Mesh* mesh) {
    this->owner = mesh;
    
    // Copy data to graphics card
    GL_CHECK( glGenBuffers(1, &VBO) );
    GL_CHECK( glBindBuffer(GL_ARRAY_BUFFER, VBO) );
    GL_CHECK( glBufferData(GL_ARRAY_BUFFER, mesh->vertices.size() * sizeof(VboEntry), nullptr, GL_STATIC_DRAW) );

    for (size_t i = 0; i < mesh->vertices.size(); ++i) {
        GL_CHECK( glBufferSubData(GL_ARRAY_BUFFER, i * sizeof(VboEntry) + offsetof(VboEntry, pos)     , sizeof(glm::vec3), &mesh->vertices [i]) );
        GL_CHECK( glBufferSubData(GL_ARRAY_BUFFER, i * sizeof(VboEntry) + offsetof(VboEntry, normal)  , sizeof(glm::vec3), &mesh->normals  [i]) );
        GL_CHECK( glBufferSubData(GL_ARRAY_BUFFER, i * sizeof(VboEntry) + offsetof(VboEntry, texcoord), sizeof(glm::vec2), &mesh->texCoords[i]) );
    }
    
    // setup vbo for index buffer
    GL_CHECK( glGenBuffers(1, &EBO) );
    GL_CHECK( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO) );
    GL_CHECK( glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->faces.size()*sizeof(glm::ivec3), &mesh->faces[0].x, GL_STATIC_DRAW) );


    // setup VAO
    // Narozdil od VBO neukladaji data o vrcholech (pozice, normala, ...), ale ukladaji reference na VBO a nastaveni atributu.
    // VAO usnadnuji a urychluji vykreslovani. Pro vykresleni staci aktivovat VAO a ten si pamatuje veskere nastaveni.
    // subsequent calls that change the vertex array state (glBindBuffer, glVertexAttribPointer, glEnableVertexAttribArray, and glDisableVertexAttribArray) will affect the new VAO.
    GL_CHECK( glGenVertexArrays(1, &VAO) );  // TODO UBO pro kazdy mesh?
    GL_CHECK( glBindVertexArray(VAO) );

    GL_CHECK( glBindBuffer(GL_ARRAY_BUFFER, VBO) );
    GL_CHECK( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO) );

    // setup vertex shader attribute bindings (connecting current <position and tc> buffer to associated 'in' variable in vertex shader)
    GL_CHECK( glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VboEntry), (const GLvoid*)offsetof(VboEntry, pos     )) );
    GL_CHECK( glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VboEntry), (const GLvoid*)offsetof(VboEntry, normal  )) );
    GL_CHECK( glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VboEntry), (const GLvoid*)offsetof(VboEntry, texcoord)) );

    // enable vertex buffers
    GL_CHECK( glEnableVertexAttribArray(0) );
    GL_CHECK( glEnableVertexAttribArray(1) );
    GL_CHECK( glEnableVertexAttribArray(2) );

    // unbind VAO
    GL_CHECK( glBindVertexArray(0) );

    elementsCount = mesh->faces.size() * 3;
}

void MeshRenderer::InitInstanceData(const std::vector<glm::mat4>& data) {
    instanceCount = data.size();
    
    instanceVBO = AllocInstanceData(data);
    AttachInstanceData(3, instanceVBO);
}
 
GLuint MeshRenderer::AllocInstanceData(const std::vector<glm::mat4>& data) {
    GLuint instanceVBO;
    
    GL_CHECK( glGenBuffers(1, &instanceVBO) );
    GL_CHECK( glBindBuffer(GL_ARRAY_BUFFER, instanceVBO) );
    GL_CHECK( glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(glm::mat4), &data[0], GL_STATIC_DRAW) );

    return instanceVBO;
}

void MeshRenderer::AttachInstanceData(GLuint location, GLuint instanceVBO) {
    GL_CHECK( glBindVertexArray(VAO) );
    GL_CHECK( glBindBuffer(GL_ARRAY_BUFFER, instanceVBO) );
    
    for (unsigned i = 0; i < 4; ++i) {
        GL_CHECK( glEnableVertexAttribArray(location + i) );
        GL_CHECK( glVertexAttribPointer    (location + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (const GLvoid*)(sizeof(glm::vec4) * i)) );
        GL_CHECK( glVertexAttribDivisor    (location + i, 1) );
    }

    GL_CHECK( glBindVertexArray(0) );
}

void MeshRenderer::PreRender(const ShaderProgram& shader, const Material& material, const glm::mat4& parent) {
    shader.SetUniform("model"       , parent * owner->matrix);
    shader.SetUniform("diffuseColor", material.diffuseColor); 
    shader.SetUniform("hasTexture"  , material.hasTexture);
    
    if (material.hasTexture)
       material.texture->Bind(GL_TEXTURE0);
}

void MeshRenderer::Render() {
    GL_CHECK( glBindVertexArray(VAO) );
    GL_CHECK( glDrawElements(GL_TRIANGLES, elementsCount, GL_UNSIGNED_INT, nullptr) );
    GL_CHECK( glBindVertexArray(0) );
}

void MeshRenderer::RenderInstanced(int count) {
    GL_CHECK( glBindVertexArray(VAO) );
    GL_CHECK( glDrawElementsInstanced(GL_TRIANGLES, elementsCount, GL_UNSIGNED_INT, nullptr, count) );
    GL_CHECK( glBindVertexArray(0) );
}

void MeshRenderer::Destroy() {
    if (instanceVBO)
        GL_CHECK( glDeleteBuffers(1, &instanceVBO) );
    
    GL_CHECK( glDeleteBuffers(1, &VBO) );
    GL_CHECK( glDeleteBuffers(1, &EBO) );
    GL_CHECK( glDeleteVertexArrays(1, &VAO) );
}