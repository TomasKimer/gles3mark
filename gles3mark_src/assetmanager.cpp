/*
 * assetmanager.cpp
 */

#include "assetmanager.h"

void *AssetFile::ioContext = nullptr;

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
    std::fseek(file, 0, SEEK_END); // seek to end of file
    int len = std::ftell(file); // get current file pointer
    std::fseek(file, 0, SEEK_SET);
    return len;
#endif
}


std::vector<char> AssetManager::LoadContents(const std::string& fileName) {
    AssetFile file;
    file.Open(fileName);

    int len = file.Length();
    std::vector<char> buff(len);
    file.Read(len, &buff[0]);
    file.Close();

    return buff;
}

std::string AssetManager::LoadText(const std::string& fileName) {
    std::vector<char> contents = LoadContents(fileName);
    return std::string(&contents[0], contents.size());
}

int AssetManager::ReadAsset(const std::string& fileName, int bytesToRead, void* buffer) {
    AssetFile file;
    file.Open(fileName);
    int bytesRead = file.Read(bytesToRead, buffer);
    file.Close();
    return bytesRead;
}

char* AssetManager::LoadTGA(const std::string& fileName, int *width, int *height) {
    AssetFile fp;
    fp.Open(fileName);

    TGA_HEADER Header;
    int bytesRead = fp.Read(sizeof (TGA_HEADER), &Header);

    *width = Header.Width;
    *height = Header.Height;

    if (Header.ColorDepth != 8 && Header.ColorDepth != 24 && Header.ColorDepth != 32) {
        fp.Close();
        throw std::runtime_error("Unsupported TGA color depth");
    }

    int bytesToRead = sizeof(char)* Header.Width * Header.Height * Header.ColorDepth / 8;
    char* buffer = new char[bytesToRead]; // Allocate the image data buffer

    bytesRead = fp.Read(bytesToRead, buffer);

    if (bytesRead != bytesToRead)
        throw std::runtime_error("Incomplete TGA image data");

    fp.Close();

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