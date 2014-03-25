
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

        Assimp::Importer importer;
        const aiScene* scene;

    public:
        void Load(AssetManager& assetManager, const std::string& fileName) {
            std::vector<char> rawModelData = assetManager.LoadContents("models/" + fileName);
            scene = importer.ReadFileFromMemory(&rawModelData[0], rawModelData.size(), aiProcessPreset_TargetRealtime_Quality);
            if (!scene) {
                throw std::runtime_error("Failed to load model: " + fileName + " - " + importer.GetErrorString());
            }

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
        virtual void OnDebug(const char* message) {}
        virtual void OnInfo (const char* message) { Log::Msg(std::string("Assimp: ") + message, Log::Severity::Verbose); }
        virtual void OnWarn (const char* message) { /*Log::Msg(std::string("Assimp: ") + message, Log::Severity::Warn);*/ }
        virtual void OnError(const char* message) { Log::Msg(std::string("Assimp: ") + message, Log::Severity::Error); }
        bool attachStream (Assimp::LogStream *pStream, unsigned int severity) { return true; }
        bool detatchStream(Assimp::LogStream *pStream, unsigned int severity) { return true; }
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