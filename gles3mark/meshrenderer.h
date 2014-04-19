

#pragma once

#include <glm/glm.hpp>
#include "glinclude.h"

class Mesh;

class MeshRenderer {
    struct VboEntry {
        glm::vec3 pos;          // layout (location=0)
        glm::vec3 normal;       // layout (location=1)
        //glm::vec3 tangent;    
        glm::vec2 texcoord;     // layout (location=2)
    };

    GLuint VBO, EBO, VAO;
    Mesh* owner;
    unsigned elementsCount;

public:
    void Init(Mesh* mesh);
    void Render();
    void Destroy();
};