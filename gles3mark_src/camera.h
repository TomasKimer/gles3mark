

#pragma once

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>

#include "log.h"

class Camera {
public:
    Camera() { Reset(); };
    ~Camera() {};

    void Reset() {
        up = glm::vec3(0.0f, 1.0f, 0.0f);
        eye = glm::vec3(0, 0, 0);
        target = glm::vec3(0.f, 0.f, 1.f);

        angleHoriz = glm::half_pi<float>(); //0.0f
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

        float l_phi = f_pi_2 - angleHoriz; // uhly sferickych souradnic jsou velke a male fi
        float u_phi = f_pi_2 - angleVert;

        target = glm::vec3(sin(u_phi) * cos(l_phi),
                           cos(u_phi),
                           sin(u_phi) * sin(l_phi));
    }

    glm::mat4 GetMatrix() {
        return glm::lookAt(eye, target + eye, up);
    }
    
    glm::vec3 GetEye()    { return eye; }
    glm::vec3 GetTarget() { return target; }

    void DebugDump() {
        Log::Msg("----------- Camera Debug Dump ------------------");
        Log::Stream() << "pos "    << eye.x    << " " << eye.y    << " " << eye.z;
        Log::Stream() << "target " << target.x << " " << target.y << " " << target.z;
        Log::Stream() << "up  "    << up.x     << " " << up.y     << " " << up.z;
    }

private:

    glm::vec3 eye;          // camera position
    glm::vec3 target;       // view direction
    glm::vec3 up;           // up vector

    float angleHoriz;
    float angleVert;
};