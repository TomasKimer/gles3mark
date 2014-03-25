

#pragma once

#include <string>

#ifdef ANDROID
#include <android/asset_manager.h>
#else
#include <cstdio>
#endif

class AssetFile {
#ifdef ANDROID
    typedef AAsset OSFile;
#else
    typedef std::FILE OSFile;   // std::fstream?
#endif

    OSFile* file;
    static void* ioContext;

public:
    AssetFile() : file(nullptr) {}
    static void SetContext(void* _ioContext) { ioContext = _ioContext; }

    void Open(const std::string& fileName);
    void Close();
    int Read(size_t bytesToRead, void* buffer);
    int Length();
};
