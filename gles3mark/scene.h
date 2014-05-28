/**
  *  gles3mark v1.0
  *
  *  \date   2014-05-28
  *  \author Tomas Kimer <xkimer00@stud.fit.vutbr.cz>
  */

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

class Scene {  // public Screen
    friend class GLES3Mark;

    std::unique_ptr<ShaderProgram> geometryPassProgram, ssaoPassProgram, lightPassProgram, screenQuadProgram; 
    Model *modelE112, *modelChairs, *modelDeskMid, *modelDeskSide;
    Mesh meshPointLight;
    QuadRenderer quadRenderer;
    MaterialDatabase materialDatabase;
    std::vector<std::unique_ptr<Light>> lightDatabase;
    
    Framebuffer framebuffer;
    Texture albedoTex, normalTex, depthTex, finalTex, ssaoTex;

    KeyFrameAnimation cameraAnim;
    
    glm::ivec2 screenSize, renderSize;
    Camera camera;
    bool freeCamera;
    
public:    
    Scene();
    ~Scene();
        
    bool OnInit(const AssetManager& assetManager, int width, int height);
    void OnResize(int w, int h);
    bool OnStep(const Time& time);
    void Destroy();
};
