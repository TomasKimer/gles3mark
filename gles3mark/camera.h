

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
    };

    ~Camera() {};


    void Reset() {
        up = glm::vec3(0.0f, 1.0f, 0.0f);
        eye = glm::vec3(0, 0, 0);
        target = glm::vec3(0.f, 0.f, 1.f);

        angleHoriz = 0.0f; // glm::half_pi<float>()
        angleVert = 0.0f;

        LookAt(eye, target + eye);
    }

    void Move(glm::vec3 vec) {
        glm::vec3 dir = glm::normalize(target); // posunout o z-nasobek smeru pohledu
        eye += dir * vec.z;

        glm::vec3 r = glm::normalize(target); // posunout do strany o x-nasobek right vektoru
        r = glm::cross(-r, up);
        eye += r * vec.x;

        eye.y += vec.y; // pouze stoupani ci klesani

        LookAt(eye, target + eye);
    }
    
    void Aim(float verticalAngle, float horizontalAngle) {
        angleHoriz += horizontalAngle; // kamera se bude otacet po jednotkove kouli
        angleVert  += verticalAngle;

        float f_pi_2 = glm::half_pi<float>();

        // osetrit pretaceni pres hlavu a pod nohama
        // pridat korekci, protoze pokud je target shodny s up vektorem, ma LookAt nedefinovane chovani
        if (angleVert >= f_pi_2)
            angleVert = f_pi_2 - 0.0001f;
        if (angleVert <= -f_pi_2)
            angleVert = -f_pi_2 + 0.0001f;

        float l_phi = f_pi_2 - angleHoriz; // uhly sferickych souradnic jsou velke a male fi
        float u_phi = f_pi_2 - angleVert;

        target = glm::vec3(sin(u_phi) * cos(l_phi),
                           cos(u_phi),
                           sin(u_phi) * sin(l_phi));

        LookAt(eye, target + eye);
    }

    glm::mat4& GetViewMatrix() {
        return view;
    }

    glm::mat4& GetProjectionMatrix() {
        return projection;
    }

    void Perspective(float fovy, float aspect, float zNear, float zFar) {
        ortographic = false;
        nearClipPlane = zNear;
        farClipPlane = zFar;
        fovY = fovy;
        this->aspect = aspect;        
        
        projection = glm::perspective(fovy, aspect, zNear, zFar);
    }

    void Orthographic(float left, float right, float bottom, float top, float zNear, float zFar) {
        ortographic = true;
        nearClipPlane = zNear;
        farClipPlane = zFar;
        
        projection = glm::ortho(left, right, bottom, top, zNear, zFar);
    }

    void LookAt(const glm::vec3& eye, const glm::vec3& target, const glm::vec3& up = glm::vec3(0.f, 1.f, 0.f)) {
        view = glm::lookAt(eye, target, up);
    }

    glm::vec3& GetEye()    { return eye; }
    glm::vec3& GetTarget() { return target; }

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