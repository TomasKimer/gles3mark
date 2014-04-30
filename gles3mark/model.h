

#pragma once

#include "mesh.h"
#include "modelrenderer.h"


class Model {
    friend class ModelRenderer;

    std::vector<Mesh*> meshes;
    glm::mat4 matrix;

public:
    ModelRenderer renderer;

    Model() {    
    }

    ~Model() {
        for (Mesh* mesh : meshes) {
            delete mesh;        
        }    
        meshes.clear();
    }

    void AddMesh(Mesh* mesh) {
        meshes.push_back(mesh);
    }

    void InitRenderer() {
        renderer.Init(this);
    }

    std::vector<Mesh*>& GetMeshes() {
        return meshes;
    }

};