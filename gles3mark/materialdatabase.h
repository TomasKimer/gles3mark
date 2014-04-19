

#pragma once

#include <vector>
#include <string>

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

    Material* Get(unsigned id) {
        return database[id];
    }
};