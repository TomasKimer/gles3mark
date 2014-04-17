

#pragma once

#include "mesh.h"


class Model {

    std::vector<Mesh*> meshes;

public:
    ~Model() {
        for (Mesh* mesh : meshes) {
            delete mesh;        
        }    
        meshes.clear();
    }

    void AddMesh(Mesh* mesh) {
        meshes.push_back(mesh);
    }

    std::vector<Mesh*>& GetMeshes() {
        return meshes;
    }

};