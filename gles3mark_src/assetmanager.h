/*
 * assetmanager.h
 */

#pragma once

#include <string>
#include <vector>
#include <stdexcept>

#ifdef ANDROID
#include <android/asset_manager.h>
//typedef AAsset AssetFile;
#else
#include <cstdio>
//typedef FILE AssetFile;
#endif

class AssetFile {
#ifdef ANDROID
    AAsset* file;
#else
    std::FILE* file;
#endif
    static void* ioContext;

public:
    AssetFile(): file(nullptr) {}    
    static void SetContext(void* _ioContext) { ioContext = _ioContext; }

    void Open(const std::string& fileName);
    void Close();
    int Read(size_t bytesToRead, void* buffer);
    int Length();
};


class AssetManager {

#pragma pack(push,x1) // Byte alignment (8-bit)
#pragma pack(1)
    typedef struct {
        unsigned char IdSize, MapType, ImageType;
        unsigned short PaletteStart, PaletteSize;
        unsigned char PaletteEntryDepth;
        unsigned short X, Y, Width, Height;
        unsigned char ColorDepth, Descriptor;
    } TGA_HEADER;
#pragma pack(pop,x1)

public:
    AssetManager(void* _ioContext = nullptr) { AssetFile::SetContext(_ioContext); }
	virtual ~AssetManager() {}

    std::vector<char> LoadContents(const std::string& fileName);
    std::string LoadText(const std::string& fileName);
    int ReadAsset(const std::string& fileName, int bytesToRead, void* buffer);

    char* LoadTGA(const std::string& fileName, int *width, int *height);
};

