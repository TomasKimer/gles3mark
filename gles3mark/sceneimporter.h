/**
  *  gles3mark v1.0
  *
  *  \date   2014-05-28
  *  \author Tomas Kimer <xkimer00@stud.fit.vutbr.cz>
  */

#pragma once

#include <string>
#include <memory>

#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/DefaultLogger.hpp>

#include "log.h"
#include "model.h"
#include "material.h"
#include "light.h"
#include "materialdatabase.h"


class ISceneImporter {
public:
    virtual ~ISceneImporter() = default;
    virtual Model* Import(/*const*/ std::vector<char>& rawModelData, MaterialDatabase& materialDatabase,
                          std::vector<std::unique_ptr<Light>>& lightDatabase) = 0;
};

class AssimpSceneImporter : public ISceneImporter {
public:
    AssimpSceneImporter() {
        Assimp::DefaultLogger::set(new AssimpLogger());    
    }

    virtual ~AssimpSceneImporter() {
        Assimp::DefaultLogger::kill();    
    }

    virtual Model* Import(std::vector<char>& rawModelData, MaterialDatabase& materialDatabase,
                          std::vector<std::unique_ptr<Light>>& lightDatabase) override;

private:
    void RecursiveTransform(Model* model, const aiScene *aScene, const aiNode* aNode, glm::mat4 matrix);

    Mesh* LoadMesh(const aiMesh* aMesh, unsigned materialZeroIndex);
    Material* LoadMaterial(const aiMaterial* aMat);
    Light* LoadLight(const aiLight *aLight);

    class AssimpLogger : public Assimp::Logger {
        virtual void OnDebug(const char* msg) override {} // Log::D() << "Assimp: " << msg; }
        virtual void OnInfo (const char* msg) override { Log::V() << "Assimp: " << msg; }
        virtual void OnWarn (const char* msg) override {} // Log::W() << "Assimp: " << msg; }
        virtual void OnError(const char* msg) override { Log::E() << "Assimp: " << msg; }
        virtual bool attachStream (Assimp::LogStream*, unsigned) override { return true; }
        virtual bool detatchStream(Assimp::LogStream*, unsigned) override { return true; }
    };

    static glm::vec3 toGLM(const aiColor3D  & c) { return                glm::make_vec3(&c.r ) ; }
    static glm::vec3 toGLM(const aiVector3D & v) { return                glm::make_vec3(&v.x ) ; }
    static glm::vec4 toGLM(const aiColor4D  & c) { return                glm::make_vec4(&c.r ) ; }
    static glm::mat4 toGLM(const aiMatrix4x4& m) { return glm::transpose(glm::make_mat4(&m.a1)); }
};