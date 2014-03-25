

#pragma once

#include "log.h"
#include "assetmanager.h"
#include "modelimporter.h"
#include "camera.h"
#include "glhelper.h"

#include <string>
#include <vector>

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

    GLuint VBO, EBO;
    GLuint VS, FS, Prog;
    GLuint texture;
    GLuint positionAttrib, tcAttrib, mvpUniform, textureUniform;

    float rx, ry, pz;
    float wheel;
    int width, height;

    Camera camera;
    glm::quat rot;
    
public:
    
    GLTest() { rx = 0.0f; ry = 0.0f; pz = -70.0f; wheel = 5.0f; }
    
    ~GLTest() {    }

    int value = 5;

    
    bool OnInit(AssetManager* assetManager) {
        int tgaWidth, tgaHeight;
        char* tgaData;
        try {            
            tgaData = assetManager->LoadTGA("textures/tiles.tga", &tgaWidth, &tgaHeight);            
            
            ModelImporter* modelImporter = new ModelImporter();
            ModelImporter::AssimpModel model;
            model.Load(*assetManager, "e112.3ds");


            // init gl here
            GLHelper::InitGL();
            GLHelper::GLInfo();

            VS = GLHelper::compileShader(GL_VERTEX_SHADER, assetManager->LoadText("shaders/simple-vs.glsl"));
            FS = GLHelper::compileShader(GL_FRAGMENT_SHADER, assetManager->LoadText("shaders/simple-fs.glsl"));

            Prog = GLHelper::linkShader({ VS, FS });

        }
        catch (std::exception &e) {
            Log::Msg(std::string("Init exception: ") + e.what(), Log::Severity::Error);
        }

        positionAttrib = glGetAttribLocation(Prog, "position");
        tcAttrib = glGetAttribLocation(Prog, "tc");
        mvpUniform = glGetUniformLocation(Prog, "mvp");
        textureUniform = glGetUniformLocation(Prog, "tex");

        // Copy house to graphics card
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(houseVertices), houseVertices, GL_STATIC_DRAW);

        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(house), house, GL_STATIC_DRAW);

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tgaWidth, tgaHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, tgaData);


        camera.Move(glm::vec3(0, 0, -70.f));
        
        return true;
    }

    void OnResize(int w, int h) {
    	glViewport(0, 0, w, h);
    	width = w;
    	height = h;
    }

    void OnStep() {
        // draw here

        //glClearColor(1.f, 0.f, 1.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        glUseProgram(Prog);

        //MVP
        float aspect = (float)width / (float)height;
        glm::mat4 projection = glm::perspective(0.785f, aspect, 1.0f, 1000.0f);

        rot = glm::rotate(rot, 0.03f, glm::vec3(0, 1, 0));
        glm::mat4 model = glm::mat4_cast(rot);    //glm::rotate(glm::rotate(glm::mat4(), ry, glm::vec3(1, 0, 0)), rx, glm::vec3(0, 1, 0));
        glm::mat4 view = camera.GetMatrix();
        
        glm::mat4 mvp = projection * view * model; //glm::rotate( glm::rotate(  glm::translate( projection,  glm::vec3(0, 0, pz)     ),  ry, glm::vec3(1, 0, 0)   ), rx, glm::vec3(0, 1, 0)       );

        glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, glm::value_ptr(mvp));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glUniform1i(textureUniform, 0);

        glEnableVertexAttribArray(positionAttrib);
        glEnableVertexAttribArray(tcAttrib);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Point), (void*)offsetof(Point, position));
        glVertexAttribPointer(tcAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Point), (void*)offsetof(Point, texcoord));

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        glDrawElements(GL_TRIANGLES, sizeof(house) / sizeof(house[0]), GL_UNSIGNED_BYTE, nullptr);  // sizeof house / sizeof house[0]

        //ry += 0.1f;
        rx += 0.03f;

    }
};
