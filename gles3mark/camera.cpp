

#include "camera.h"

void Camera::Reset() {
    up = glm::vec3(0.0f, 1.0f, 0.0f);
    eye = glm::vec3(0, 0, 0);
    target = glm::vec3(0.f, 0.f, 1.f);

    angleHoriz = 0.0f; // glm::half_pi<float>()
    angleVert = 0.0f;

    LookAt(eye, target + eye);
}

void Camera::Move(glm::vec3 vec) {
    glm::vec3 dir = glm::normalize(target); // posunout o z-nasobek smeru pohledu
    eye += dir * vec.z;

    glm::vec3 r = glm::normalize(target); // posunout do strany o x-nasobek right vektoru
    r = glm::cross(-r, up);
    eye += r * vec.x;

    eye.y += vec.y; // pouze stoupani ci klesani

    LookAt(eye, target + eye);
}

void Camera::Aim(float verticalAngle, float horizontalAngle) {
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

void Camera::Perspective(float fovy, float aspect, float zNear, float zFar) {
    ortographic = false;
    nearClipPlane = zNear;
    farClipPlane = zFar;
    fovY = fovy;
    this->aspect = aspect;        
    
    projection = glm::perspective(fovy, aspect, zNear, zFar);
}

void Camera::Orthographic(float left, float right, float bottom, float top, float zNear, float zFar) {
    ortographic = true;
    nearClipPlane = zNear;
    farClipPlane = zFar;
    
    projection = glm::ortho(left, right, bottom, top, zNear, zFar);
}