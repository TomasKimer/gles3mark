/**
  *  gles3mark v1.0
  *
  *  \date   2014-05-28
  *  \author Tomas Kimer <xkimer00@stud.fit.vutbr.cz>
  */

#include "model.h"

Model::Model() {    
}

Model::~Model() {
    for (Mesh* mesh : meshes) {
        delete mesh;        
    }    
    meshes.clear();
}

void Model::AddMesh(Mesh* mesh) {
    meshes.push_back(mesh);
}

void Model::InitRenderer() {
    renderer.Init(this);
}