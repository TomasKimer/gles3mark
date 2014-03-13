

#pragma once

#ifdef _WIN32

#include <GL/glew.h>
#pragma comment(lib, "glew32.lib")
#include <GL/GL.h>
#pragma comment(lib, "opengl32.lib")
#else
//#include <GLES3/gl3.h> // TODO glimports.h
#include "gl3stub.h"
#endif

#include "log.h"
#include "assetmanager.h"

#ifndef NULL
#define NULL 0
#endif

#include <string>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


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
    
public:
    GLTest() { rx = 0.0f; ry = 0.0f; pz = -70.0f; wheel = 5.0f; }
    ~GLTest() {    }
    
    bool OnInit(AssetManager* assetManager) {
        int tgaWidth, tgaHeight;
        char* tgaData = assetManager->LoadTGA("textures/tiles.tga", &tgaWidth, &tgaHeight);
        Log::Stream() << tgaWidth << ", " << tgaHeight;




        // init gl here
#ifdef _WIN32
        GLenum error = glewInit();
        if(error != GLEW_OK)
            Log::Msg("!!!! GLEW ERROR !!!!");
        VS = compileShader(GL_VERTEX_SHADER, "#version 130\n in vec3 position; in vec2 tc; uniform mat4 mvp; out vec2 coord; void main() { gl_Position = mvp*vec4(position,1); coord = tc; }");
        FS = compileShader(GL_FRAGMENT_SHADER, "#version 130\n in vec2 coord; uniform sampler2D tex; out vec4 fragColor; void main() { fragColor = texture(tex, coord); }");
#else
        if (!gl3stubInit())
        	Log::Msg("!!!! GL stub init failed !!!!");
        VS = compileShader(GL_VERTEX_SHADER, "#version 300 es\n in vec3 position; in vec2 tc; uniform mat4 mvp; out vec2 coord; void main() { gl_Position = mvp*vec4(position,1); coord = tc; }");
        FS = compileShader(GL_FRAGMENT_SHADER, "#version 300 es\n in vec2 coord; uniform sampler2D tex; out vec4 fragColor; void main() { fragColor = texture(tex, coord); }");
#endif

        Prog = linkShader(2, VS, FS);

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
        glm::mat4 projection;

        float aspect = (float)width / (float)height;

        projection = glm::perspective(0.785f, aspect, 1.0f, 1000.0f);

        glm::mat4 mvp = glm::rotate(
            glm::rotate(
            glm::translate(
            projection,
            glm::vec3(0, 0, pz)
            ),
            ry, glm::vec3(1, 0, 0)
            ),
            rx, glm::vec3(0, 1, 0)
            );

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

        glDrawElements(GL_TRIANGLES, sizeof(house) / sizeof(*house), GL_UNSIGNED_BYTE, NULL);

        ry += 0.1f;
        rx += 0.1f;

    }

    GLuint compileShader(const GLenum type, const char * source) {
        GLuint shader = glCreateShader(type);
        //if (shader == 0) throw GL_Exception("glCreateShader failed");

        glShaderSource(shader, 1, &source, NULL);
        glCompileShader(shader);

        //std::cout << getShaderInfoLog(shader);

        int compileStatus;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);

        if (compileStatus == GL_FALSE)
            Log::Msg("shader compilation failed"); //throw std::runtime_error("shader compilation failed");
        else
            Log::Msg("shader compilation ok");

        return shader;
    }


    GLuint linkShader(size_t count, ...) {
        // Create program object
        GLuint program = glCreateProgram();
        //if (program == 0) throw GL_Exception("glCreateProgram failed");

        // Attach arguments
        va_list args;
        va_start(args, count);
        for (size_t i = 0; i < count; ++i) {
            glAttachShader(program, va_arg(args, GLuint));
        }
        va_end(args);

        // Link program and check for errors
        glLinkProgram(program);
        //std::cout << getProgramInfoLog(program);

        int linkStatus;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);

        if (linkStatus == GL_FALSE)
            Log::Msg("shader linking failed");  //throw std::runtime_error("shader linking failed");
        else
            Log::Msg("shader linking ok");

        return program;
    }

    //std::string getInfoLog(GLuint id, PFNGLGETSHADERIVPROC getLen, PFNGLGETSHADERINFOLOGPROC getLog);
    //// Info logs contain errors and warnings from shader compilation and linking
    //inline std::string getShaderInfoLog(GLuint shader)
    //{
    //    assert(glIsShader(shader));
    //    return getInfoLog(shader, glGetShaderiv, glGetShaderInfoLog);
    //}
    //inline std::string getProgramInfoLog(GLuint program)
    //{
    //    assert(glIsProgram(program));
    //    return getInfoLog(program, glGetProgramiv, glGetProgramInfoLog);
    //}
};
