
#pragma once

#include "mesh.h"
#include "modelrenderer.h"

class Model {
    friend class ModelRenderer;

    std::vector<Mesh*> meshes;

public:
    Model();
    ~Model();

    void AddMesh(Mesh* mesh);
    void InitRenderer();

    std::vector<Mesh*>& GetMeshes() { return meshes; }

    ModelRenderer renderer;
    glm::mat4 matrix;
};