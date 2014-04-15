
#pragma once

#include <cstring>
#include "log.h"
#include "assetmanager.h"
#include "model.h"


class ModelImporter {
public:
    ModelImporter(/*const AssetManager& assetManager*/) /*: refAssetManager(assetManager)*/ {}
    virtual ~ModelImporter() = default;

    virtual Model* Import(const std::vector<char>& rawModelData) = 0;

protected:
    /*const AssetManager& refAssetManager;*/
};
