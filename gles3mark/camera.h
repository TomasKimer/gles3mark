
#pragma once

#include <ostream>

#include <glm/glm.hpp>
//#include <glm/gtc/quaternion.hpp>

class Camera {
public:
    Camera(float fovy, float aspect, float zNear, float zFar, const glm::vec4 backgroundColor = glm::vec4());
    ~Camera();

    void Reset();
    void Move(glm::vec3 vec);    
    void Aim(float verticalAngle, float horizontalAngle);

    void LookAt(const glm::vec3& eye, const glm::vec3& target, const glm::vec3& up = glm::vec3(0.f, 1.f, 0.f));

    void Perspective(float fovy, float aspect, float zNear, float zFar);
    void Perspective();
    void Orthographic(float left, float right, float bottom, float top, float zNear, float zFar);

    void UpdateAspect(float aspect);

    glm::mat4& GetViewMatrix()       { return view;       }
    glm::mat4& GetProjectionMatrix() { return projection; }
    
    glm::vec3& GetEye()    { return eye; }
    glm::vec3& GetTarget() { return target; }

    glm::vec4& GetBackgroundColor() { return backgroundColor; }

    friend std::ostream& operator << (std::ostream& o, const Camera& c) {
        o << "pos "    << c.eye.x    << " " << c.eye.y    << " " << c.eye.z    << '\n'
          << "target " << c.target.x << " " << c.target.y << " " << c.target.z << '\n'
          << "up  "    << c.up.x     << " " << c.up.y     << " " << c.up.z;
        return o;
    }

private:
    glm::vec3 eye;          // camera position
    glm::vec3 target;       // view direction
    glm::vec3 up;           // up vector

    float angleHoriz, angleVert;

    glm::mat4 projection;   // projection matrix
    glm::mat4 view;         // view matrix

    float nearClipPlane, farClipPlane;
    float aspect, fovY;
    bool orthographic;
    glm::vec4 backgroundColor;
};