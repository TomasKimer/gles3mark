
#include <random>

#include "scene.h"
#include "instancedatabuilder.h"


bool Scene::OnInit(std::unique_ptr<AssetManager>& assetManager, int width, int height) {
    AssimpSceneImporter* modelImporter = new AssimpSceneImporter(/* *assetManager*/);
    
    // -----------------------------------------------------------------
    // ------------- load scene models, materials and lights -----------
    // -----------------------------------------------------------------
    std::vector<char> modelData(assetManager->LoadContents("models/e112max.dae")); // e112.3ds
    modelE112 = modelImporter->Import(modelData, materialDatabase, lightDatabase);

    modelData = assetManager->LoadContents("models/chairs.dae");
    modelChairs = modelImporter->Import(modelData, materialDatabase, lightDatabase);

    modelData = assetManager->LoadContents("models/desk-mid.dae");
    modelDeskMid = modelImporter->Import(modelData, materialDatabase, lightDatabase);

    modelData = assetManager->LoadContents("models/desk-side.dae");
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
        
            lightDatabase.push_back(new Light(glm::vec3(-70.0f + j*7, 2.5f, i*7.0f), color, 5.0f));
        }
    }

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

    // -----------------------------------------------
    // ------------- first pass shader setup ---------
    // -----------------------------------------------
    firstPassProgram = new ShaderProgram(assetManager->LoadText("shaders/firstpass.vert"),
                                         assetManager->LoadText("shaders/firstpass.frag"));
    firstPassProgram->AddUniform("model");
    firstPassProgram->AddUniform("view");
    firstPassProgram->AddUniform("projection");
    firstPassProgram->AddUniform("tex");
    firstPassProgram->AddUniform("diffuseColor");
    firstPassProgram->AddUniform("hasTexture");
    firstPassProgram->AddUniform("isInstanced");

    // ------------------------------------------------
    // ------------- second pass shader setup ---------
    // ------------------------------------------------
    secondPassProgram = new ShaderProgram(assetManager->LoadText("shaders/secondpass.vert"),
                                          assetManager->LoadText("shaders/secondpass.frag"));
    secondPassProgram->AddUniform("model");
    secondPassProgram->AddUniform("view");
    secondPassProgram->AddUniform("projection");
    secondPassProgram->AddUniform("invProj");
    secondPassProgram->AddUniform("viewport");

    secondPassProgram->AddUniform("albedoTex");
    secondPassProgram->AddUniform("normalVStex");
    secondPassProgram->AddUniform("depthTex");

    secondPassProgram->AddUniform("lightColor");
    secondPassProgram->AddUniform("lightPos");
    secondPassProgram->AddUniform("lightSize");

    secondPassProgram->Use();  // gbuffer[2] ?  glUniform1iv(gbuffer, 3, {0,1,2});
    secondPassProgram->SetUniform("albedoTex", 0);
    secondPassProgram->SetUniform("normalVStex", 1);
    secondPassProgram->SetUniform("depthTex", 2);

    // ------------------------------------------------
    // ------------- screen quad shader setup ---------
    // ------------------------------------------------
    screenQuadProgram = new ShaderProgram(assetManager->LoadText("shaders/screenquad.vert"),
                                          assetManager->LoadText("shaders/screenquad.frag"));
    screenQuadProgram->AddUniform("tex");

    // ------------------------------------------------
    // ------------- gbuffer textures setup -----------
    // ------------------------------------------------
    albedoTex.InitStorage(GL_RGBA            , GL_RGBA           , GL_UNSIGNED_BYTE, renderSize.x, renderSize.y           );
    normalTex.InitStorage(GL_R11F_G11F_B10F  , GL_RGB            , GL_FLOAT        , renderSize.x, renderSize.y           );  // GL_R11F_G11F_B10F , GL_RGB  | GL_RG16F, GL_RG
    depthTex.InitStorage(GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT , renderSize.x, renderSize.y           ); // GL_RG16F
    finalTex.InitStorage(GL_RGBA             , GL_RGBA           , GL_UNSIGNED_BYTE, renderSize.x, renderSize.y, GL_LINEAR);    

    // --------------------------------------------
    // ------------- framebuffers setup -----------
    // --------------------------------------------
    framebuffer.Bind();
    framebuffer.Attach(depthTex , GL_DEPTH_ATTACHMENT );
    framebuffer.Attach(albedoTex, GL_COLOR_ATTACHMENT0);    
    framebuffer.Attach(normalTex, GL_COLOR_ATTACHMENT1);
    framebuffer.Attach(finalTex , GL_COLOR_ATTACHMENT2);
    framebuffer.ActiveColorAttachments(std::vector<GLenum>{GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2});
    framebuffer.CheckCompleteness();
    
    framebufferSecond.Bind();
    framebufferSecond.Attach(depthTex, GL_DEPTH_ATTACHMENT);
    framebufferSecond.Attach(finalTex, GL_COLOR_ATTACHMENT0);
    framebufferSecond.ActiveColorAttachments(std::vector<GLenum>{GL_COLOR_ATTACHMENT0});
    framebufferSecond.CheckCompleteness();

    // ------------------------------------------------
    // ------------- camera animation setup -----------
    // ------------------------------------------------
    cameraAnim.MakeOrbit(72.0f, 0.1f, 50.0f, camera.GetTarget());  
    camera.LookAt(cameraAnim.GetKeyFrame(0).position, cameraAnim.GetKeyFrame(0).direction);  //camera.Move(glm::vec3(0, 20, -50.f));
     
    // ---------------------------------------
    // ------------- misc gl setup -----------
    // ---------------------------------------
    glEnable(GL_CULL_FACE);
    
    return true;
}

void Scene::OnResize(int w, int h) {
	screenSize = glm::ivec2(w, h);
    camera.Perspective(glm::radians(60.0f), static_cast<float>(screenSize.x) / screenSize.y, 1.0f, 200.0f);
}

bool Scene::OnStep(const Time& time) {
    if (!freeCamera && time.RealTimeSinceStartup() > 1.0f) {
        cameraAnim.Update(time.DeltaTime());
        camera.LookAt(cameraAnim.GetCurrentPosition(), cameraAnim.GetCurrentDirection());
    }
    
    // MVP
    glm::mat4& projection    = camera.GetProjectionMatrix();
    glm::mat4& view          = camera.GetViewMatrix();
    glm::mat4  projectionInv = glm::inverse(projection);

    //testTrans.Rotate(Transform::Up(), glm::radians(90.0f), Transform::Space::World);
    //testTrans.Translate(glm::vec3(0,0,-70));
    //rot = glm::rotate(rot, time.DeltaTime(), glm::vec3(0, 1, 0));
    //glm::mat4 modelMat = glm::scale(glm::mat4(), glm::vec3(0.1f, 0.1f, 0.1f)); //glm::mat4_cast(rot); //testTrans.GetMatrix(); //      
    //glm::mat4 mvp = projection * view * model;


    // -----------------------------------------------------
    // ------- first pass - render to textures -------------
    // -----------------------------------------------------
    glViewport(0, 0, renderSize.x, renderSize.y);
    framebuffer.Bind();

    glDepthMask(GL_TRUE); // enable writing into the depth buffer

    glClearColor(0.5f, 0.5f, 1.f, 1.f); //
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    
    
    glEnable(GL_DEPTH_TEST); 
    glDepthFunc(GL_LESS); // passes if the incoming depth value is less than the stored depth value.    

    firstPassProgram->Use();
    firstPassProgram->SetUniform("tex", 0);
    firstPassProgram->SetUniform("projection", projection);
    firstPassProgram->SetUniform("view", view);

    modelE112->matrix = glm::scale(glm::mat4(), glm::vec3(0.1f, 0.1f, 0.1f));
    modelE112->renderer.Render(*firstPassProgram, materialDatabase);

    //Material lightMat = Material(glm::vec4(0,1,0,1));
    //meshPointLight.renderer.PreRender(*firstPassProgram, lightMat);
    //meshPointLight.renderer.Render();
    
    modelChairs  ->renderer.RenderInstanced(*firstPassProgram, materialDatabase, modelChairs  ->renderer.GetInstanceCount());
    modelDeskMid ->renderer.RenderInstanced(*firstPassProgram, materialDatabase, modelDeskMid ->renderer.GetInstanceCount());
    modelDeskSide->renderer.RenderInstanced(*firstPassProgram, materialDatabase, modelDeskSide->renderer.GetInstanceCount());


    // -------------------------------------------------
    // -------- second pass - lights -------------------
    // -------------------------------------------------
    framebufferSecond.Bind();
    
    //glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    //glClear(GL_COLOR_BUFFER_BIT);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);
    
    glDepthMask(GL_FALSE);  // disable writing into the depth buffer
    glDepthFunc(GL_GEQUAL); // passes if the incoming depth value is greater than or equal to the stored depth value
    glCullFace(GL_FRONT);   // front-facing polygons are culled

    secondPassProgram->Use();
    secondPassProgram->SetUniform("projection", projection);
    secondPassProgram->SetUniform("invProj", projectionInv);
    secondPassProgram->SetUniform("view", view);    
    secondPassProgram->SetUniform("viewport", glm::vec2(renderSize));

    albedoTex.Bind(GL_TEXTURE0);
    normalTex.Bind(GL_TEXTURE1);    
    depthTex .Bind(GL_TEXTURE2);

    for (Light* l : lightDatabase) {
        if (l->type != Light::Type::Point)
            continue;
        
        glm::mat4 lightModelMatrix = glm::scale(glm::translate(glm::mat4(), l->position), glm::vec3(l->size));
           
        secondPassProgram->SetUniform("model", lightModelMatrix);
        secondPassProgram->SetUniform("lightPos", glm::vec3(view * glm::vec4(l->position, 1)));
        secondPassProgram->SetUniform("lightColor", l->diffuseColor);
        secondPassProgram->SetUniform("lightSize", l->size);    

        meshPointLight.renderer.Render();   // TODO instancing? 
    }
    

    // -------------------------------------------------
    // -------- third pass - show textures ------------
    // -------------------------------------------------
    glViewport(0, 0, screenSize.x, screenSize.y);
    Framebuffer::BindDefault();

    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    glCullFace(GL_BACK);  // back-facing polygons are culled
    glDisable(GL_BLEND);

    screenQuadProgram->Use();
    screenQuadProgram->SetUniform("tex", 0);
    
    finalTex.Bind();
    quadRenderer.Render(0.0f, 0.0f, 2.0f);

    albedoTex.Bind();
    quadRenderer.Render(0.0f, 0.0f, 0.5f);

    normalTex.Bind();
    quadRenderer.Render(1.5f, 0.0f, 0.5f);

    //glFlush();
#ifdef ANDROID
//    glFinish();  //- not causing delay on swaping egl context
#endif
    // eglSwapBuffers performs an implicit flush operation on the context (glFlush for an OpenGL ES or OpenGL context,
    // vgFlush for an OpenVG context) bound to surface before swapping. Subsequent client API commands may be issued on
    // that context immediately after calling eglSwapBuffers, but are not executed until the buffer exchange is completed.

    // FENCE - http://permalink.gmane.org/gmane.comp.lib.cairo/24458

    //assert(glGetError() == GL_NO_ERROR);

    return true;
}

void Scene::Destroy() {
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // TODO destroy models etc

    delete firstPassProgram;
    delete secondPassProgram;
    delete screenQuadProgram;
}