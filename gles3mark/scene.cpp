
#include "scene.h"

bool Scene::OnInit(std::unique_ptr<AssetManager>& assetManager, int width, int height) {

    //texture.FromKTXdata(assetManager->LoadContents("textures/chair2.ktx"));  // chair512_etc2rgb_mip_slowperc.ktx
    
    AssimpSceneImporter* modelImporter = new AssimpSceneImporter(/* *assetManager*/);
    
    std::vector<char> modelData(assetManager->LoadContents("models/e112max.dae")); // e112.3ds
    modelE112 = modelImporter->Import(modelData, materialDatabase);

    modelData = assetManager->LoadContents("models/chairs.dae");
    modelChairs = modelImporter->Import(modelData, materialDatabase);

    modelData = assetManager->LoadContents("models/desk-mid.dae");
    modelDeskMid = modelImporter->Import(modelData, materialDatabase);

    modelData = assetManager->LoadContents("models/desk-side.dae");
    modelDeskSide = modelImporter->Import(modelData, materialDatabase);

    for (Material* m : materialDatabase) {
        if (m->hasTexture) {
            std::string ktxPath = m->texture->path.substr(0, m->texture->path.find_last_of(".")) + ".ktx";
            std::transform(ktxPath.begin(), ktxPath.end(), ktxPath.begin(), ::tolower);
            m->texture->FromKTXdata(assetManager->LoadContents("textures/" + ktxPath)); //test/rgb-reference.ktx
        }            
    }

    for (Mesh* m : modelE112->GetMeshes()) {
        m->InitRenderer();
        m->FreeMemory();
    }
    for (Mesh* m : modelChairs->GetMeshes()) {
        m->InitRenderer();
        m->FreeMemory();
    }
    for (Mesh* m : modelDeskMid->GetMeshes()) {
        m->InitRenderer();
        m->FreeMemory();
    }
    for (Mesh* m : modelDeskSide->GetMeshes()) {
        m->InitRenderer();
        m->FreeMemory();
    }

    firstPassProgram = new ShaderProgram(assetManager->LoadText("shaders/firstpass.vert"),
                                         assetManager->LoadText("shaders/firstpass.frag"));
    firstPassProgram->AddUniform("model");
    firstPassProgram->AddUniform("view");
    firstPassProgram->AddUniform("projection");
    firstPassProgram->AddUniform("tex");
    firstPassProgram->AddUniform("diffuseColor");
    firstPassProgram->AddUniform("hasTexture");


    screenQuadProgram = new ShaderProgram(assetManager->LoadText("shaders/screenquad.vert"),
                                          assetManager->LoadText("shaders/screenquad.frag"));
    screenQuadProgram->AddUniform("tex");


    depthRenderbuf.InitStorage(GL_DEPTH_COMPONENT24, renderWidth, renderHeight);
    diffuseTex.InitStorage(GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, renderWidth, renderHeight);
    positionTex.InitStorage(GL_R11F_G11F_B10F, GL_RGB, GL_FLOAT, renderWidth, renderHeight); // GL_RGB16F
    normalTex.InitStorage(GL_R11F_G11F_B10F, GL_RGB, GL_FLOAT, renderWidth, renderHeight);

    framebuffer.Bind();
    framebuffer.Attach(depthRenderbuf, GL_DEPTH_ATTACHMENT);
    framebuffer.Attach(diffuseTex, GL_COLOR_ATTACHMENT0);
    framebuffer.Attach(positionTex, GL_COLOR_ATTACHMENT1);
    framebuffer.Attach(normalTex, GL_COLOR_ATTACHMENT2);
    framebuffer.ActiveColorAttachments(std::vector<GLenum>{GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2});
    framebuffer.CheckCompleteness();
    framebuffer.Unbind();


    camera.Move(glm::vec3(0, 20, -50.f));

    //cameraAnim.AddKeyFrame(KeyFrame(camera.GetEye(), camera.GetTarget(), 0));
    
    int j = 1;
    for (float i = -glm::pi<float>() / 2.f; i < 2*glm::pi<float>() - glm::pi<float>() / 2.f; i += (2*glm::pi<float>() / 72.0f)) {
        float x = 50 * glm::cos((float)i);
        float z = 50 * glm::sin((float)i);

        //Log::V() << x << " " << z;
        
        cameraAnim.AddKeyFrame(KeyFrame(glm::vec3(x, 20+5*glm::sin(i*2), z), camera.GetTarget(), j*0.1f));        
        j++;
    }
    
    
  /*cameraAnim.AddKeyFrame(KeyFrame(glm::vec3(50, 20, 0), camera.GetTarget(), 3));
    cameraAnim.AddKeyFrame(KeyFrame(glm::vec3(0, 20, 50), camera.GetTarget(), 6));
    cameraAnim.AddKeyFrame(KeyFrame(glm::vec3(-50, 20, 0), camera.GetTarget(), 9));
    cameraAnim.AddKeyFrame(KeyFrame(glm::vec3(0, 20, -50), camera.GetTarget(), 12));*/




    testTrans.Rotate(Transform::Up(), glm::radians(90.0f), Transform::Space::World);
    testTrans.Translate(glm::vec3(0,0,-70));
     
    //glEnable(GL_DEPTH_TEST); // glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glClearColor(0.5f, 0.5f, 1.f, 1.f);
    
    return true;
}

void Scene::OnResize(int w, int h) {
	//glViewport(0, 0, w, h);
	width = w;
	height = h;
    camera.Perspective(glm::radians(60.0f), static_cast<float>(renderWidth) / renderHeight, 1.0f, 1000.0f);
    //camera.Orthographic(0.0f, static_cast<float>(w), static_cast<float>(h), 0.0f, 1.0f, 1000.0f);
}

void Scene::RenderModel(Model* model, const glm::mat4& modelM) {
    for (Mesh* m : model->GetMeshes()) {
        //if (i == 5 || i == 7) continue;  // chair realTime_quality bad mesh

        // transform
        //glm::mat4 mvp = projection * view * modelM * m->matrix;

        firstPassProgram->SetUniform("model", modelM * m->matrix);
        
        // material
        Material* mat = materialDatabase[m->materialID];
        firstPassProgram->SetUniform("diffuseColor", mat->diffuseColor); 
        firstPassProgram->SetUniform("hasTexture", mat->hasTexture);
        if (mat->hasTexture)
            mat->texture->Bind(GL_TEXTURE0);
        
        // draw
        m->renderer.Render();
    }
}

bool Scene::OnStep(const Time& time) {
    if (!freeCamera) {
        cameraAnim.Update(time.DeltaTime());
        camera.LookAt(cameraAnim.GetCurrentPosition(), cameraAnim.GetCurrentDirection());
    }
    
    //MVP
    glm::mat4& projection = camera.GetProjectionMatrix();
    glm::mat4& view       = camera.GetViewMatrix();

    rot = glm::rotate(rot, time.DeltaTime(), glm::vec3(0, 1, 0));
    glm::mat4 modelM = glm::scale(glm::mat4(), glm::vec3(0.1f, 0.1f, 0.1f)); //glm::mat4_cast(rot); //testTrans.GetMatrix(); //        
    
    //glm::mat4 mvp = projection * view * model;

    
    framebuffer.Bind();
    glViewport(0, 0, renderWidth, renderHeight);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    firstPassProgram->Use();
    firstPassProgram->SetUniform("tex", 0);
    firstPassProgram->SetUniform("projection", projection);
    firstPassProgram->SetUniform("view", view);

    RenderModel(modelE112, modelM);
    RenderModel(modelChairs, glm::translate(modelM, glm::vec3(-740, 19, -70)));
    RenderModel(modelDeskMid, glm::translate(modelM, glm::vec3(-365, 13, -43)));
    RenderModel(modelDeskSide, glm::translate(modelM, glm::vec3(-785, 20, -15)));

    //for (Mesh* m : modelE112->GetMeshes()) {
    //    //if (i == 5 || i == 7) continue;

    //    // transform
    //    //glm::mat4 mvp = projection * view * modelM * m->matrix;
    //    firstPassProgram->SetUniform("projection", projection);
    //    firstPassProgram->SetUniform("view", view);
    //    firstPassProgram->SetUniform("model", modelM * m->matrix);
    //    
    //    // material
    //    Material* mat = materialDatabase[m->materialID];
    //    firstPassProgram->SetUniform("diffuseColor", mat->diffuseColor); 
    //    firstPassProgram->SetUniform("hasTexture", mat->hasTexture);
    //    if (mat->hasTexture)
    //        mat->texture->Bind(GL_TEXTURE0);
    //    
    //    // draw
    //    m->renderer.Render();
    //}

    framebuffer.Unbind();
    glViewport(0, 0, width, height);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);

    screenQuadProgram->Use();
    screenQuadProgram->SetUniform("tex", 0);
    diffuseTex.Bind();
    quadRenderer.Render(0.0f, 0.0f, 2.0f);

    normalTex.Bind();
    quadRenderer.Render(0.0f, 0.0f, 0.5f);

    positionTex.Bind();
    quadRenderer.Render(1.5f, 0.0f, 0.5f);

    //glFlush();
#ifdef _ANDROID
    glFinish();  //- not causing delay on swaping egl context
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

    //for (Mesh* m : model->GetMeshes()) {
    //    m->renderer.Destroy();
    //}

    delete firstPassProgram;
    delete secondPassProgram;
    delete screenQuadProgram;
}