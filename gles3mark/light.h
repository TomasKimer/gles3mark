

#pragma once

#include <glm/glm.hpp>

// http://docs.unity3d.com/Documentation/ScriptReference/Light.html
class Light {

    enum class Type {
        Directional,
        Point,
        Spot,
        Area  // it affects only lightmaps and lightprobes.
    } type;
    
    
    glm::vec4 position;      // light position for a point/spotlight
    glm::vec3 direction;     // normalized dir. for a directional light
    
    glm::vec4 ambientColor;
    glm::vec4 diffuseColor;
    glm::vec4 specularColor;
    
    glm::vec3 spotDirection;
    glm::vec3 attenuationFactors;
    
    float spotExponent;
    float spotCutoffAngle;

    bool castShadows;

    // renderMode - vertex, pixel
    // cookie - The cookie texture projected by the light. If the cookie is a cube map, the light will become a Point light. Note that cookies are only displayed for pixel lights.

public:
    Light(): type(Type::Directional) {}
};