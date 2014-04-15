

#pragma once

#include "log.h"
#include "assetmanager.h"
#include "modelimporter_assimp.h"
#include "camera.h"
#include "glhelper.h"
#include "time.h"
#include "gltexture.h"
#include "glmeshrenderer.h"

#include <string>
#include <vector>
#include <cassert>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>


class GLTest {

    GLuint VS, FS, Prog;
    GLuint mvpUniform, textureUniform;

    int width, height;

    std::vector<GLMeshRenderer> meshRenderers;
    GLTexture texture;

    glm::quat rot;
    Transform testTrans;
    
public:
    Camera camera;
    
    GLTest()  {}
    ~GLTest() {}
        
    bool OnInit(AssetManager* assetManager) {
        try {            
            texture.FromKTXdata(assetManager->LoadContents("textures/chair512_etc2rgb_mip_slowperc.ktx"));
            
            AssimpModelImporter* modelImporter = new AssimpModelImporter(/* *assetManager*/);
            Model* model = modelImporter->Import(assetManager->LoadContents("models/e112.3ds"));

            

            GLHelper::GLInfo();

            VS = GLHelper::compileShader(GL_VERTEX_SHADER, assetManager->LoadText("shaders/simple.vert"));
            FS = GLHelper::compileShader(GL_FRAGMENT_SHADER, assetManager->LoadText("shaders/simple.frag"));

            Prog = GLHelper::linkShader({ VS, FS });

            meshRenderers.resize(model->GetMeshes().size());            
            for (size_t i = 0; i < model->GetMeshes().size(); ++i) {
                meshRenderers[i].Init(model->GetMeshes()[i]);            
            }
        }
        catch (std::exception &e) {
            Log::E() << "Init exception: " << e.what();
        }

        //glBindFragDataLocation(_programID, 0, "FColor");    -- http://stackoverflow.com/questions/1733838/fragment-shaders-output-variabless    
        
        mvpUniform = glGetUniformLocation(Prog, "mvp");
        textureUniform = glGetUniformLocation(Prog, "tex");

        camera.Move(glm::vec3(0, 0, -70.f));

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
        glm::mat4 model = glm::mat4_cast(rot); //testTrans.GetMatrix(); //        
        
        //glm::mat4 mvp = projection * view * model;

        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(Prog);
        //glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, glm::value_ptr(mvp));

        texture.Bind(GL_TEXTURE0);
        glUniform1i(textureUniform, 0);

        for (GLMeshRenderer &mr : meshRenderers) {
            glm::mat4 mvp = projection * view * model * mr.mesh->matrix;
            glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, glm::value_ptr(mvp));
            mr.Render();
        }

        //glUseProgram(0);
        //glFlush();
        //assert(glGetError() == GL_NO_ERROR);
    }

    void Destroy() {
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        for (GLMeshRenderer &mr : meshRenderers)
            mr.Destroy();

        glDeleteShader(Prog);
    }
};
