/**
  *  gles3mark v1.0
  *
  *  \date   2014-05-28
  *  \author Tomas Kimer <xkimer00@stud.fit.vutbr.cz>
  */

#pragma once

#include <string>
#include <glm/glm.hpp>

#include "texture.h"

// http://docs.unity3d.com/Documentation/ScriptReference/Material.html
// http://assimp.sourceforge.net/lib_html/materials.html
/**
 * \brief Material.
 */
class Material {
public:
    glm::vec4 diffuseColor;
    glm::vec4 ambientColor;
    glm::vec4 specularColor;
    glm::vec4 emissiveColor;
    // glm::vec4 reflectColor
    float shininess; // specularExponent: describes the shininess of the material and is used to control the shininess of the specular highlight

    // Shader program? http://docs.unity3d.com/Documentation/ScriptReference/Shader.html
    // Textures? texture database + index? map<path,Texture> ?
    bool hasTexture;
    Texture *texture;      // The material's texture. - main, normal, ..., another material can have same texture - TextureDatabase - index: path?, sharedPtr?
    //ShaderProgram *shader; //The shader used by the material. - only one,            - || -             shader  - ShaderDatabase  - index: -||-?

    unsigned id;
    std::string name;

public:
    Material(const glm::vec4& diffuse, const glm::vec4& ambient = glm::vec4(0), const glm::vec4& specular = glm::vec4(0),
             const glm::vec4& emissive = glm::vec4(0), float shininess = 0.0f, const std::string& texturePath = std::string(), const std::string& name = std::string());
};