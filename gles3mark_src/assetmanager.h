/*
 * assetmanager.h
 */

#pragma once

#include <string>
#include <vector>
#include <stdexcept>

#ifdef ANDROID
#include <android/asset_manager.h>
typedef AAsset AssetFile;
#else
#include <cstdio>
typedef FILE AssetFile;
#endif

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

	void* ioContext;

public:
	AssetManager(void* _ioContext): ioContext(_ioContext) {	}
	virtual ~AssetManager() {}

    std::string LoadText(const std::string& fileName);
    int ReadAsset(const std::string& fileName, int bytesToRead, void* buffer);

    AssetFile* Open(const std::string& fileName);
    static void Close(AssetFile* file);
    static int Read(AssetFile* file, size_t bytesToRead, void* buffer);
    static int Length(AssetFile* file);

    char* LoadTGA(const std::string& fileName, int *width, int *height);
};

