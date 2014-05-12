
#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

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

    std::vector<KeyFrame> keyFrames;
    unsigned currentFrame;
    float currentTime;
    
    unsigned repeatCount, currentRepeat;

    glm::vec3 currentPos, currentDir;
    glm::quat currentRot;

    void DoLerp(const KeyFrame& first, const KeyFrame& second, float amount);
    float GetTransition(const KeyFrame& first, const KeyFrame& second);
    unsigned GetCurrentKeyFrame(unsigned lastFrame);

public:
    KeyFrameAnimation(unsigned repeatCount = 1)
        : currentFrame(0), currentTime(0), repeatCount(repeatCount), currentRepeat(0) {
    }

    void AddKeyFrame(const KeyFrame& keyFrame);
    const KeyFrame& GetKeyFrame(unsigned i) const { return keyFrames[i];     }
    unsigned KeyFrameCount()                const { return keyFrames.size(); }

    void Update(float deltaTime);
    bool HasEnded();

    glm::vec3& GetCurrentPosition()  { return currentPos; }
    glm::vec3& GetCurrentDirection() { return currentDir; }
    glm::quat& GetCurrentRotation()  { return currentRot; }    

    void MakeOrbit(float segments, float timestep, float radius, const glm::vec3 target);
};