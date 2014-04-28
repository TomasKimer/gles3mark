

#pragma once

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/quaternion.hpp>

#include <ostream>
#include "transform.h"

class Camera {
public:
    Camera() {
        Reset();
    }
    ~Camera() {}

    void Reset();
    void Move(glm::vec3 vec);    
    void Aim(float verticalAngle, float horizontalAngle);

    void LookAt(const glm::vec3& eye, const glm::vec3& target, const glm::vec3& up = glm::vec3(0.f, 1.f, 0.f)) {
        view = glm::lookAt(eye, target, up);
    }

    glm::mat4& GetViewMatrix()       { return view;       }
    glm::mat4& GetProjectionMatrix() { return projection; }
    
    glm::vec3& GetEye()    { return eye; }
    glm::vec3& GetTarget() { return target; }

    void Perspective(float fovy, float aspect, float zNear, float zFar);
    void Orthographic(float left, float right, float bottom, float top, float zNear, float zFar);

    //void SetPosition(const glm::vec3& pos) {
    //    eye = pos;
    //}
    //void SetDirection(const glm::vec3& dir) {
    //    target = dir;
    //}

    friend std::ostream& operator << (std::ostream& o, const Camera& c) {
        o << "pos "    << c.eye.x    << " " << c.eye.y    << " " << c.eye.z    << '\n'
          << "target " << c.target.x << " " << c.target.y << " " << c.target.z << '\n'
          << "up  "    << c.up.x     << " " << c.up.y     << " " << c.up.z;
        return o;
    }

private:
    glm::vec3 eye;          // camera position
    glm::vec3 target;       // view direction  -- ??
    glm::vec3 up;           // up vector

    float angleHoriz;       // ??
    float angleVert;


    glm::mat4 projection;
    glm::mat4 view;
    

    glm::vec3 position;
    glm::quat rotation;
    Transform transform;   // TODO, combine with lookAt?

    float nearClipPlane, farClipPlane;
    float aspect, fovY;
    glm::vec4 backgroundColor; // TODO Color class
    bool ortographic;
};