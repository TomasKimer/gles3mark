/**
  *  gles3mark v1.0
  *
  *  \date   2014-05-28
  *  \author Tomas Kimer <xkimer00@stud.fit.vutbr.cz>
  */

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

    std::vector<std::unique_ptr<Material>> database;

public:
    MaterialDatabase() {}
    ~MaterialDatabase() {}

    void Add(Material* m);

    const Material& Get(unsigned id) const;

    unsigned MaterialCount() const;

    // TODO check for duplicite textures
    void LoadTextures(const AssetManager& assetManager);
};