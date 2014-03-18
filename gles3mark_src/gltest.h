

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

#include <string>
#include <vector>

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

    int value = 5;
    
    bool OnInit(AssetManager* assetManager) {
        int tgaWidth, tgaHeight;
        char* tgaData = assetManager->LoadTGA("textures/tiles.tga", &tgaWidth, &tgaHeight);

        try {

            // init gl here
#ifdef _WIN32
            GLenum error = glewInit();
            if (error != GLEW_OK)
                Log::Msg("!!!! GLEW ERROR !!!!");
            std::string shaderVer("#version 330");
#else
            if (!gl3stubInit())
                Log::Msg("!!!! GL stub init failed !!!!");
            std::string shaderVer("#version 300 es");
#endif
            VS = compileShader(GL_VERTEX_SHADER, shaderVer + assetManager->LoadText("shaders/simple-vs.glsl"));
            FS = compileShader(GL_FRAGMENT_SHADER, shaderVer + assetManager->LoadText("shaders/simple-fs.glsl"));

            Prog = linkShader({ VS, FS });

        }
        catch (std::exception &e) {
            Log::Msg(std::string("Init exception: ") + e.what());
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

        glDrawElements(GL_TRIANGLES, sizeof(house) / sizeof(*house), GL_UNSIGNED_BYTE, nullptr);  // sizeof house / sizeof house[0]

        ry += 0.1f;
        rx += 0.1f;

    }

    GLuint compileShader(GLenum type, const std::string& source) {
        GLuint shader = glCreateShader(type);
        if (shader == 0)
            throw std::runtime_error("glCreateShader failed");

        const char *c_str = source.c_str();
        glShaderSource(shader, 1, &c_str, nullptr);
        glCompileShader(shader);

        Log::Msg("Compile shader: " + getShaderInfoLog(shader));

        int compileStatus;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);

        if (compileStatus == GL_FALSE) {
            glDeleteShader(shader);
            throw std::runtime_error("shader compilation failed");
        }            
        
        return shader;
    }

    GLuint linkShader(std::initializer_list<GLuint> shaders) {  // size_t count, ...
        GLuint program = glCreateProgram();
        if (program == 0)
            throw std::runtime_error("glCreateProgram failed");

        //    // Attach arguments
        //    va_list args;
        //    va_start(args, count);
        //    for (size_t i = 0; i < count; ++i) {
        //        glAttachShader(program, va_arg(args, GLuint));
        //    }
        //    va_end(args);

        for (GLuint shader : shaders) {
            glAttachShader(program, shader);
        }

        // Link program and check for errors
        glLinkProgram(program);
        
        Log::Msg("Link shaders: " + getProgramInfoLog(program));

        int linkStatus;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);

        if (linkStatus == GL_FALSE) {
            glDeleteProgram(program);
            throw std::runtime_error("shader linking failed");
        }

        return program;
    }

    // Info logs contain errors and warnings from shader compilation and linking
    std::string getShaderInfoLog(GLuint shader) {
        //assert(glIsShader(shader));

        GLint length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        
        std::vector<GLchar> log(length);
        glGetShaderInfoLog(shader, length, nullptr, &log[0]);

        //assert(glGetError() == GL_NO_ERROR);

        return std::string(&log[0], log.size());
    }

    std::string getProgramInfoLog(GLuint program) {
        //assert(glIsProgram(program));
        
        GLint length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        
        std::vector<GLchar> log(length);
        glGetProgramInfoLog(program, length, nullptr, &log[0]);

        //assert(glGetError() == GL_NO_ERROR);

        return std::string(&log[0], log.size());
    }


    struct GL_Exception : public std::runtime_error {
        GL_Exception(const GLenum error = glGetError()) throw()
            : std::runtime_error("OpenGL: " + getGlErrorString(error)) {}
        GL_Exception(const std::string& text, const GLenum error = glGetError()) throw()
            : std::runtime_error("OpenGL: " + text + " : " + getGlErrorString(error)) {}
        
        std::string getGlErrorString(GLenum error) {
#define GLERROR(e) case e : return std::string(#e)
            switch (error) {
                GLERROR(GL_NO_ERROR);
                GLERROR(GL_INVALID_ENUM);
                GLERROR(GL_INVALID_VALUE);
                GLERROR(GL_INVALID_OPERATION);
                GLERROR(GL_INVALID_FRAMEBUFFER_OPERATION);
                GLERROR(GL_OUT_OF_MEMORY);
                default: return std::string("UNKNOWN_GL_ERROR");
            }
#undef GLERROR
        }
    };

};
