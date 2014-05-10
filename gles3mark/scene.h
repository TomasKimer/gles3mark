
#pragma once

#include <memory>
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "assetmanager.h"
#include "model.h"
#include "light.h"
#include "camera.h"
#include "time.h"
#include "texture.h"
#include "shaderprogram.h"
#include "framebuffer.h"
#include "quadrenderer.h"
#include "keyframeanimation.h"
#include "materialdatabase.h"

class Scene {  // public Screen?
    friend class GLES3Mark;

    std::unique_ptr<ShaderProgram> shadowPassProgram, geometryPassProgram, ssaoPassProgram, lightPassProgram, screenQuadProgram; 
    Model *modelE112, *modelChairs, *modelDeskMid, *modelDeskSide;
    Mesh meshPointLight;
    QuadRenderer quadRenderer;
    MaterialDatabase materialDatabase;
    std::vector<std::unique_ptr<Light>> lightDatabase;
    
    Framebuffer framebuffer;//, framebufferSecond;
    Texture albedoTex, normalTex, depthTex, finalTex;

    KeyFrameAnimation cameraAnim;
    
    glm::ivec2 screenSize, renderSize;
    Camera camera;
    bool freeCamera;
    
public:    
    Scene(): renderSize(1280, 720), freeCamera(false), camera(60.0f, 16.0f/9.0f, 1.0f, 256.0f, glm::vec4(0.5f, 0.5f, 1.f, 1.f))  {}
    ~Scene() {}
        
    bool OnInit(const AssetManager& assetManager, int width, int height);
    void OnResize(int w, int h);
    bool OnStep(const Time& time);
    void Destroy();
};
