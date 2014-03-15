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

//	int ReadAsset(const std::string& fileName, int bytesToRead, void* buffer) {
//
//        AssetFile* file = Open(fileName);
//
//        int bytesRead = Read(file, bytesToRead, buffer);
//
//        Close(file);
//
//		return bytesRead;
//	}

    std::string LoadText(const std::string& fileName) {
        AssetFile* file = Open(fileName);

        int len = Length(file);
        std::vector<char> buff(len);
        Read(file, len, &buff[0]);
        Close(file);

        return std::string(&buff[0], buff.size());
    }

    // Load whole file and return it as std::string
    //std::string LoadText(const std::string& fileName) {
    //    std::ifstream stream(fileName);
    //    if (stream.fail()) throw std::runtime_error("Can't open \'" + fileName + "\'");
    //    return std::string(std::istream_iterator<char>(stream >> std::noskipws), std::istream_iterator<char>());
    //}

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
        file = std::fopen(("../assets/" + fileName).c_str(), "rb");
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
            std::fclose(file);
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
        bytesRead = std::fread(buffer, 1, bytesToRead, file);  // TODO http://www.cplusplus.com/reference/cstdio/fread/
#endif

        return bytesRead;
    }

    static int Length(AssetFile* file) {
#ifdef ANDROID
    	return AAsset_getLength(file);
#else
        std::fseek(file, 0, SEEK_END); // seek to end of file
        int len = std::ftell(file); // get current file pointer
        std::fseek(file, 0, SEEK_SET);
        return len;
#endif
    }

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

