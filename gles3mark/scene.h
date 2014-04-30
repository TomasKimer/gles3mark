

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

#include <memory>
#include <string>
#include <vector>
#include <cassert>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>


class Scene {  // public Screen?
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

    ShaderProgram* firstPassProgram;
    ShaderProgram* secondPassProgram;
    ShaderProgram* screenQuadProgram;
    Model *modelE112, *modelChairs, *modelDeskMid, *modelDeskSide;
    QuadRenderer quadRenderer;
    std::vector<Material*> materialDatabase;

    void RenderModel(Model* model, const glm::mat4& modelM, unsigned instanceCount = 1);
    
    Framebuffer framebuffer;
    Texture diffuseTex, positionTex, normalTex;
    RenderBuffer depthRenderbuf;

    KeyFrameAnimation cameraAnim;
    
    //GLTexture texture;
    glm::quat rot;
    Transform testTrans;

    int width, height;
    int renderWidth, renderHeight;
    
public:
    Camera camera;
    bool freeCamera;
    
    Scene(): renderWidth(1280), renderHeight(720), freeCamera(false)  {}
    ~Scene() {}
        
    bool OnInit(std::unique_ptr<AssetManager>& assetManager, int width, int height);
    void OnResize(int w, int h);
    bool OnStep(const Time& time);
    void Destroy();
};
