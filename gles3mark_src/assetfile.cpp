

#include "assetfile.h"
#include <stdexcept>

void* AssetFile::ioContext = nullptr;

void AssetFile::Open(const std::string& fileName) {

#ifdef ANDROID
    if (ioContext) {
        AAssetManager* aAssetManager = (AAssetManager*)ioContext;
        file = AAssetManager_open(aAssetManager, fileName.c_str(), AASSET_MODE_BUFFER);
    }
    else {
        throw std::runtime_error("AssetFile ioContext not set");
    }
#else
    file = std::fopen(("../assets/" + fileName).c_str(), "rb");
    if (file == nullptr)
        throw std::runtime_error("Cannot open file: " + fileName);
#endif
}

void AssetFile::Close() {
    if (file) {
#ifdef ANDROID
        AAsset_close(file);
#else
        std::fclose(file);
#endif
        file = nullptr;
    }
}

int AssetFile::Read(size_t bytesToRead, void* buffer) {
    int bytesRead = 0;

    if (file == nullptr)
        return bytesRead;

#ifdef ANDROID
    bytesRead = AAsset_read(file, buffer, bytesToRead);
#else
    bytesRead = std::fread(buffer, 1, bytesToRead, file);  // TODO http://www.cplusplus.com/reference/cstdio/fread/
#endif

    return bytesRead;
}

int AssetFile::Length() {
#ifdef ANDROID
    return AAsset_getLength(file);
#else
    std::fseek(file, 0, SEEK_END);
    int len = std::ftell(file);
    std::fseek(file, 0, SEEK_SET);
    return len;
#endif
}
