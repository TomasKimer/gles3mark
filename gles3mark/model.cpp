
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