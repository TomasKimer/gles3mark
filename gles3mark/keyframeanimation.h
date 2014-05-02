

#pragma once

#include <vector>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "log.h"

struct KeyFrame {
    glm::vec3 position;
    glm::vec3 direction;
    glm::quat rotation;
    float time;

    KeyFrame(const glm::vec3& pos, const glm::vec3& dir, float time, const glm::quat& rot = glm::quat()):
        position(pos), direction(dir), time(time), rotation(rot) {}
};


class KeyFrameAnimation {

    
    unsigned currentFrame, lastFrame;
    float currentTime, duration, transition;
    
    bool repeat, ended;
    unsigned repeatCount;

    glm::vec3 currentPos, currentDir;
    glm::quat currentRot;

    void DoLerp(const KeyFrame& first, const KeyFrame& second, float amount) {
        currentPos = glm::mix  (first.position , second.position , amount);
        currentDir = glm::mix  (first.direction, second.direction, amount);
        currentRot = glm::slerp(first.rotation , second.rotation , amount);    
    }

public:
    std::vector<KeyFrame> keyFrames;

    KeyFrameAnimation(float timeOffset = 0.0f) : currentFrame(0), lastFrame(0), currentTime(0), repeat(false), ended(false), repeatCount(0), transition(0) {
    }

    void AddKeyFrame(const KeyFrame& keyFrame) {      
        keyFrames.push_back(keyFrame);
    }

    
    unsigned GetCurrentKeyFrame(unsigned lastFrame);
    float GetAmount(const KeyFrame& first, const KeyFrame& second);
    void Update(float deltaTime);



    glm::vec3& GetCurrentPosition() {
        return currentPos;
    }

    glm::vec3& GetCurrentDirection() {
        return currentDir;
    }

    glm::quat& GetCurrentRotation() {
        return currentRot;
    }

    glm::mat4 GetCurrentMatrix() {
        return glm::mat4();
    }


    bool HasEnded() {
        return ended;
    }
};