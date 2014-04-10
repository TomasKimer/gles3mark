
#pragma once

#include <vector>

#include "glinclude.h"


class GLTexture {
    GLuint textureObject;

public:
    GLTexture() {
        glGenTextures(1, &textureObject);    
    }

    GLTexture(const std::vector<char>& rawData, int width, int height): GLTexture() {
        FromRawData(rawData, width, height);        
    }

    ~GLTexture() {
        glDeleteTextures(1, &textureObject);    
    }

    void FromRawData(const std::vector<char>& rawData, int width, int height) {
        //glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
        glBindTexture(GL_TEXTURE_2D, textureObject);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, reinterpret_cast<const GLvoid*>(&rawData[0])); // GL_BGR not in ES
        glBindTexture(GL_TEXTURE_2D, 0);    
    }

    void Bind() {
        glBindTexture(GL_TEXTURE_2D, textureObject);    
    }
};
