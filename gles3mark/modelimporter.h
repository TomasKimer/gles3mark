
#pragma once

#include <cstring>
#include "log.h"
#include "assetmanager.h"
#include "model.h"
#include "material.h"


class ModelImporter {
public:
    ModelImporter(/*const AssetManager& assetManager*/) /*: refAssetManager(assetManager)*/ {}
    virtual ~ModelImporter() = default;

    virtual Model* Import(/*const*/ std::vector<char>& rawModelData, std::vector<Material*>& materialDatabase) = 0;

protected:
    /*const AssetManager& refAssetManager;*/
};
