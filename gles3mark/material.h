
#pragma once

#include <string>
#include <glm/glm.hpp>

#include "gltexture.h"

// http://docs.unity3d.com/Documentation/ScriptReference/Material.html
class Material {
public:
    glm::vec4 diffuseColor;
    glm::vec4 ambientColor;
    glm::vec4 specularColor;
    glm::vec4 emissiveColor;
    // glm::vec4 reflectColor
    float shininess; // specularExponent: describes the shininess of the material and is used to control the shininess of the specular highlight

    // Shader program? http://docs.unity3d.com/Documentation/ScriptReference/Shader.html
    // Textures?
    bool hasTexture;
    Texture *texture;

public:
    Material(const glm::vec4& diffuse, const glm::vec4& ambient, const glm::vec4& specular,
             const glm::vec4& emissive, float shininess, const std::string& texturePath = std::string()) :
             diffuseColor(diffuse), ambientColor(ambient), specularColor(specular),
             emissiveColor(emissive), shininess(shininess), texture(nullptr)
    {
        hasTexture = !texturePath.empty();
    
    
    
    }


};