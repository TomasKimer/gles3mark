

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/quaternion.hpp>


class Transform {

public:
    // The coordinate space in which to operate.
    enum class Space {
        World,  // Applies transformation relative to the world coordinate system.
        Self,   // Applies transformation relative to the local coordinate system.
    };

    Transform(): 
        up     (Up     ()),
        forward(Forward()),
        right  (Right  ())
    {
    
    }

    static glm::vec3 Up()      { return glm::vec3(0, 1, 0); }
    static glm::vec3 Forward() { return glm::vec3(0, 0, 1); }
    static glm::vec3 Right()   { return glm::vec3(1, 0, 1); }


    // The rotation as Euler angles in degrees.
    glm::vec3 eulerAngles;
    
    // The blue, red and green axis of the transform in world space.
    glm::vec3 forward, right, up;
    
    // The position of the transform in world space.
    glm::vec3 position;
    
    // The rotation of the transform in world space stored as a Quaternion.
    glm::quat rotation;

    
    glm::mat4 matrix;


    // Rotates the transform so the forward vector points at target's current position.
    void LookAt(Transform target, glm::vec3 worldUp = Up()) {}
    
    // Rotates the transform so the forward vector points at worldPosition.
    void LookAt(glm::vec3 worldPosition, glm::vec3 worldUp = Up()) {}
    
    // Applies a rotation of z degrees around the z axis, x degrees around the x axis, and y degrees around the y axis (in that order).
    void Rotate(glm::vec3 eulerAngles, Space relativeTo = Space::Self) {}
    
    // Rotates the transform around axis by angle degrees.
    void Rotate(glm::vec3 axis, float angle, Space relativeTo = Space::Self) {
        if (relativeTo == Space::Self)
            rotation = glm::rotate(rotation, angle, axis);
        else
            rotation = glm::rotate(glm::quat(), angle, axis);
    }


    // Moves the transform in the direction and distance of translation.
    void Translate(glm::vec3 translation, Space relativeTo = Space::Self) {
        position += translation;    
    }

    // Moves the transform in the direction and distance of translation.
    // The movement is applied relative to relativeTo's local coordinate system. If relativeTo is null, the movement is applied relative to the world coordinate system.
    void Translate(glm::vec3 translation, Transform relativeTo) {}

    
    glm::mat4 GetMatrix() {
        matrix = glm::mat4_cast(rotation);

        glm::mat4 translation(1);
        translation[3].x = -position.x;
        translation[3].y = -position.y;
        translation[3].z = -position.z;

        matrix *= translation;

        return matrix;
    }


    // friend std::ostream& operator << (std::ostream& o, const Transform& v) {  ??
};