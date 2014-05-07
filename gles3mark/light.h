

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// http://docs.unity3d.com/Documentation/ScriptReference/Light.html
struct Light {
    enum class Type {
        Directional,
        Point,
        Spot,
        Area  // it affects only lightmaps and lightprobes.
    } type;
    
    
    glm::vec3 position;      // light position for a point/spotlight
    glm::vec3 diffuseColor;
    float size;
    glm::mat4 matrix;
    
    
    // -- unused --
    glm::vec3 direction;     // normalized dir. for a directional light
    glm::vec3 ambientColor;
    glm::vec3 specularColor;
    
    glm::vec3 spotDirection;
    glm::vec3 attenuationFactors;
    
    float spotExponent;
    float spotCutoffAngle;

    bool castShadows;

    void Move(const glm::vec3& vec) {
        position += vec;
        UpdateMatrix();    
    }

    void UpdateMatrix() {
        matrix = glm::scale(glm::translate(glm::mat4(), position), glm::vec3(size));
    }

    // renderMode - vertex, pixel
    // cookie - The cookie texture projected by the light. If the cookie is a cube map, the light will become a Point light. Note that cookies are only displayed for pixel lights.

    Light() : type(Type::Directional) { }    
    Light(const glm::vec3& pos, const glm::vec3& diffuseColor, float size) : position(pos), diffuseColor(diffuseColor), size(size), type(Type::Point) {
        UpdateMatrix();        
    }
};