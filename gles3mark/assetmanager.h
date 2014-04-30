/*
 * assetmanager.h
 */

#pragma once

#include <string>
#include <vector>
#include <stdexcept>

#include "assetfile.h"


class AssetManager {

public:
    AssetManager(void* _ioContext = nullptr) { AssetFile::SetContext(_ioContext); }
    virtual ~AssetManager() { }

    std::vector<char> LoadContents(const std::string& fileName) const;
    std::string LoadText(const std::string& fileName) const;
    int ReadAsset(const std::string& fileName, int bytesToRead, void* buffer) const;
};
