/**
  *  gles3mark v1.0
  *
  *  \date   2014-05-28
  *  \author Tomas Kimer <xkimer00@stud.fit.vutbr.cz>
  */

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// http://docs.unity3d.com/Documentation/ScriptReference/Light.html
// renderMode - vertex, pixel
// cookie - The cookie texture projected by the light. If the cookie is a cube map, the light will become a Point light. Note that cookies are only displayed for pixel lights.
class Light {
public:
    enum class Type {
        Directional,
        Point,
        Spot,
        Area  // it affects only lightmaps and lightprobes.
    } type;

    Light();
    Light(const glm::vec3& pos, const glm::vec3& diffuseColor, float size);

    void Move(const glm::vec3& vec);
    void UpdateMatrix();
        
    glm::vec3 position;      // light position for a point/spotlight
    glm::vec3 diffuseColor;
    float size;
    glm::mat4 matrix;
    
    
    // -- unused --
    glm::vec3 direction;     // normalized dir. for a directional light
    glm::vec3 ambientColor;
    glm::vec3 specularColor;
    
    //glm::vec3 spotDirection;
    //glm::vec3 attenuationFactors;
    //
    //float spotExponent;
    //float spotCutoffAngle;

    //bool castShadows;
};