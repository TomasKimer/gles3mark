
#pragma once

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/DefaultLogger.hpp>

#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>

#include "modelimporter.h"


class AssimpModelImporter : public ModelImporter {
     Model* model;

public:
    AssimpModelImporter(/*const AssetManager& assetManager*/): ModelImporter(/*assetManager*/) {
        Assimp::DefaultLogger::set(new AssimpLogger());    
    }

    virtual ~AssimpModelImporter() {
        Assimp::DefaultLogger::kill();    
    }

    virtual Model* Import(const std::vector<char>& rawModelData) override {   // 
        //std::vector<char> rawModelData = refAssetManager.LoadContents("models/" + fileName);
        
        Assimp::Importer importer;
        const aiScene* aScene = importer.ReadFileFromMemory(&rawModelData[0], rawModelData.size(), aiProcessPreset_TargetRealtime_Quality);
        if (!aScene) {
                throw std::runtime_error(std::string("Failed to load model: ") + importer.GetErrorString());
        }

        model = new Model();

        for (unsigned int i = 0; i < aScene->mNumMeshes; ++i) {
                const aiMesh* aMesh = aScene->mMeshes[i];
                //Log::Stream() << "Mesh " << i << " - Vertices: " << mesh->mNumVertices << ", faces: " << mesh->mNumFaces;  // normals, uv
                
                std::vector<glm::vec3 > vertices(aMesh->mNumVertices);
                std::memcpy(&vertices[0].x, &aMesh->mVertices[0], aMesh->mNumVertices * sizeof(glm::vec3));
                
                std::vector<glm::ivec3> faces(aMesh->mNumFaces);
                for (unsigned int j = 0; j < aMesh->mNumFaces; ++j) {
                    std::memcpy(&faces[j].x, aMesh->mFaces[j].mIndices, sizeof(glm::ivec3));  //assert(aMesh->mFaces[0].mNumIndices == 3);
                }

                std::vector<glm::vec3> normals(aMesh->mNumVertices);
                std::memcpy(&normals[0].x, &aMesh->mNormals[0], aMesh->mNumVertices * sizeof(glm::vec3));
                
                std::vector<glm::vec2> texCoords(aMesh->mNumVertices);
                if (aMesh->HasTextureCoords(0)) {
                    //Log::Stream() << "Has texture coords.";                    
                    //std::memcpy(&texCoords[0].x, &aMesh->mTextureCoords[0][0].x, aMesh->mNumVertices * sizeof(glm::vec2));                
                    for (unsigned int j = 0; j < aMesh->mNumVertices; ++j) {
                        texCoords[j].s = aMesh->mTextureCoords[0][j].x;
                        texCoords[j].t = aMesh->mTextureCoords[0][j].y; 
                    }
                }

                model->AddMesh(new Mesh(vertices, faces, texCoords, normals, std::vector<glm::vec3>()));
        }

        RecursiveTransform(aScene, aScene->mRootNode, glm::mat4());

        return model;

       //for (unsigned int i = 0; i < aScene->mNumMaterials; i++) {
       //    const aiMaterial* material = aScene->mMaterials[i];
       //    Log::Stream() << "Material " << i << " - Diffuse textures: " << material->GetTextureCount(aiTextureType_DIFFUSE);
       //}
    }

    void RecursiveTransform(const aiScene *sc, const aiNode* nd, glm::mat4 matrix) {
        glm::mat4 m = matrix * ToGLMmatrix(nd->mTransformation);      

        // transform all meshes assigned to this node
        for (unsigned int n=0; n < nd->mNumMeshes; ++n){
            Mesh* myMesh = model->GetMeshes()[nd->mMeshes[n]];
            myMesh->matrix = m; 
        }

        // transform all children
        for (unsigned int n=0; n < nd->mNumChildren; ++n){
            RecursiveTransform(sc, nd->mChildren[n], m);
        }
    }

    static glm::mat4 ToGLMmatrix(const aiMatrix4x4& m) {
        return glm::transpose(glm::make_mat4(&m.a1));
    }

private:
    class AssimpLogger : public Assimp::Logger {
        virtual void OnDebug(const char* msg) override {}// Log::D() << "Assimp: " << msg; }
        virtual void OnInfo (const char* msg) override { Log::V() << "Assimp: " << msg; }
        virtual void OnWarn (const char* msg) override { Log::W() << "Assimp: " << msg; }
        virtual void OnError(const char* msg) override { Log::E() << "Assimp: " << msg; }
        virtual bool attachStream (Assimp::LogStream*, unsigned) override { return true; }
        virtual bool detatchStream(Assimp::LogStream*, unsigned) override { return true; }
    };
};