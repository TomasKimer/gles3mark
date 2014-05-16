
#include <random>

#include "scene.h"
#include "sceneimporter.h"
#include "instancedatabuilder.h"
#include "log.h"
#include "glerror.h"
#include "meshrenderer.h"
#include "glquery.h"

Scene::Scene():
    renderSize(1280, 720),  // 640, 360  // 1280, 720  // 1920, 1080  // 3840, 2160
    freeCamera(false    ),
    cameraAnim(2        ), 
    camera(60.0f, 16.0f/9.0f, 1.0f, 256.0f, glm::vec4(0.5f, 0.5f, 1.f, 1.f)) {

}

Scene::~Scene() {

}

bool Scene::OnInit(const AssetManager& assetManager, int width, int height) {
    std::unique_ptr<AssimpSceneImporter> modelImporter = std::unique_ptr<AssimpSceneImporter>(new AssimpSceneImporter());
    
    // -----------------------------------------------------------------
    // ------------- load scene models, materials and lights -----------
    // -----------------------------------------------------------------
    std::vector<char> modelData(assetManager.LoadContents("models/e112max.dae"));
    modelE112 = modelImporter->Import(modelData, materialDatabase, lightDatabase);

    modelData = assetManager.LoadContents("models/chairs.dae");
    modelChairs = modelImporter->Import(modelData, materialDatabase, lightDatabase);

    modelData = assetManager.LoadContents("models/desk-mid.dae");
    modelDeskMid = modelImporter->Import(modelData, materialDatabase, lightDatabase);

    modelData = assetManager.LoadContents("models/desk-side.dae");
    modelDeskSide = modelImporter->Import(modelData, materialDatabase, lightDatabase);

    // --------------------------------------------------------
    // ------------- load textures and setup lights -----------
    // --------------------------------------------------------
    materialDatabase.LoadTextures(assetManager);

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    lightDatabase.clear();
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 20; j++) {
            glm::vec3 color = glm::vec3(1);
            
            if (i == 3)
                color = glm::vec3(dist(mt), dist(mt), dist(mt));
        
            lightDatabase.push_back(std::unique_ptr<Light>(new Light(glm::vec3(-70.0f + j*7, 2.5f, i*7.0f), color, 5.0f)));
        }
    }

    lightDatabase.push_back(std::unique_ptr<Light>(new Light(glm::vec3(  0.0, 30.0f, 10.0), glm::vec3(1.0f, 1.0f, 1.0f), 70.0f)));
    lightDatabase.push_back(std::unique_ptr<Light>(new Light(glm::vec3( 40.0, 30.0f, 10.0), glm::vec3(1.0f, 1.0f, 1.0f), 70.0f)));
    lightDatabase.push_back(std::unique_ptr<Light>(new Light(glm::vec3(-40.0, 30.0f, 10.0), glm::vec3(1.0f, 1.0f, 1.0f), 70.0f)));

    // -----------------------------------------------------------
    // ------------- init renderers and instanced data -----------
    // -----------------------------------------------------------
    modelE112->InitRenderer();
    
    modelChairs->InitRenderer();
    std::vector<glm::mat4>* instanceData = InstanceDataBuilder::BuildChairsInstanceData();
    modelChairs->renderer.InitInstanceData(*instanceData);
    delete instanceData;

    modelDeskMid->InitRenderer();
    instanceData = InstanceDataBuilder::BuildDeskMidInstanceData();
    modelDeskMid->renderer.InitInstanceData(*instanceData);
    delete instanceData;
    
    modelDeskSide->InitRenderer();
    instanceData = InstanceDataBuilder::BuildDeskSideInstanceData();
    modelDeskSide->renderer.InitInstanceData(*instanceData);
    delete instanceData;

    // point light mesh for deferred shading
    meshPointLight.MakeCube();
    meshPointLight.InitRenderer();

    // --------------------------------------------------
    // ------------- geometry pass shader setup ---------
    // --------------------------------------------------
    geometryPassProgram = std::unique_ptr<ShaderProgram>(new ShaderProgram(assetManager.LoadText("shaders/geometrypass-vs.glsl"),
                                                                           assetManager.LoadText("shaders/geometrypass-fs.glsl")));
    geometryPassProgram->AddUniform("model");
    geometryPassProgram->AddUniform("view");
    geometryPassProgram->AddUniform("projection");
    geometryPassProgram->AddUniform("tex");
    geometryPassProgram->AddUniform("diffuseColor");
    geometryPassProgram->AddUniform("hasTexture");
    geometryPassProgram->AddUniform("isInstanced");

    // ----------------------------------------------
    // ------------- ssao pass shader setup ---------
    // ----------------------------------------------
    ssaoPassProgram = std::unique_ptr<ShaderProgram>(new ShaderProgram(assetManager.LoadText("shaders/ssaopass-vs.glsl"),
                                                                       assetManager.LoadText("shaders/ssaopass-fs.glsl")));
    ssaoPassProgram->AddUniform("invProj");

    ssaoPassProgram->AddUniform("albedoTex");
    ssaoPassProgram->AddUniform("normalVStex");
    ssaoPassProgram->AddUniform("depthTex");

    ssaoPassProgram->Use();
    ssaoPassProgram->SetUniform("albedoTex", 0);
    ssaoPassProgram->SetUniform("normalVStex", 1);
    ssaoPassProgram->SetUniform("depthTex", 2);

    // ------------------------------------------------
    // ------------- light pass shader setup ----------
    // ------------------------------------------------
    lightPassProgram = std::unique_ptr<ShaderProgram>(new ShaderProgram(assetManager.LoadText("shaders/lightpass-vs.glsl"),
                                                                        assetManager.LoadText("shaders/lightpass-fs.glsl")));
    lightPassProgram->AddUniform("model");
    lightPassProgram->AddUniform("view");
    lightPassProgram->AddUniform("projection");
    lightPassProgram->AddUniform("invProj");
    lightPassProgram->AddUniform("viewport");

    lightPassProgram->AddUniform("albedoTex");
    lightPassProgram->AddUniform("normalVStex");
    lightPassProgram->AddUniform("depthTex");
    lightPassProgram->AddUniform("ssaoTex");

    lightPassProgram->AddUniform("lightColor");
    lightPassProgram->AddUniform("lightPos");
    lightPassProgram->AddUniform("lightSize");

    lightPassProgram->Use();  // gbuffer[2] => glUniform1iv(gbuffer, 3, {0,1,2});
    lightPassProgram->SetUniform("albedoTex", 0);
    lightPassProgram->SetUniform("normalVStex", 1);
    lightPassProgram->SetUniform("depthTex", 2);
    lightPassProgram->SetUniform("ssaoTex", 3);

    // ------------------------------------------------
    // ------------- screen quad shader setup ---------
    // ------------------------------------------------
    screenQuadProgram = std::unique_ptr<ShaderProgram>(new ShaderProgram(assetManager.LoadText("shaders/screenquad-vs.glsl"),
                                                                         assetManager.LoadText("shaders/screenquad-fs.glsl")));
    screenQuadProgram->AddUniform("tex");


    // ------------------------------------------------
    // ------------- gbuffer textures setup -----------
    // ------------------------------------------------
    albedoTex.InitStorage(GL_RGBA             , GL_RGBA           , GL_UNSIGNED_BYTE, renderSize.x, renderSize.y           );
    normalTex.InitStorage(GL_RGB16F           , GL_RGB            , GL_FLOAT        , renderSize.x, renderSize.y           );  // encode to GL_RG16F, GL_RG ?
    depthTex .InitStorage(GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT , renderSize.x, renderSize.y           );
    finalTex .InitStorage(GL_RGBA8            , GL_RGBA           , GL_UNSIGNED_BYTE, renderSize.x, renderSize.y, GL_LINEAR);
    ssaoTex  .InitStorage(GL_RED              , GL_RED            , GL_UNSIGNED_BYTE, renderSize.x, renderSize.y           );

    // --------------------------------------------
    // ------------- framebuffers setup -----------
    // --------------------------------------------
    framebuffer.Bind();
    framebuffer.Attach(depthTex , GL_DEPTH_ATTACHMENT );
    framebuffer.Attach(albedoTex, GL_COLOR_ATTACHMENT0);    
    framebuffer.Attach(normalTex, GL_COLOR_ATTACHMENT1);
    framebuffer.Attach(finalTex , GL_COLOR_ATTACHMENT2);
    framebuffer.Attach(ssaoTex  , GL_COLOR_ATTACHMENT3);
    framebuffer.CheckCompleteness();
    
    // ------------------------------------------------
    // ------------- camera animation setup -----------
    // ------------------------------------------------
    cameraAnim.MakeOrbit(72.0f, 0.1f, 50.0f);  
    camera.LookAt(cameraAnim.GetKeyFrame(0).position, glm::vec3(0.f, 10.f, 1.f));  //camera.Move(glm::vec3(0, 20, -50.f));
     
    // ---------------------------------------
    // ------------- misc gl setup -----------
    // ---------------------------------------
    glEnable(GL_CULL_FACE);
    glEnable(GL_SCISSOR_TEST);
    
    return true;
}

void Scene::OnResize(int w, int h) {
	screenSize = glm::ivec2(w, h);
    //camera.UpdateAspect(static_cast<float>(screenSize.x) / screenSize.y);
}

bool Scene::OnStep(const Time& time) {
    // animation
    if (!freeCamera && time.RealTimeSinceStartup() > 1.0f) {
        cameraAnim.Update(time.DeltaTime());
        camera.LookAt(cameraAnim.GetCurrentPosition(), glm::vec3(0.f, 10.f, 1.f));
    }
    
    // camera properties
    glm::vec4& bc            = camera.GetBackgroundColor();
    glm::mat4& projection    = camera.GetProjectionMatrix();
    glm::mat4& view          = camera.GetViewMatrix();
    glm::mat4  projectionInv = glm::inverse(projection);

    // -----------------------------------------------------
    // ------- first pass - render to textures -------------
    // -----------------------------------------------------
    framebuffer.Bind();
    framebuffer.ActiveColorAttachments(std::vector<GLenum>{GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_NONE, GL_NONE});

    glViewport(0, 0, renderSize.x, renderSize.y);
    glScissor (0, 0, renderSize.x, renderSize.y);

    glDepthMask(GL_TRUE); // enable writing into the depth buffer

    glClearColor(bc.r, bc.g, bc.b, bc.a); //
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    
    
    glEnable(GL_DEPTH_TEST); 
    glDepthFunc(GL_LESS); // passes if the incoming depth value is less than the stored depth value.    

    geometryPassProgram->Use();
    geometryPassProgram->SetUniform("tex", 0);
    geometryPassProgram->SetUniform("projection", projection);
    geometryPassProgram->SetUniform("view", view);

    modelE112->matrix = glm::scale(glm::mat4(), glm::vec3(0.1f, 0.1f, 0.1f));
    modelE112->renderer.Render(*geometryPassProgram, materialDatabase);

#ifdef _DEBUG    // draw cubes at light positions
    for (std::unique_ptr<Light>& l : lightDatabase) {
        if (l->type != Light::Type::Point) continue;
        meshPointLight.renderer.PreRender(*geometryPassProgram, Material(glm::vec4(l->diffuseColor, 1.0f)), glm::translate(glm::mat4(), l->position));
        meshPointLight.renderer.Render();
    }
#endif

    modelChairs  ->renderer.RenderInstanced(*geometryPassProgram, materialDatabase, modelChairs  ->renderer.GetInstanceCount());
    modelDeskMid ->renderer.RenderInstanced(*geometryPassProgram, materialDatabase, modelDeskMid ->renderer.GetInstanceCount());
    modelDeskSide->renderer.RenderInstanced(*geometryPassProgram, materialDatabase, modelDeskSide->renderer.GetInstanceCount());

    // -------------------------------------------------
    // -------- second pass - SSAO ---------------------
    // -------------------------------------------------
    framebuffer.ActiveColorAttachments(std::vector<GLenum>{GL_NONE, GL_NONE, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3});
    
    glDisable(GL_DEPTH_TEST); 
    glDepthMask(GL_FALSE); // disable writing into the depth buffer
    
    ssaoPassProgram->Use();    
    ssaoPassProgram->SetUniform("invProj", projectionInv);

    albedoTex.Bind(GL_TEXTURE0);
    normalTex.Bind(GL_TEXTURE1);    
    depthTex .Bind(GL_TEXTURE2);
    
    quadRenderer.Render(0.0f, 0.0f, 2.0f);
    
    
    // -------------------------------------------------
    // -------- third pass - lights -------------------
    // -------------------------------------------------
    framebuffer.ActiveColorAttachments(std::vector<GLenum>{GL_NONE, GL_NONE, GL_COLOR_ATTACHMENT2, GL_NONE});
        
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);
    
    glEnable(GL_DEPTH_TEST); 
    glDepthFunc(GL_GEQUAL); // passes if the incoming depth value is greater than or equal to the stored depth value
    glCullFace(GL_FRONT);   // front-facing polygons are culled

    lightPassProgram->Use();
    lightPassProgram->SetUniform("projection", projection);
    lightPassProgram->SetUniform("invProj", projectionInv);
    lightPassProgram->SetUniform("view", view);    
    lightPassProgram->SetUniform("viewport", glm::vec2(renderSize));

    //albedoTex.Bind(GL_TEXTURE0);
    //normalTex.Bind(GL_TEXTURE1);    
    //depthTex .Bind(GL_TEXTURE2);
    ssaoTex.Bind(GL_TEXTURE3);

    for (std::unique_ptr<Light>& l : lightDatabase) {
        if (l->type != Light::Type::Point)
            continue;
           
        lightPassProgram->SetUniform("model", l->matrix);
        lightPassProgram->SetUniform("lightPos", glm::vec3(view * glm::vec4(l->position, 1)));
        lightPassProgram->SetUniform("lightColor", l->diffuseColor);
        lightPassProgram->SetUniform("lightSize", l->size);    

        meshPointLight.renderer.Render();   // could be also instanced
    }
    

    // -------------------------------------------------
    // --------- final pass - show textures ------------
    // -------------------------------------------------
    Framebuffer::BindDefault();

    glViewport(0, 0, screenSize.x, screenSize.y);
    glScissor (0, 0, screenSize.x, screenSize.y);

    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    glCullFace(GL_BACK);
    glDisable(GL_BLEND);

    screenQuadProgram->Use();
    screenQuadProgram->SetUniform("tex", 0);
    
    finalTex.Bind();
    quadRenderer.Render(0.0f, 0.0f, 2.0f);

#ifndef ANDROID_PRODUCTION
#ifdef _DEBUG
    ssaoTex.Bind();
    quadRenderer.Render(0.0f, 0.0f, 0.5f);

    normalTex.Bind();
    quadRenderer.Render(1.5f, 0.0f, 0.5f);
#endif
#endif


#ifdef ANDROID
    glFinish();  //- not causing delay on swaping egl context if vsync is off, and eliminates tearing on low fps (both vsync on and off)
#endif

    // eglSwapBuffers performs an implicit flush operation on the context (glFlush for an OpenGL ES or OpenGL context,
    // vgFlush for an OpenVG context) bound to surface before swapping. Subsequent client API commands may be issued on
    // that context immediately after calling eglSwapBuffers, but are not executed until the buffer exchange is completed.
    // FENCE - http://permalink.gmane.org/gmane.comp.lib.cairo/24458

    if (cameraAnim.HasEnded())
        return false;

    return true;
}

void Scene::Destroy() {
    delete modelE112;
    delete modelChairs;
    delete modelDeskMid;
    delete modelDeskSide;
}