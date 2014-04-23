

#pragma once

#include <vector>

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

    std::vector<KeyFrame> keyFrames;
    unsigned currentFrame;
    float /*startTime, endTime,*/ duration, transition;
    
    bool repeat, ended;
    unsigned repeatCount;

    glm::vec3 currentPos, currentDir;
    glm::quat currentRot;

public:
    KeyFrameAnimation(float timeOffset = 0.0f) : currentFrame(0), repeat(true), ended(false), repeatCount(0), transition(0) {
    
    }

    void AddKeyFrame(const KeyFrame& keyFrame) {      
        keyFrames.push_back(keyFrame);
    }

    void AddKeyFrame(const glm::vec3& pos, const glm::vec3& dir, float time, const glm::quat& rot = glm::quat()) {
        AddKeyFrame(KeyFrame(pos, dir, time, rot));    
    }

    void Update(float deltaTime) {
        if (ended) return;
        
        if (transition == 0.0f && keyFrames.size() > currentFrame + 1) {
            duration = keyFrames[currentFrame + 1].time - keyFrames[currentFrame].time;        
        }
        
        transition += deltaTime;

        float percent = transition / duration;
        
        currentPos = glm::mix(keyFrames[currentFrame].position, keyFrames[currentFrame + 1].position, percent);
        currentDir = glm::mix(keyFrames[currentFrame].direction, keyFrames[currentFrame + 1].direction, percent);
        currentRot = glm::slerp(keyFrames[currentFrame].rotation, keyFrames[currentFrame + 1].rotation, percent);

        if (percent >= 1.0f)
        {
            transition = 0.0f;
            currentFrame++;

            if (currentFrame >= keyFrames.size() - 1)            
            {
                if (!repeat)
                    ended = true;
                else
                    currentFrame = 0;
            }
        }
    }

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