
#pragma once

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/DefaultLogger.hpp>

#include "log.h"
#include "assetmanager.h"


class ModelImporter {
public:
    class AssimpModel {

        Assimp::Importer importer; // !! must be alive during the scene processing
        const aiScene* scene;

    public:
        void Load(AssetManager& assetManager, const std::string& fileName) {
            std::vector<char> rawModelData = assetManager.LoadContents("models/" + fileName);
           
            scene = importer.ReadFileFromMemory(&rawModelData[0], rawModelData.size(), aiProcessPreset_TargetRealtime_Quality);
            if (!scene) {
                throw std::runtime_error("Failed to load model: " + fileName + " - " + importer.GetErrorString());
            }
        }

        void Process() {
            for (unsigned int i = 0; i < scene->mNumMeshes; i++) { // ++i?
                const aiMesh* mesh = scene->mMeshes[i];
                Log::Stream() << "Mesh " << i << " - Vertices: " << mesh->mNumVertices << ", faces: " << mesh->mNumFaces;  // normals, uv
                if (mesh->HasTextureCoords(0))
                    Log::Stream() << "Mesh " << i << " - has texture coords";

                //if (mesh->HasNormals())
                //    Log::Stream() << "Mesh " << i << " - has normals";
            }

            for (unsigned int i = 0; i < scene->mNumMaterials; i++) {
                const aiMaterial* material = scene->mMaterials[i];
                Log::Stream() << "Material " << i << " - Diffuse textures: " << material->GetTextureCount(aiTextureType_DIFFUSE);
            }
        }


    } model;


    class AssimpLogger : public Assimp::Logger {
        virtual void OnDebug(const char* msg) override {}// Log::D() << "Assimp: " << msg; }
        virtual void OnInfo (const char* msg) override { Log::V() << "Assimp: " << msg; }
        virtual void OnWarn (const char* msg) override { Log::W() << "Assimp: " << msg; }
        virtual void OnError(const char* msg) override { Log::E() << "Assimp: " << msg; }
        virtual bool attachStream (Assimp::LogStream*, unsigned) override { return true; }
        virtual bool detatchStream(Assimp::LogStream*, unsigned) override { return true; }
    };


public:
    ModelImporter() {
        Assimp::Logger::LogSeverity severity = Assimp::Logger::NORMAL;
        Assimp::DefaultLogger::set(new AssimpLogger());
        //Assimp::DefaultLogger::create("", severity, aiDefaultLogStream_STDERR);
        //Assimp::DefaultLogger::create("assimp_log.txt", severity, aiDefaultLogStream_FILE);
    }

    ~ModelImporter() {
        Assimp::DefaultLogger::kill();
    }
};