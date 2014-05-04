

#pragma once

#include <vector>
#include <string>
#include <memory>

#include "material.h"
#include "assetmanager.h"

// TODO texture database + index(=path)? map<path,Texture> ?
// but what about texture settings - only texture data (from file) database? - not in memory after load, Sampler objects?
// resource manager? - ShaderDatabase, ...

class MaterialDatabase {

    std::vector<Material*> database;

public:
    MaterialDatabase() {}
    ~MaterialDatabase() {}

    void Add(Material* m) {
        database.push_back(m);
    }

    Material* Get(unsigned id) const {
        return database[id];
    }

    unsigned MaterialCount() const {
        return database.size();
    }

    // TODO check for duplicite textures
    void LoadTextures(std::unique_ptr<AssetManager>& assetManager) {
        for (Material* m : database) {
            if (m->hasTexture) {
                std::string ktxPath = m->texture->path.substr(0, m->texture->path.find_last_of(".")) + ".ktx";
                std::transform(ktxPath.begin(), ktxPath.end(), ktxPath.begin(), ::tolower);
                m->texture->FromKTXdata(assetManager->LoadContents("textures/" + ktxPath)); //test/rgb-reference.ktx
            }            
        }
    }
};