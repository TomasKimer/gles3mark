/**
  *  gles3mark v1.0
  *
  *  \date   2014-05-28
  *  \author Tomas Kimer <xkimer00@stud.fit.vutbr.cz>
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

// Load whole file and return it as std::string
/*
std::string AssetManager LoadText(const std::string& fileName) {
    std::ifstream stream(fileName);
    if (stream.fail()) throw std::runtime_error("Can't open \'" + fileName + "\'");
    return std::string(std::istream_iterator<char>(stream >> std::noskipws), std::istream_iterator<char>());
}
*/