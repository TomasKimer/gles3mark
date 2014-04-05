

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


struct VboEntry {
    glm::vec3 pos;    // x, y, z;		// souradnice
    glm::vec3 normal; //float nx, ny, nz;	// normala
//    float tx, ty, tz;	// tangenta
    glm::vec2 texcoord; //float u, v;			// textura
};


class GLTest {

    GLuint VBO, EBO, VAO;
    GLuint VS, FS, Prog;
    GLuint texture;
    GLuint positionAttrib, normalAttrib, tcAttrib;
    GLuint mvpUniform, textureUniform;

    int width, height;
    Mesh* mesh;

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
            tgaData = assetManager->LoadTGA("textures/chair2.tga", &tgaWidth, &tgaHeight);            
            
            AssimpModelImporter* modelImporter = new AssimpModelImporter(*assetManager);
            mesh = modelImporter->Import("chairs.3ds");
            //mesh->MakeTestMesh();           


            GLHelper::GLInfo();

            VS = GLHelper::compileShader(GL_VERTEX_SHADER, assetManager->LoadText("shaders/simple.vert"));
            FS = GLHelper::compileShader(GL_FRAGMENT_SHADER, assetManager->LoadText("shaders/simple.frag"));

            Prog = GLHelper::linkShader({ VS, FS });
        }
        catch (std::exception &e) {
            Log::Msg(std::string("Init exception: ") + e.what(), Log::Severity::Error);
        }

        positionAttrib = glGetAttribLocation(Prog, "vertexPosition");
        normalAttrib = glGetAttribLocation(Prog, "vertexNormal");
        tcAttrib = glGetAttribLocation(Prog, "vertexTextureCoord");
        
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
        glGenVertexArrays(1, &VAO);  // TODO VAO + UBO pro kazdy mesh?
        glBindVertexArray(VAO);

        // Copy house to graphics card
        glGenBuffers(1, &VBO);      // TODO neni treba ukladat? - staci ulozit jen VAO -- stejna promenna na vsechny docasne buffery
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        GL_CHECK( glBufferData(GL_ARRAY_BUFFER, mesh->vertices.size() * sizeof(glm::vec3) * 2 + mesh->texCoords.size() * sizeof(glm::vec2), nullptr, GL_STATIC_DRAW) );

        for (size_t i = 0; i < mesh->vertices.size(); ++i) {
            GL_CHECK( glBufferSubData(GL_ARRAY_BUFFER, i * sizeof(VboEntry) + offsetof(VboEntry, pos)     , sizeof(glm::vec3), &mesh->vertices [i]) );
            GL_CHECK( glBufferSubData(GL_ARRAY_BUFFER, i * sizeof(VboEntry) + offsetof(VboEntry, normal)  , sizeof(glm::vec3), &mesh->normals  [i]) );
            GL_CHECK( glBufferSubData(GL_ARRAY_BUFFER, i * sizeof(VboEntry) + offsetof(VboEntry, texcoord), sizeof(glm::vec2), &mesh->texCoords[i]) );
        }
        
        // setup vertex shader attribute bindings (connecting current <position and tc> buffer to associated 'in' variable in vertex shader)
        GL_CHECK( glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(VboEntry), (GLvoid*)offsetof(VboEntry, pos     )) );
        GL_CHECK( glVertexAttribPointer(normalAttrib  , 3, GL_FLOAT, GL_FALSE, sizeof(VboEntry), (GLvoid*)offsetof(VboEntry, normal  )) );
        GL_CHECK( glVertexAttribPointer(tcAttrib      , 2, GL_FLOAT, GL_FALSE, sizeof(VboEntry), (GLvoid*)offsetof(VboEntry, texcoord)) );
        

        // setup vbo for index buffer
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        GL_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->faces.size()*sizeof(glm::ivec3), &mesh->faces[0].x, GL_STATIC_DRAW));

        // enable vertex buffers
        glEnableVertexAttribArray(positionAttrib);
        glEnableVertexAttribArray(normalAttrib);
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
        GL_CHECK( glDrawElements(GL_TRIANGLES, mesh->faces.size() * 3, GL_UNSIGNED_INT, nullptr) );  // sizeof house / sizeof house[0]
        glBindVertexArray(0);

        //glUseProgram(0);
        //glFlush();
        assert(glGetError() == GL_NO_ERROR);
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
