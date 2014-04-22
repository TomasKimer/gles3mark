
#pragma once

#include <string>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/DefaultLogger.hpp>

#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>

#include "log.h"
#include "model.h"
#include "material.h"
#include "light.h"


class SceneImporter {
public:
    SceneImporter(/*const AssetManager& assetManager*/) /*: refAssetManager(assetManager)*/ {}
    virtual ~SceneImporter() = default;

    virtual Model* Import(/*const*/ std::vector<char>& rawModelData, std::vector<Material*>& materialDatabase) = 0;

protected:
    /*const AssetManager& refAssetManager;*/
};


class AssimpSceneImporter : public SceneImporter {
public:
    AssimpSceneImporter(/*const AssetManager& assetManager*/): SceneImporter(/*assetManager*/) {
        Assimp::DefaultLogger::set(new AssimpLogger());    
    }

    virtual ~AssimpSceneImporter() {
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

        unsigned materialZeroIndex = materialDatabase.size();
        for (unsigned int i = 0; i < aScene->mNumMeshes; ++i) {
            const aiMesh* aMesh = aScene->mMeshes[i];
            Mesh* m = LoadMesh(aMesh, materialZeroIndex); 
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

        for (unsigned int i = 0; i < aScene->mNumLights; ++i) {
            const aiLight* aLight = aScene->mLights[i];
            Light* l = LoadLight(aLight);
        }

        if (aScene->HasTextures  ()) Log::W() << "Has embedded textures.";
        if (aScene->HasAnimations()) Log::W() << "Has animations.";
        if (aScene->HasCameras   ()) Log::W() << "Has cameras.";

        return model;
    }

private:
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

    Mesh* LoadMesh(const aiMesh* aMesh, unsigned materialZeroIndex) {
        // vertices
        std::vector<glm::vec3> vertices(aMesh->mNumVertices);
        std::memcpy(&vertices[0].x, &aMesh->mVertices[0], aMesh->mNumVertices * sizeof(glm::vec3));
        
        // faces
        std::vector<glm::ivec3> faces(aMesh->mNumFaces);
        for (unsigned int j = 0; j < aMesh->mNumFaces; ++j) {
            faces[j] = glm::make_vec3(aMesh->mFaces[j].mIndices);   //assert(aMesh->mFaces[0].mNumIndices == 3);
        }

        // normals
        std::vector<glm::vec3> normals(aMesh->mNumVertices, glm::vec3(0));
        if (aMesh->HasNormals())
            std::memcpy(&normals[0].x, &aMesh->mNormals[0], aMesh->mNumVertices * sizeof(glm::vec3));

        // tangents (and bitangents?)
        std::vector<glm::vec3> tangents(aMesh->mNumVertices, glm::vec3(0));
        if (aMesh->HasTangentsAndBitangents())
            std::memcpy(&tangents[0].x, &aMesh->mTangents[0], aMesh->mNumVertices * sizeof(glm::vec3));
        
        // texture coordinates
        int texLevel = 0;
        std::vector<glm::vec2> texCoords(aMesh->mNumVertices, glm::vec2(0));
        if (aMesh->HasTextureCoords(texLevel)) {
            for (unsigned int j = 0; j < aMesh->mNumVertices; ++j) {
                texCoords[j] = glm::make_vec2(&aMesh->mTextureCoords[texLevel][j].x);                
            }
        }

        return new Mesh(vertices, faces, texCoords, normals, tangents, materialZeroIndex + aMesh->mMaterialIndex, std::string(aMesh->mName.C_Str()));
    }

    // http://assimp.sourceforge.net/lib_html/materials.html
    Material* LoadMaterial(const aiMaterial* aMat) {
        aiString name;
        if (aMat->Get(AI_MATKEY_NAME, name) != AI_SUCCESS) {
            Log::W() << "no name";
        }
        
        // textures
        aiString path;
        if (aMat->GetTexture(aiTextureType_DIFFUSE, 0, &path) != AI_SUCCESS)
            Log::W() << "no texture";
        
        // colors
        aiColor4D diffuse; //(0.f, 0.f, 0.f, 1.f);
        if (aiGetMaterialColor(aMat, AI_MATKEY_COLOR_DIFFUSE, &diffuse) != AI_SUCCESS)
            Log::W() << "no diffuse";
        
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

    Light* LoadLight(const aiLight *aLight) {
        Log::D() << "Light present: " << aLight->mName.data;
        
        Light* light = new Light(); 

        light->diffuseColor  = toGLM(aLight->mColorDiffuse );
        light->ambientColor  = toGLM(aLight->mColorAmbient );
        light->specularColor = toGLM(aLight->mColorSpecular);

        light->direction = toGLM(aLight->mDirection);
        light->position  = toGLM(aLight->mPosition );

        Log::D() << "Position: " << light->position.x << " " << light->position.y << " " << light->position.z;
        Log::D() << "Direction: " << light->direction.x << " " << light->direction.y << " " << light->direction.z;
        
        // TODO mAngleInnerCone, mAngleOuterCone, mAttenuationConstant, mAttenuationLinear, mAttenuationQuadratic

        switch (aLight->mType) {
            case aiLightSource_DIRECTIONAL:
                light->type = Light::Type::Directional;
                break;

            case aiLightSource_POINT:
                light->type = Light::Type::Point;
                break;

            case aiLightSource_SPOT:
                light->type = Light::Type::Spot;
                break;
        
            default:   // aiLightSource_UNDEFINED
                Log::W() << "Undefined light source type.";
        }
    
        return light;
    }

    
    class AssimpLogger : public Assimp::Logger {
        virtual void OnDebug(const char* msg) override {}// Log::D() << "Assimp: " << msg; }
        virtual void OnInfo (const char* msg) override { Log::V() << "Assimp: " << msg; }
        virtual void OnWarn (const char* msg) override {}// Log::W() << "Assimp: " << msg; }
        virtual void OnError(const char* msg) override { Log::E() << "Assimp: " << msg; }
        virtual bool attachStream (Assimp::LogStream*, unsigned) override { return true; }
        virtual bool detatchStream(Assimp::LogStream*, unsigned) override { return true; }
    };

    static glm::vec3 toGLM(const aiColor3D  & c) { return                glm::make_vec3(&c.r ) ; }
    static glm::vec3 toGLM(const aiVector3D & v) { return                glm::make_vec3(&v.x ) ; }
    static glm::vec4 toGLM(const aiColor4D  & c) { return                glm::make_vec4(&c.r ) ; }
    static glm::mat4 toGLM(const aiMatrix4x4& m) { return glm::transpose(glm::make_mat4(&m.a1)); }
};