/**
  *  gles3mark v1.0
  *
  *  \date   2014-05-28
  *  \author Tomas Kimer <xkimer00@stud.fit.vutbr.cz>
  */

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>

#include "camera.h"

Camera::Camera(float fovy, float aspect, float zNear, float zFar, const glm::vec4 backgroundColor)
    : fovY(glm::radians(fovy)), aspect(aspect), nearClipPlane(zNear), farClipPlane(zFar), backgroundColor(backgroundColor), orthographic(false) {
    Reset();
    Perspective();
}

Camera::~Camera() {

}

void Camera::Reset() {
    up = glm::vec3(0.0f, 1.0f, 0.0f);
    eye = glm::vec3(0, 0, 0);
    target = glm::vec3(0.f, 10.f, 1.f);

    angleHoriz = 0.0f; // glm::half_pi<float>()
    angleVert = 0.0f;

    LookAt(eye, target + eye);
}

void Camera::Move(glm::vec3 vec) {
    glm::vec3 dir = glm::normalize(target); // z
    eye += dir * vec.z;

    glm::vec3 r = glm::normalize(target);   // x
    r = glm::cross(-r, up);
    eye += r * vec.x;

    eye.y += vec.y;                         // y

    LookAt(eye, target + eye);
}

void Camera::Aim(float verticalAngle, float horizontalAngle) {
    angleHoriz += horizontalAngle;    // unit sphere rotation
    angleVert  += verticalAngle;

    float halfPI = glm::half_pi<float>();

    // clamp + correction (target == up -> undefined lookAt)
    if (angleVert >=  halfPI) angleVert =  halfPI - 0.0001f;
    if (angleVert <= -halfPI) angleVert = -halfPI + 0.0001f;

    float hPhi = halfPI - angleHoriz; // spherical angles
    float vPhi = halfPI - angleVert;

    float hPhiSin = glm::sin(hPhi);
    float hPhiCos = glm::cos(hPhi);

    float vPhiSin = glm::sin(vPhi);
    float vPhiCos = glm::cos(vPhi);

    target = glm::vec3(vPhiSin * hPhiCos, vPhiCos, vPhiSin * hPhiSin);

    LookAt(eye, target + eye);
}

void Camera::LookAt(const glm::vec3& eye, const glm::vec3& target, const glm::vec3& up) {
    this->eye = eye;
    view = glm::lookAt(eye, target, up);
}

void Camera::Perspective(float fovy, float aspect, float zNear, float zFar) {
    orthographic = false;
    nearClipPlane = zNear;
    farClipPlane = zFar;
    fovY = fovy;
    this->aspect = aspect;        
    
    projection = glm::perspective(fovy, aspect, zNear, zFar);
}

void Camera::Perspective() {
    projection = glm::perspective(fovY, aspect, nearClipPlane, farClipPlane);
}

void Camera::Orthographic(float left, float right, float bottom, float top, float zNear, float zFar) {
    orthographic = true;
    nearClipPlane = zNear;
    farClipPlane = zFar;
    
    projection = glm::ortho(left, right, bottom, top, zNear, zFar);
}

void Camera::UpdateAspect(float aspect) {
    if (!orthographic) {
        this->aspect = aspect;
        Perspective();
    }   
}
