
#pragma once

#include <cstring>
#include "log.h"
#include "assetmanager.h"
#include "mesh.h"


class ModelImporter {
public:
    ModelImporter(const AssetManager& assetManager): refAssetManager(assetManager) {}
    virtual ~ModelImporter() = default;

    virtual Mesh* Import(const std::string& fileName) = 0;

protected:
    const AssetManager& refAssetManager;
};



#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/DefaultLogger.hpp>


class AssimpModelImporter : public ModelImporter {

public:
    AssimpModelImporter(const AssetManager& assetManager): ModelImporter(assetManager) {
        Assimp::DefaultLogger::set(new AssimpLogger());    
    }

    virtual ~AssimpModelImporter() {
        Assimp::DefaultLogger::kill();    
    }

    virtual Mesh* Import(const std::string& fileName) override {   // 
        std::vector<char> rawModelData = refAssetManager.LoadContents("models/" + fileName);
        
        Assimp::Importer importer;
        const aiScene* aScene = importer.ReadFileFromMemory(&rawModelData[0], rawModelData.size(), aiProcessPreset_TargetRealtime_Quality);
        if (!aScene) {
                throw std::runtime_error("Failed to load model: " + fileName + " - " + importer.GetErrorString());
        }

        for (unsigned int i = 9; i < aScene->mNumMeshes; ++i) { // ++i?
                const aiMesh* aMesh = aScene->mMeshes[i];
                //Log::Stream() << "Mesh " << i << " - Vertices: " << mesh->mNumVertices << ", faces: " << mesh->mNumFaces;  // normals, uv
                
                std::vector<glm::vec3 > vertices(aMesh->mNumVertices);
                std::memcpy(&vertices[0].x, &aMesh->mVertices[0], aMesh->mNumVertices * sizeof(glm::vec3));
                
                std::vector<glm::ivec3> faces(aMesh->mNumFaces);
                //assert(aMesh->mFaces[0].mNumIndices == 3);
                for (unsigned int j = 0; j < aMesh->mNumFaces; ++j) {
                    std::memcpy(&faces[j].x, aMesh->mFaces[j].mIndices, sizeof(glm::ivec3));
                }

                 //if (mesh->HasNormals())
                std::vector<glm::vec3> normals(aMesh->mNumVertices);
                std::memcpy(&normals[0].x, &aMesh->mNormals[0], aMesh->mNumVertices * sizeof(glm::vec3));
                
                std::vector<glm::vec2> texCoords(aMesh->mNumVertices);
                if (aMesh->HasTextureCoords(0)) {
                    Log::Stream() << "Has texture coords.";                    
                    //std::memcpy(&texCoords[0].x, &aMesh->mTextureCoords[0][0].x, aMesh->mNumVertices * sizeof(glm::vec2));                
                    for (unsigned int j = 0; j < aMesh->mNumVertices; ++j) {
                        texCoords[j].x = aMesh->mTextureCoords[0][j].x;
                        texCoords[j].y = aMesh->mTextureCoords[0][j].y; 
                    }
                }

                return new Mesh(vertices, faces, texCoords, normals, std::vector<glm::vec3>());
        }

        return nullptr;

       //for (unsigned int i = 0; i < aScene->mNumMaterials; i++) {
       //    const aiMaterial* material = aScene->mMaterials[i];
       //    Log::Stream() << "Material " << i << " - Diffuse textures: " << material->GetTextureCount(aiTextureType_DIFFUSE);
       //}
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
