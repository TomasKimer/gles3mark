

#pragma once

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/quaternion.hpp>

#include "log.h"

class Camera {
public:
    Camera() { Reset(); };
    ~Camera() {};

    void Reset() {
        up = glm::vec3(0.0f, 1.0f, 0.0f);
        eye = glm::vec3(0, 0, 0);
        target = glm::vec3(0.f, 0.f, 1.f);

        angleHoriz = 0.0f; // glm::half_pi<float>()
        angleVert = 0.0f;
    }

    void Move(glm::vec3 vec) {
        glm::vec3 dir = glm::normalize(target); // posunout o z-nasobek smeru pohledu
        eye += dir * vec.z;

        glm::vec3 r = glm::normalize(target); // posunout do strany o x-nasobek right vektoru
        r = glm::cross(-r, up);
        eye += r * vec.x;

        eye.y += vec.y; // pouze stoupani ci klesani
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

        //Log::V() << angleVert << " " << angleHoriz;

        float l_phi = f_pi_2 - angleHoriz; // uhly sferickych souradnic jsou velke a male fi
        float u_phi = f_pi_2 - angleVert;

        target = glm::vec3(sin(u_phi) * cos(l_phi),
                           cos(u_phi),
                           sin(u_phi) * sin(l_phi));
    }

    glm::mat4 GetViewMatrix() {
        return glm::lookAt(eye, target + eye, up);
    }

    glm::mat4& GetProjectionMatrix() {
        return projection;
    }

    void Perspective(float fovy, float aspect, float zNear, float zFar) {
        projection = glm::perspective(fovy, aspect, zNear, zFar);
    }

    void Orthographic(float left, float right, float bottom, float top, float zNear, float zFar) {
        projection = glm::ortho(left, right, bottom, top, zNear, zFar);
    }
    
    glm::vec3 GetEye()    { return eye; }
    glm::vec3 GetTarget() { return target; }

    void DebugDump() {
        Log::V("----------- Camera Debug Dump ------------------");
        Log::V() << "pos "    << eye.x    << " " << eye.y    << " " << eye.z;
        Log::V() << "target " << target.x << " " << target.y << " " << target.z;
        Log::V() << "up  "    << up.x     << " " << up.y     << " " << up.z;
    }

private:

    glm::vec3 eye;          // camera position
    glm::vec3 target;       // view direction  -- ??
    glm::vec3 up;           // up vector

    glm::fquat rotation;     // TODO transform class, combine with lookAt?

    glm::mat4 projection;

    float angleHoriz;       // ??
    float angleVert;


    // TODO
    float nearClipPlane, farClipPlane;
    float aspect, fovY;
    glm::vec4 backgroundColor; // TODO Color class
};