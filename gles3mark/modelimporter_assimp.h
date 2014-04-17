
#pragma once

#include <string>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/DefaultLogger.hpp>

#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>

#include "modelimporter.h"



class AssimpModelImporter : public ModelImporter {
public:
    AssimpModelImporter(/*const AssetManager& assetManager*/): ModelImporter(/*assetManager*/) {
        Assimp::DefaultLogger::set(new AssimpLogger());    
    }

    virtual ~AssimpModelImporter() {
        Assimp::DefaultLogger::kill();    
    }

    virtual Model* Import(std::vector<char>& rawModelData, std::vector<Material*>& materialDatabase) override {   // 
        //std::vector<char> rawModelData = refAssetManager.LoadContents("models/" + fileName);
        
        Assimp::Importer aImporter;
        const aiScene* aScene = aImporter.ReadFileFromMemory(&rawModelData[0], rawModelData.size(), aiProcessPreset_TargetRealtime_Quality);
        if (!aScene) {
            throw std::runtime_error(std::string("Failed to load model: ") + aImporter.GetErrorString());
        }
        std::vector<char>().swap(rawModelData); // free memory

        Model* model = new Model();

        for (unsigned int i = 0; i < aScene->mNumMeshes; ++i) {
            const aiMesh* aMesh = aScene->mMeshes[i];
            Mesh* m = LoadMesh(aMesh);            
            model->AddMesh(m);
            // Log::Stream() << "Mesh " << i << " - Vertices: " << mesh->mNumVertices << ", faces: " << mesh->mNumFaces;  // normals, uv            
        }

        RecursiveTransform(model, aScene, aScene->mRootNode, glm::mat4());

        for (unsigned int i = 0; i < aScene->mNumMaterials; i++) {
            const aiMaterial* aMat = aScene->mMaterials[i];            
            Material* m = LoadMaterial(aMat);            
            materialDatabase.push_back(m);            
            //Log::V() << "Material " << i << " - name: " << name.C_Str() << (path.length > 0 ? (std::string(", texture: ") + path.data) : "");
        }

        return model;
    }


    static glm::mat4 toGLM(const aiMatrix4x4& m) {
        return glm::transpose(glm::make_mat4(&m.a1));
    }

    static glm::vec4 toGLM(const aiColor4D& c) {
        return glm::make_vec4(&c.r);
    }

private:
    class AssimpLogger : public Assimp::Logger {
        virtual void OnDebug(const char* msg) override {}// Log::D() << "Assimp: " << msg; }
        virtual void OnInfo (const char* msg) override { Log::V() << "Assimp: " << msg; }
        virtual void OnWarn (const char* msg) override {}// Log::W() << "Assimp: " << msg; }
        virtual void OnError(const char* msg) override { Log::E() << "Assimp: " << msg; }
        virtual bool attachStream (Assimp::LogStream*, unsigned) override { return true; }
        virtual bool detatchStream(Assimp::LogStream*, unsigned) override { return true; }
    };

    void RecursiveTransform(Model* model, const aiScene *aScene, const aiNode* aNode, glm::mat4 matrix) {
        glm::mat4 m = matrix * toGLM(aNode->mTransformation);      

        // transform all meshes assigned to this node
        for (unsigned i = 0; i < aNode->mNumMeshes; ++i) {
            Mesh* myMesh = model->GetMeshes()[aNode->mMeshes[i]];
            myMesh->matrix = m; 
        }

        // transform all children
        for (unsigned i = 0; i < aNode->mNumChildren; ++i) {
            RecursiveTransform(model, aScene, aNode->mChildren[i], m);
        }
    }

    Mesh* LoadMesh(const aiMesh* aMesh) {
        // vertices
        std::vector<glm::vec3> vertices(aMesh->mNumVertices);
        std::memcpy(&vertices[0].x, &aMesh->mVertices[0], aMesh->mNumVertices * sizeof(glm::vec3));
        
        // faces
        std::vector<glm::ivec3> faces(aMesh->mNumFaces);
        for (unsigned int j = 0; j < aMesh->mNumFaces; ++j) {
            std::memcpy(&faces[j].x, aMesh->mFaces[j].mIndices, sizeof(glm::ivec3));  //assert(aMesh->mFaces[0].mNumIndices == 3);
        }

        // normals
        std::vector<glm::vec3> normals(aMesh->mNumVertices);
        std::memcpy(&normals[0].x, &aMesh->mNormals[0], aMesh->mNumVertices * sizeof(glm::vec3));

        // tangents
        std::vector<glm::vec3> tangents(aMesh->mNumVertices);
        std::memcpy(&tangents[0].x, &aMesh->mTangents[0], aMesh->mNumVertices * sizeof(glm::vec3)); // TODO bitangents?
        
        // texture coordinates
        int texLevel = 0;
        std::vector<glm::vec2> texCoords;
        if (aMesh->HasTextureCoords(texLevel)) {
            texCoords.resize(aMesh->mNumVertices);
            for (unsigned int j = 0; j < aMesh->mNumVertices; ++j) {
                texCoords[j].s = aMesh->mTextureCoords[texLevel][j].x;
                texCoords[j].t = aMesh->mTextureCoords[texLevel][j].y; 
            }
        }

        return new Mesh(vertices, faces, texCoords, normals, tangents, aMesh->mMaterialIndex, std::string(aMesh->mName.C_Str()));
    }

    Material* LoadMaterial(const aiMaterial* aMat) {
        aiString name;
        if (aMat->Get(AI_MATKEY_NAME, name) != AI_SUCCESS) {
            Log::W() << "AI_MATKEY_NAME";
        }
        
        // textures
        aiString path;
        aMat->GetTexture(aiTextureType_DIFFUSE, 0, &path);
        
        // colors
        aiColor4D diffuse; //(0.f, 0.f, 0.f, 1.f);
        aiGetMaterialColor(aMat, AI_MATKEY_COLOR_DIFFUSE, &diffuse);
        
        aiColor4D ambient;
        if (aiGetMaterialColor(aMat, AI_MATKEY_COLOR_AMBIENT, &ambient) != AI_SUCCESS)
            Log::W() << "no ambient";
        
        aiColor4D specular;
        if (aiGetMaterialColor(aMat, AI_MATKEY_COLOR_SPECULAR, &specular) != AI_SUCCESS)
            Log::W() << "no specular";
        
        aiColor4D emissive;
        if (aiGetMaterialColor(aMat, AI_MATKEY_COLOR_EMISSIVE, &emissive) != AI_SUCCESS)
            Log::W() << "no emissive";
        
        float shininess = 0.0f;
        aiGetMaterialFloat(aMat, AI_MATKEY_SHININESS, &shininess);
        
        return new Material(toGLM(diffuse), toGLM(ambient), toGLM(specular), toGLM(emissive), shininess, std::string(path.C_Str()));
    }
};