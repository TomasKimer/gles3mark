
#include "scene.h"
#include "instancedatabuilder.h"

bool Scene::OnInit(std::unique_ptr<AssetManager>& assetManager, int width, int height) {
    AssimpSceneImporter* modelImporter = new AssimpSceneImporter(/* *assetManager*/);
    
    std::vector<char> modelData(assetManager->LoadContents("models/e112max.dae")); // e112.3ds
    modelE112 = modelImporter->Import(modelData, materialDatabase);

    modelData = assetManager->LoadContents("models/chairs.dae");
    modelChairs = modelImporter->Import(modelData, materialDatabase);

    modelData = assetManager->LoadContents("models/desk-mid.dae");
    modelDeskMid = modelImporter->Import(modelData, materialDatabase);

    modelData = assetManager->LoadContents("models/desk-side.dae");
    modelDeskSide = modelImporter->Import(modelData, materialDatabase);


    materialDatabase.LoadTextures(assetManager);

    
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


    meshPointLight.MakeCube();
    meshPointLight.InitRenderer();


    firstPassProgram = new ShaderProgram(assetManager->LoadText("shaders/firstpass.vert"),
                                         assetManager->LoadText("shaders/firstpass.frag"));
    firstPassProgram->AddUniform("model");
    firstPassProgram->AddUniform("view");
    firstPassProgram->AddUniform("projection");
    firstPassProgram->AddUniform("tex");
    firstPassProgram->AddUniform("diffuseColor");
    firstPassProgram->AddUniform("hasTexture");
    firstPassProgram->AddUniform("isInstanced");


    screenQuadProgram = new ShaderProgram(assetManager->LoadText("shaders/screenquad.vert"),
                                          assetManager->LoadText("shaders/screenquad.frag"));
    screenQuadProgram->AddUniform("tex");


    //depthRenderbuf.InitStorage(GL_DEPTH_COMPONENT24, renderWidth, renderHeight);
    albedoTex.InitStorage(GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, renderWidth, renderHeight);
    normalTex.InitStorage(GL_R11F_G11F_B10F, GL_RGB, GL_FLOAT, renderWidth, renderHeight);  // GL_R11F_G11F_B10F , GL_RGB  | GL_RG16F, GL_RG
    depthTex.InitStorage(GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, renderWidth, renderHeight); // GL_RG16F
    

    framebuffer.Bind();
    //framebuffer.Attach(depthRenderbuf, GL_DEPTH_ATTACHMENT);
    framebuffer.Attach(albedoTex, GL_COLOR_ATTACHMENT0);    
    framebuffer.Attach(normalTex, GL_COLOR_ATTACHMENT1);
    framebuffer.Attach(depthTex, GL_DEPTH_ATTACHMENT);
    framebuffer.ActiveColorAttachments(std::vector<GLenum>{GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1});
    framebuffer.CheckCompleteness();
    framebuffer.Unbind();


    //camera.Move(glm::vec3(0, 20, -50.f));
    cameraAnim.MakeOrbit(72.0f, 0.1f, 50.0f, camera.GetTarget());
    camera.LookAt(cameraAnim.GetKeyFrame(0).position, cameraAnim.GetKeyFrame(0).direction);
    

    //testTrans.Rotate(Transform::Up(), glm::radians(90.0f), Transform::Space::World);
    //testTrans.Translate(glm::vec3(0,0,-70));
     
    glEnable(GL_CULL_FACE);
    glClearColor(0.5f, 0.5f, 1.f, 1.f);
    
    return true;
}

void Scene::OnResize(int w, int h) {
	width = w;
	height = h;
    camera.Perspective(glm::radians(60.0f), static_cast<float>(renderWidth) / renderHeight, 0.25f, 1024.0f);
    //camera.Orthographic(0.0f, static_cast<float>(w), static_cast<float>(h), 0.0f, 0.25f, 1024.0f);
}

bool Scene::OnStep(const Time& time) {
    if (!freeCamera && time.RealTimeSinceStartup() > 1.0f) {
        cameraAnim.Update(time.DeltaTime());
        camera.LookAt(cameraAnim.GetCurrentPosition(), cameraAnim.GetCurrentDirection());
    }
    
    //MVP
    glm::mat4& projection = camera.GetProjectionMatrix();
    glm::mat4& view       = camera.GetViewMatrix();

    //rot = glm::rotate(rot, time.DeltaTime(), glm::vec3(0, 1, 0));
    glm::mat4 modelMat = glm::scale(glm::mat4(), glm::vec3(0.1f, 0.1f, 0.1f)); //glm::mat4_cast(rot); //testTrans.GetMatrix(); //      
    //glm::mat4 mvp = projection * view * model;


    // -----------------------------------------------------
    // ------- first pass - render to textures -------------
    // -----------------------------------------------------
    framebuffer.Bind();
    glViewport(0, 0, renderWidth, renderHeight);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    firstPassProgram->Use();
    firstPassProgram->SetUniform("tex", 0);
    firstPassProgram->SetUniform("projection", projection);
    firstPassProgram->SetUniform("view", view);

    modelE112->matrix = modelMat;
    modelE112->renderer.Render(*firstPassProgram, materialDatabase);

    Material lightMat = Material(glm::vec4(0,1,0,1));
    meshPointLight.renderer.PreRender(*firstPassProgram, lightMat);
    meshPointLight.renderer.Render();
    
    //modelChairs  ->renderer.RenderInstanced(*firstPassProgram, materialDatabase, modelChairs  ->renderer.GetInstanceCount());
    modelDeskMid ->renderer.RenderInstanced(*firstPassProgram, materialDatabase, modelDeskMid ->renderer.GetInstanceCount());
    modelDeskSide->renderer.RenderInstanced(*firstPassProgram, materialDatabase, modelDeskSide->renderer.GetInstanceCount());


    // -------------------------------------------------
    // -------- second pass - show textures ------------
    // -------------------------------------------------
    framebuffer.Unbind();
    glViewport(0, 0, width, height);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);

    screenQuadProgram->Use();
    screenQuadProgram->SetUniform("tex", 0);
    albedoTex.Bind();
    quadRenderer.Render(0.0f, 0.0f, 2.0f);

    normalTex.Bind();
    quadRenderer.Render(0.0f, 0.0f, 0.5f);

    depthTex.Bind();
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