/**
  *  gles3mark v1.0
  *
  *  \date   2014-05-28
  *  \author Tomas Kimer <xkimer00@stud.fit.vutbr.cz>
  */

#include "keyframeanimation.h"

void KeyFrameAnimation::AddKeyFrame(const KeyFrame& keyFrame) {      
    keyFrames.push_back(keyFrame);
}

unsigned KeyFrameAnimation::GetCurrentKeyFrame(unsigned lastFrame) {
    for (unsigned i = lastFrame + 1; i < keyFrames.size(); ++i) {
        if (currentTime <= keyFrames[i].time) {
            return i - 1;
        }
    }
    return keyFrames.size() - 1;
}

float KeyFrameAnimation::GetTransition(const KeyFrame& first, const KeyFrame& second) {
    return (currentTime - first.time) / (second.time - first.time); // glm::abs
}

void KeyFrameAnimation::Update(float deltaTime) {
    if (HasEnded()) return;

    currentTime += deltaTime;

    currentFrame = GetCurrentKeyFrame(currentFrame);    

    if (currentFrame == keyFrames.size() - 1) {
        currentRepeat++;
        if (HasEnded()) {
            currentFrame--; // step to the last frame (vs return)
        }
        else {
            currentTime = std::fmod(currentTime, keyFrames[currentFrame].time); // or = 0 for animation reset on repeat
            currentFrame = GetCurrentKeyFrame(0);
        }
    }

    KeyFrame& first  = keyFrames[currentFrame    ];
    KeyFrame& second = keyFrames[currentFrame + 1];
    
    float amount = GetTransition(first, second);

    DoLerp(first, second, amount);
}

bool KeyFrameAnimation::HasEnded() {
    return (repeatCount != 0 && currentRepeat >= repeatCount);
}

void KeyFrameAnimation::DoLerp(const KeyFrame& first, const KeyFrame& second, float amount) {
    currentPos = glm::mix  (first.position , second.position , amount);
    //currentRot = glm::slerp(first.rotation , second.rotation , amount);  // rotation is not used
}

void KeyFrameAnimation::MakeOrbit(float segments, float timestep, float radius) {
    float start =     -glm::pi<float>() / 2.0f;
    float end   =  2 * glm::pi<float>() - glm::pi<float>() / 2.f;
    float step  = (2 * glm::pi<float>() / segments);
    
    int j = 0;
    for (float f = start; f < end; f += step) {
        float x = radius * glm::cos(f);
        float z = radius * glm::sin(f);
        
        AddKeyFrame(KeyFrame(glm::vec3(x, 25.0f + 2.0f * glm::sin(f), z), j * timestep));        

        j++;
    }

    KeyFrame final = keyFrames[0];
    final.time = keyFrames[KeyFrameCount() - 1].time + timestep;
    AddKeyFrame(final);

    // timestep = 3.0f;
    //AddKeyFrame(KeyFrame(glm::vec3(  0, 20, -50), 0*timestep));
    //AddKeyFrame(KeyFrame(glm::vec3( 50, 20,   0), 1*timestep));
    //AddKeyFrame(KeyFrame(glm::vec3(  0, 20,  50), 2*timestep));
    //AddKeyFrame(KeyFrame(glm::vec3(-50, 20,   0), 3*timestep));
    //AddKeyFrame(KeyFrame(glm::vec3(  0, 20, -50), 4*timestep));
}