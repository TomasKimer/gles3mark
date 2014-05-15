
#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "log.h"

struct KeyFrame {
    glm::vec3 position;  //, direction
    glm::quat rotation;
    float time;

    KeyFrame(const glm::vec3& pos, float time, const glm::quat& rot = glm::quat()):
        position(pos), time(time), rotation(rot) {}
};

class KeyFrameAnimation {

    std::vector<KeyFrame> keyFrames;
    unsigned currentFrame;
    float currentTime;
    
    unsigned repeatCount, currentRepeat;

    glm::vec3 currentPos;
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
    glm::quat& GetCurrentRotation()  { return currentRot; }    

    void MakeOrbit(float segments, float timestep, float radius);
};