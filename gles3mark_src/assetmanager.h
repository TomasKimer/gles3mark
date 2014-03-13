/*
 * assetmanager.h
 */

#pragma once

#include <string>
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

	int ReadAsset(const std::string& fileName, int bytesToRead, void* buffer) {

        AssetFile* file = Open(fileName);
        
        int bytesRead = Read(file, bytesToRead, buffer);

        Close(file);

		return bytesRead;
	}



private:

    AssetFile* Open(const std::string& fileName) {
        AssetFile *file = nullptr;

#ifdef ANDROID
        if (ioContext) {
            AAssetManager* aAssetManager = (AAssetManager*)ioContext;
            file = AAssetManager_open(aAssetManager, fileName.c_str(), AASSET_MODE_BUFFER);
        } else {
            throw std::runtime_error("AAssetManager == NULL");
        }
#else
        file = fopen(("../assets/" + fileName).c_str(), "rb");
        if (file == nullptr)
            throw std::runtime_error("Cannot open file: " + fileName);
#endif

		return file;
	}


    static void Close(AssetFile* file) {
        if (file) {
#ifdef ANDROID
            AAsset_close(file);
#else
            fclose(file);
#endif
            file = nullptr;
        }
    }

    static int Read(AssetFile* file, size_t bytesToRead, void* buffer) {
        int bytesRead = 0;

        if (file == nullptr)
            return bytesRead;

#ifdef ANDROID
        bytesRead = AAsset_read(file, buffer, bytesToRead);
#else
        bytesRead = fread(buffer, 1, bytesToRead, file);  // TODO http://www.cplusplus.com/reference/cstdio/fread/
#endif

        return bytesRead;
    }

public:
    char* LoadTGA(const std::string& fileName, int *width, int *height) {
        // Open the file for reading
        AssetFile* fp = Open(fileName);

        if (fp == nullptr) {  // Error in opening the input file from apk
            throw std::runtime_error("LoadTGA FAILED: " + fileName);
            //return nullptr;
        }

        TGA_HEADER Header;
        int bytesRead = Read(fp, sizeof (TGA_HEADER), &Header);

        *width = Header.Width;
        *height = Header.Height;

        if (Header.ColorDepth == 8 || Header.ColorDepth == 24 || Header.ColorDepth == 32) {
            
            int bytesToRead = sizeof(char) * (*width) * (*height) * Header.ColorDepth / 8;            
            char* buffer = (char*)malloc(bytesToRead); // Allocate the image data buffer

            if (buffer) {
                bytesRead = Read(fp, bytesToRead, buffer);
                Close(fp);
                return buffer;
            }
        }

        return nullptr;
    }
};

