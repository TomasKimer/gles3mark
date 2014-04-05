/*
 * assetmanager.cpp
 */

#include "assetmanager.h"
#include "log.h"

std::vector<char> AssetManager::LoadContents(const std::string& fileName) const {
    AssetFile file;
    file.Open(fileName);

    int len = file.Length();
    std::vector<char> buff(len);
    file.Read(len, &buff[0]);
    file.Close();

    return buff;
}

std::string AssetManager::LoadText(const std::string& fileName) const {
    std::vector<char> contents = LoadContents(fileName);
    return std::string(&contents[0], contents.size());
}

int AssetManager::ReadAsset(const std::string& fileName, int bytesToRead, void* buffer) const {
    AssetFile file;
    file.Open(fileName);
    int bytesRead = file.Read(bytesToRead, buffer);
    file.Close();
    return bytesRead;
}

char* AssetManager::LoadTGA(const std::string& fileName, int *width, int *height) const {
    AssetFile fp;
    fp.Open(fileName);

    TGA_HEADER Header;
    int bytesRead = fp.Read(sizeof (TGA_HEADER), &Header);

    *width = Header.Width;
    *height = Header.Height;

    Log::V() << "TGA color depth: " << (int)Header.ColorDepth;
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