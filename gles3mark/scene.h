

#pragma once

#include "log.h"
#include "assetmanager.h"
#include "sceneimporter.h"
#include "camera.h"
#include "glerror.h"
#include "time.h"
#include "texture.h"
#include "meshrenderer.h"
#include "shaderprogram.h"
#include "glquery.h"
#include "framebuffer.h"
#include "quadrenderer.h"
#include "keyframeanimation.h"
#include "materialdatabase.h"
#include "ssaobuilder.h"

#include <memory>
#include <string>
#include <vector>
#include <cassert>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>


class Scene {  // public Screen?
    friend class GLES3Mark;

    // layout (std140) uniform MatrixUniformBlock
    //struct MatrixUniformBlock {
    //    glm::mat4 projection;
    //    glm::mat4 view;
    //    glm::mat4 model;
    //} mub;
    //GLuint matrixUniformBuffer;
    //glBindBuffer(GL_UNIFORM_BUFFER, matrixUniformBuffer);
    //glBufferSubData(GL_UNIFORM_BUFFER, offsetof(MatrixUniformBlock, model), sizeof(glm::mat4), glm::value_ptr(mub.model));
    //glBufferData(GL_UNIFORM_BUFFER, sizeof(MatrixUniformBlock), (void*)&mub, GL_STATIC_DRAW);
    // ...

    std::unique_ptr<ShaderProgram> firstPassProgram, secondPassProgram, screenQuadProgram, ssaoPassProgram;
    Model *modelE112, *modelChairs, *modelDeskMid, *modelDeskSide;
    Mesh meshPointLight;
    QuadRenderer quadRenderer;
    MaterialDatabase materialDatabase;
    std::vector<std::unique_ptr<Light>> lightDatabase;
    
    Framebuffer framebuffer;//, framebufferSecond;
    Texture albedoTex, normalTex, depthTex, finalTex, noiseTex;
    SSAOBuilder ssaoBuilder;

    KeyFrameAnimation cameraAnim;
    
    //glm::quat rot;
    //Transform testTrans;

    glm::ivec2 screenSize, renderSize;
    Camera camera;
    bool freeCamera;
    
public:    
    // TODO adjust renderWidth/height with aspect ration? 16:9 vs 16:10?
    Scene(): renderSize(1280, 720), freeCamera(false)  {}
    ~Scene() {}
        
    bool OnInit(const AssetManager& assetManager, int width, int height);
    void OnResize(int w, int h);
    bool OnStep(const Time& time);
    void Destroy();
};
