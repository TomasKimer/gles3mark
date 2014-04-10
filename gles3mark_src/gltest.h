

#pragma once

#include "log.h"
#include "assetmanager.h"
#include "modelimporter.h"
#include "camera.h"
#include "glhelper.h"
#include "time.h"
#include "gltexture.h"

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
    GLuint mvpUniform, textureUniform;

    int width, height;
    Mesh* mesh;
    GLTexture texture;

    glm::quat rot;
    Transform testTrans;
    
public:
    Camera camera;
    
    GLTest()  {}
    ~GLTest() {}
        
    bool OnInit(AssetManager* assetManager) {
        int tgaWidth, tgaHeight, tgaBpp;
        std::vector<char> TGAdata;
        try {            
            TGAdata = assetManager->LoadTGA("textures/basemap.tga", tgaWidth, tgaHeight, tgaBpp);
            texture.FromRawData(TGAdata, tgaWidth, tgaHeight);
            
            AssimpModelImporter* modelImporter = new AssimpModelImporter(*assetManager);
            mesh = modelImporter->Import("chairs.3ds");
            //mesh->MakeTestMesh();           


            GLHelper::GLInfo();

            VS = GLHelper::compileShader(GL_VERTEX_SHADER, assetManager->LoadText("shaders/simple.vert"));
            FS = GLHelper::compileShader(GL_FRAGMENT_SHADER, assetManager->LoadText("shaders/simple.frag"));

            Prog = GLHelper::linkShader({ VS, FS });
        }
        catch (std::exception &e) {
            Log::E() << "Init exception: " << e.what();
        }


        //glBindFragDataLocation(_programID, 0, "FColor");    -- http://stackoverflow.com/questions/1733838/fragment-shaders-output-variabless    
        
        mvpUniform = glGetUniformLocation(Prog, "mvp");
        textureUniform = glGetUniformLocation(Prog, "tex");


        // Copy data to graphics card
        glGenBuffers(1, &VBO);      // TODO neni treba ukladat? - staci ulozit jen VAO -- stejna promenna na vsechny docasne buffery
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        GL_CHECK( glBufferData(GL_ARRAY_BUFFER, mesh->vertices.size() * sizeof(VboEntry), nullptr, GL_STATIC_DRAW) );

        for (size_t i = 0; i < mesh->vertices.size(); ++i) {
            GL_CHECK( glBufferSubData(GL_ARRAY_BUFFER, i * sizeof(VboEntry) + offsetof(VboEntry, pos)     , sizeof(glm::vec3), &mesh->vertices [i]) );
            GL_CHECK( glBufferSubData(GL_ARRAY_BUFFER, i * sizeof(VboEntry) + offsetof(VboEntry, normal)  , sizeof(glm::vec3), &mesh->normals  [i]) );
            GL_CHECK( glBufferSubData(GL_ARRAY_BUFFER, i * sizeof(VboEntry) + offsetof(VboEntry, texcoord), sizeof(glm::vec2), &mesh->texCoords[i]) );
        }
        
        // setup vbo for index buffer
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        GL_CHECK( glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->faces.size()*sizeof(glm::ivec3), &mesh->faces[0].x, GL_STATIC_DRAW) );


        // setup VAO
        // Narozdil od VBO neukladaji data o vrcholech (pozice, normala, ...), ale ukladaji reference na VBO a nastaveni atributu.
        // VAO usnadnuji a urychluji vykreslovani. Pro vykresleni staci aktivovat VAO a ten si pamatuje veskere nastaveni.
        // subsequent calls that change the vertex array state (glBindBuffer, glVertexAttribPointer, glEnableVertexAttribArray, and glDisableVertexAttribArray) will affect the new VAO.
        glGenVertexArrays(1, &VAO);  // TODO VAO + UBO pro kazdy mesh?
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        // setup vertex shader attribute bindings (connecting current <position and tc> buffer to associated 'in' variable in vertex shader)
        GL_CHECK( glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VboEntry), (GLvoid*)offsetof(VboEntry, pos     )) );
        GL_CHECK( glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VboEntry), (GLvoid*)offsetof(VboEntry, normal  )) );
        GL_CHECK( glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VboEntry), (GLvoid*)offsetof(VboEntry, texcoord)) );

        // enable vertex buffers
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        // unbind VAO
        glBindVertexArray(0);


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
        
        glm::mat4 mvp = projection * view * model;

        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(Prog);
        glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, glm::value_ptr(mvp));

        glActiveTexture(GL_TEXTURE0);
        texture.Bind();
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
