

#pragma once

#include "log.h"
#include "assetmanager.h"
#include "modelimporter.h"
#include "camera.h"
#include "glhelper.h"
#include "time.h"

#include <string>
#include <vector>
#include <cassert>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>


// House indices
const unsigned char house[] = {
    //Walls
    0, 1, 2,
    0, 2, 3,
    4, 5, 6,
    4, 6, 7,
    8, 9, 10,
    8, 10, 11,
    12, 13, 14,
    12, 14, 15,
    //Roof
    16, 17, 18,
    19, 20, 21,
    22, 23, 24,
    25, 26, 27
};

struct Point {
    float texcoord[2];
    float position[3];
} const houseVertices[] = {
    // Walls
    { { 0.0, 0.0 }, { -5.0, -5.0, -5.0 } },
    { { 0.0, 4.0 }, { -5.0, -5.0, 5.0 } },
    { { 4.0, 4.0 }, { 5.0, -5.0, 5.0 } },
    { { 4.0, 0.0 }, { 5.0, -5.0, -5.0 } },

    { { 0.0, 0.0 }, { -5.0, 5.0, -5.0 } },
    { { 0.0, 2.0 }, { -5.0, 5.0, 5.0 } },
    { { 2.0, 2.0 }, { 5.0, 5.0, 5.0 } },
    { { 2.0, 0.0 }, { 5.0, 5.0, -5.0 } },

    { { 0.0, 0.0 }, { -5.0, -5.0, -5.0 } },
    { { 0.0, 1.0 }, { -5.0, -5.0, 5.0 } },
    { { 1.0, 1.0 }, { -5.0, 5.0, 5.0 } },
    { { 1.0, 0.0 }, { -5.0, 5.0, -5.0 } },

    { { 0.0, 0.0 }, { 5.0, -5.0, -5.0 } },
    { { 0.0, 1.0 }, { 5.0, -5.0, 5.0 } },
    { { 1.0, 1.0 }, { 5.0, 5.0, 5.0 } },
    { { 1.0, 0.0 }, { 5.0, 5.0, -5.0 } },
    // Roof
    { { 0.0, 0.0 }, { -5.0, 5.0, -5.0 } },
    { { 1.0, 0.0 }, { 5.0, 5.0, -5.0 } },
    { { 0.0, 1.0 }, { 0.0, 11.0, 0.0 } },

    { { 0.0, 0.0 }, { 5.0, 5.0, -5.0 } },
    { { 1.0, 0.0 }, { 5.0, 5.0, 5.0 } },
    { { 0.0, 1.0 }, { 0.0, 11.0, 0.0 } },

    { { 0.0, 0.0 }, { 5.0, 5.0, 5.0 } },
    { { 0.0, 1.0 }, { -5.0, 5.0, 5.0 } },
    { { 1.0, 1.0 }, { 0.0, 11.0, 0.0 } },

    { { 0.0, 1.0 }, { -5.0, 5.0, 5.0 } },
    { { 0.0, 0.0 }, { -5.0, 5.0, -5.0 } },
    { { 1.0, 1.0 }, { 0.0, 11.0, 0.0 } }
};


class GLTest {

    GLuint VBO, EBO, VAO;
    GLuint VS, FS, Prog;
    GLuint texture;
    GLuint positionAttrib, tcAttrib, mvpUniform, textureUniform;

    int width, height;


    glm::quat rot;
    Transform testTrans;
    
public:
    Camera camera;
    
    GLTest()  {}
    ~GLTest() {}
        
    bool OnInit(AssetManager* assetManager) {
        int tgaWidth, tgaHeight;
        char* tgaData;
        try {            
            tgaData = assetManager->LoadTGA("textures/tiles.tga", &tgaWidth, &tgaHeight);            
            
            ModelImporter* modelImporter = new ModelImporter();
            ModelImporter::AssimpModel model;
            model.Load(*assetManager, "monkey.3ds");
            model.Process();


            // init gl here
            GLHelper::GLInfo();

            VS = GLHelper::compileShader(GL_VERTEX_SHADER, assetManager->LoadText("shaders/simple.vert"));
            FS = GLHelper::compileShader(GL_FRAGMENT_SHADER, assetManager->LoadText("shaders/simple.frag"));

            Prog = GLHelper::linkShader({ VS, FS });
        }
        catch (std::exception &e) {
            Log::Msg(std::string("Init exception: ") + e.what(), Log::Severity::Error);
        }

        positionAttrib = glGetAttribLocation(Prog, "position");
        tcAttrib = glGetAttribLocation(Prog, "tc");
        
        //glUseProgram(Prog);
        //glBindAttribLocation(_programID, 0, "in_Position");
        //glBindAttribLocation(_programID, 1, "in_Normal");
        //glBindAttribLocation(_programID, 2, "in_TexCoord");
        //glBindFragDataLocation(_programID, 0, "FColor");
        //glUseProgram(0);
        
        
        mvpUniform = glGetUniformLocation(Prog, "mvp");
        textureUniform = glGetUniformLocation(Prog, "tex");

        // setup VAO
        // Narozdil od VBO neukladaji data o vrcholech (pozice, normala, ...)
        // VAO ukladaji reference na VBO a nastaveni atributu.
        // VAO usnadnuji a urychluji vykreslovani. Pro vykresleni staci aktivovat VAO a ten si pamatuje veskere nastaveni.
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        // Copy house to graphics card
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(houseVertices), houseVertices, GL_STATIC_DRAW);
        // setup vertex shader attribute bindings (connecting current <position and tc> buffer to associated 'in' variable in vertex shader)
        GL_CHECK(glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Point), (void*)offsetof(Point, position)));
        GL_CHECK(glVertexAttribPointer(tcAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(Point), (void*)offsetof(Point, texcoord)));

        // setup vbo for index buffer
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        GL_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(house), house, GL_STATIC_DRAW));

        // enable vertex buffers
        glEnableVertexAttribArray(positionAttrib);
        glEnableVertexAttribArray(tcAttrib);

        // unbind VAO
        glBindVertexArray(0);


        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tgaWidth, tgaHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, tgaData);

        camera.Move(glm::vec3(0, 0, -70.f));

        testTrans.Rotate(Transform::Up(), glm::radians(90.0f), Transform::Space::World);
        testTrans.Translate(glm::vec3(0,0,-70));

        glEnable(GL_DEPTH_TEST);
        //glDepthFunc(GL_LESS);
        //glClearColor(1.f, 0.f, 1.f, 1.f);
        
        return true;
    }

    void OnResize(int w, int h) {
    	glViewport(0, 0, w, h);
    	width = w;
    	height = h;
        camera.Perspective(glm::radians(60.0f), static_cast<float>(width) / height, 1.0f, 1000.0f);
        //camera.Orthographic(0, static_cast<float>(w), 0, static_cast<float>(h), 1.0f, 1.0000f);
    }

    void OnStep(Time& time) {
        //MVP
        glm::mat4& projection = camera.GetProjectionMatrix();
        glm::mat4  view       = camera.GetViewMatrix();

        rot = glm::rotate(rot, time.DeltaTime(), glm::vec3(0, 1, 0));
        glm::mat4 model = glm::mat4_cast(rot); //testTrans.GetMatrix(); //        
        
        glm::mat4 mvp = projection * view * model;

        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(Prog);
        glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, glm::value_ptr(mvp));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(textureUniform, 0);
        
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, sizeof(house) / sizeof(house[0]), GL_UNSIGNED_BYTE, nullptr);  // sizeof house / sizeof house[0]
        glBindVertexArray(0);


        //glBindBuffer(GL_ARRAY_BUFFER, VBO);         
        //glEnableVertexAttribArray(positionAttrib);
        //GL_CHECK( glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Point), (void*)offsetof(Point, position)) );        
        //glEnableVertexAttribArray(tcAttrib);
        //glVertexAttribPointer(tcAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(Point), (void*)offsetof(Point, texcoord));
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        //glDrawElements(GL_TRIANGLES, sizeof(house) / sizeof(house[0]), GL_UNSIGNED_BYTE, nullptr);  // sizeof house / sizeof house[0]


        //glDisableVertexAttribArray(positionAttrib);
        //glDisableVertexAttribArray(tcAttrib);

        //glUseProgram(0);
        //glFlush();
        //assert(glGetError() == GL_NO_ERROR);
    }

    void Destroy() {
        glBindVertexArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glDeleteVertexArrays(1, &VAO);

        glDeleteShader(Prog);
    }
};
