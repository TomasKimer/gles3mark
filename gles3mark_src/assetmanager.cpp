/*
 * assetmanager.cpp
 */

#include "assetmanager.h"

std::vector<char> AssetManager::LoadContents(const std::string& fileName) {
    AssetFile* file = Open(fileName);
    
    int len = Length(file);
    std::vector<char> buff(len);
    Read(file, len, &buff[0]);
    Close(file);

    return buff;
}

std::string AssetManager::LoadText(const std::string& fileName) {
    std::vector<char> contents = LoadContents(fileName);    
    return std::string(&contents[0], contents.size());
}

int AssetManager::ReadAsset(const std::string& fileName, int bytesToRead, void* buffer) {
    AssetFile* file = Open(fileName);
    int bytesRead = Read(file, bytesToRead, buffer);
    Close(file);
    return bytesRead;
}

AssetFile* AssetManager::Open(const std::string& fileName) {
    AssetFile *file = nullptr;

#ifdef ANDROID
    if (ioContext) {
        AAssetManager* aAssetManager = (AAssetManager*)ioContext;
        file = AAssetManager_open(aAssetManager, fileName.c_str(), AASSET_MODE_BUFFER);
    }
    else {
        throw std::runtime_error("AAssetManager == NULL");
    }
#else
    file = std::fopen(("../assets/" + fileName).c_str(), "rb");
    if (file == nullptr)
        throw std::runtime_error("Cannot open file: " + fileName);
#endif

    return file;
}

void AssetManager::Close(AssetFile* file) {
    if (file) {
#ifdef ANDROID
        AAsset_close(file);
#else
        std::fclose(file);
#endif
        file = nullptr;
    }
}

int AssetManager::Read(AssetFile* file, size_t bytesToRead, void* buffer) {
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

int AssetManager::Length(AssetFile* file) {
#ifdef ANDROID
    return AAsset_getLength(file);
#else
    std::fseek(file, 0, SEEK_END); // seek to end of file
    int len = std::ftell(file); // get current file pointer
    std::fseek(file, 0, SEEK_SET);
    return len;
#endif
}

char* AssetManager::LoadTGA(const std::string& fileName, int *width, int *height) {
    AssetFile* fp = Open(fileName);

    TGA_HEADER Header;
    int bytesRead = Read(fp, sizeof (TGA_HEADER), &Header);

    *width = Header.Width;
    *height = Header.Height;

    if (Header.ColorDepth != 8 && Header.ColorDepth != 24 && Header.ColorDepth != 32) {
        Close(fp);
        throw std::runtime_error("Unsupported TGA color depth");
    }

    int bytesToRead = sizeof(char)* Header.Width * Header.Height * Header.ColorDepth / 8;
    char* buffer = new char[bytesToRead]; // Allocate the image data buffer

    bytesRead = Read(fp, bytesToRead, buffer);

    if (bytesRead != bytesToRead)
        throw std::runtime_error("Incomplete TGA image data");

    Close(fp);

    return buffer;
}


// Load whole file and return it as std::string
/*
std::string AssetManager LoadText(const std::string& fileName) {
    std::ifstream stream(fileName);
    if (stream.fail()) throw std::runtime_error("Can't open \'" + fileName + "\'");
    return std::string(std::istream_iterator<char>(stream >> std::noskipws), std::istream_iterator<char>());
}
*/