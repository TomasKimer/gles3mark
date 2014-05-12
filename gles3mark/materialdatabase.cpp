
#include <algorithm>

#include "materialdatabase.h"

void MaterialDatabase::Add(Material* m) {
    database.push_back(std::unique_ptr<Material>(m));
}

const Material& MaterialDatabase::Get(unsigned id) const {
    return *database[id];
}

unsigned MaterialDatabase::MaterialCount() const {
    return database.size();
}

// TODO check for duplicite textures
void MaterialDatabase::LoadTextures(const AssetManager& assetManager) {
    for (std::unique_ptr<Material>& m : database) {
        if (m->hasTexture) {
            std::string ktxPath = m->texture->path.substr(0, m->texture->path.find_last_of(".")) + ".ktx";
            std::transform(ktxPath.begin(), ktxPath.end(), ktxPath.begin(), ::tolower);
            m->texture->FromKTXdata(assetManager.LoadContents("textures/" + ktxPath)); //test/rgb-reference.ktx
        }            
    }
}