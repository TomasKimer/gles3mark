/**
  *  gles3mark v1.0
  *
  *  \date   2014-05-28
  *  \author Tomas Kimer <xkimer00@stud.fit.vutbr.cz>
  */

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