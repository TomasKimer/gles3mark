

#pragma once

#include "log.h"
#include "assetmanager.h"
#include "sceneimporter.h"
#include "camera.h"
#include "glhelper.h"
#include "time.h"
#include "texture.h"
#include "meshrenderer.h"
#include "shaderprogram.h"

#include <string>
#include <vector>
#include <cassert>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>


class GLTest {
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

    ShaderProgram* shaderProgramSimple;
    Model* model;
    std::vector<Material*> materialDatabase;

    //GLTexture texture;
    glm::quat rot;
    Transform testTrans;

    int width, height;
    
public:
    Camera camera;
    
    GLTest()  {}
    ~GLTest() {}
        
    bool OnInit(AssetManager* assetManager) {
        GLHelper::GLInfo();        
        try {            
            //texture.FromKTXdata(assetManager->LoadContents("textures/chair2.ktx"));  // chair512_etc2rgb_mip_slowperc.ktx
            
            AssimpSceneImporter* modelImporter = new AssimpSceneImporter(/* *assetManager*/);
            std::vector<char> modelData(assetManager->LoadContents("models/e112.3ds"));
            model = modelImporter->Import(modelData, materialDatabase);

            for (Material* m : materialDatabase) {
                if (m->hasTexture) {
                    std::string ktxPath = m->texture->path.substr(0, m->texture->path.find_last_of(".")) + ".ktx";
                    std::transform(ktxPath.begin(), ktxPath.end(), ktxPath.begin(), ::tolower);
                    m->texture->FromKTXdata(assetManager->LoadContents("textures/" + ktxPath));
                }            
            }

            for (size_t i = 0; i < model->GetMeshes().size(); ++i) {
                Mesh* m = model->GetMeshes()[i];
                m->InitRenderer();
                m->FreeMemory();
            }

            shaderProgramSimple = new ShaderProgram(assetManager->LoadText("shaders/simple.vert"),
                                                    assetManager->LoadText("shaders/simple.frag"));

            shaderProgramSimple->AddUniform("mvp");
            shaderProgramSimple->AddUniform("tex");
            shaderProgramSimple->AddUniform("diffuseColor");
            shaderProgramSimple->AddUniform("hasTexture");

        }
        catch (std::exception &e) {
            Log::E() << "Init exception: " << e.what();
        }

        camera.Move(glm::vec3(0, 20, -50.f));

        testTrans.Rotate(Transform::Up(), glm::radians(90.0f), Transform::Space::World);
        testTrans.Translate(glm::vec3(0,0,-70));
         
        glEnable(GL_DEPTH_TEST); // glDepthFunc(GL_LESS);
        glEnable(GL_CULL_FACE);
        glClearColor(0.5f, 0.5f, 1.f, 1.f);
        
        return true;
    }

    void OnResize(int w, int h) {
    	glViewport(0, 0, w, h);
    	width = w;
    	height = h;
        camera.Perspective(glm::radians(60.0f), static_cast<float>(width) / height, 1.0f, 1000.0f);
        //camera.Orthographic(0.0f, static_cast<float>(w), static_cast<float>(h), 0.0f, 1.0f, 1000.0f);
    }

    void OnStep(const Time& time) {
        //MVP
        glm::mat4& projection = camera.GetProjectionMatrix();
        glm::mat4& view       = camera.GetViewMatrix();

        rot = glm::rotate(rot, time.DeltaTime(), glm::vec3(0, 1, 0));
        glm::mat4 modelM = glm::scale(glm::translate(glm::mat4(), glm::vec3(0,0,0)), glm::vec3(0.1f, 0.1f, 0.1f)); //glm::mat4_cast(rot); //testTrans.GetMatrix(); //        
        
        //glm::mat4 mvp = projection * view * model;

        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderProgramSimple->Use();
        shaderProgramSimple->SetUniform("tex", 0);

        for (Mesh* m : model->GetMeshes()) {
            // transform
            glm::mat4 mvp = projection * view * modelM * m->matrix;
            shaderProgramSimple->SetUniform("mvp", mvp);
            
            // material
            Material* mat = materialDatabase[m->materialID];
            shaderProgramSimple->SetUniform("diffuseColor", mat->diffuseColor); 
            shaderProgramSimple->SetUniform("hasTexture", mat->hasTexture);
            if (mat->hasTexture)
                mat->texture->Bind(GL_TEXTURE0);
            
            // draw
            m->renderer.Render();
        }


        //glUseProgram(0);
        //glFlush();
        //assert(glGetError() == GL_NO_ERROR);
    }

    void Destroy() {
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        for (Mesh* m : model->GetMeshes()) {
            m->renderer.Destroy();
        }

        delete shaderProgramSimple;
    }
};
