
#include <cmath>
#include "keyframeanimation.h"

unsigned KeyFrameAnimation::GetCurrentKeyFrame(unsigned lastFrame) {
    for (unsigned i = lastFrame+1; i < keyFrames.size(); ++i) {
        if (currentTime <= keyFrames[i].time)
            return i-1;        
    }
    return keyFrames.size() - 1;
}

float KeyFrameAnimation::GetAmount(const KeyFrame& first, const KeyFrame& second) {
    return (currentTime - first.time) / (second.time - first.time); // glm::abs
}


void KeyFrameAnimation::Update(float deltaTime) {
    if (ended) return;

    currentTime += deltaTime;

    currentFrame = GetCurrentKeyFrame(currentFrame);    

    if (currentFrame == keyFrames.size() - 1) {
        if (!repeat) {
            ended = true;
            currentFrame--; // step to the last frame // vs return
        }
        else {
            currentTime = std::fmod(currentTime, keyFrames[currentFrame].time); // or = 0 for animation reset on repeat
            currentFrame = GetCurrentKeyFrame(0);
        }
    }

    KeyFrame& first  = keyFrames[currentFrame    ];
    KeyFrame& second = keyFrames[currentFrame + 1];
    
    float amount = GetAmount(first, second);

    DoLerp(first, second, amount);
    

    //if (transition == 0.0f && keyFrames.size() > currentFrame + 1) {
    //    duration = keyFrames[currentFrame + 1].time - keyFrames[currentFrame].time;        
    //}

    //transition += deltaTime;
    //float percent = transition / duration;
    //DoLerp(keyFrames[currentFrame], keyFrames[currentFrame + 1], percent);

    //if (percent >= 1.0f) {
    //    transition = 0.0f;
    //    currentFrame++;
    //    if (currentFrame >= keyFrames.size() - 1) {            
    //        if (!repeat)
    //            ended = true;
    //        else {
    //            currentFrame = 0;
    //        }                
    //    }
    //}
}