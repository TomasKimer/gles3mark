/**
  *  gles3mark v1.0
  *
  *  \date   2014-05-28
  *  \author Tomas Kimer <xkimer00@stud.fit.vutbr.cz>
  */

#include "material.h"

Material::Material(const glm::vec4& diffuse, const glm::vec4& ambient, const glm::vec4& specular,
         const glm::vec4& emissive, float shininess, const std::string& texturePath, const std::string& name) :
         diffuseColor(diffuse), ambientColor(ambient), specularColor(specular),
         emissiveColor(emissive), shininess(shininess), texture(nullptr), name(name)
{
    hasTexture = !texturePath.empty();

    if (hasTexture) {
        texture = new Texture(texturePath);
    }
}