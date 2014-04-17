
#pragma once

#include <vector>
#include <stdexcept>

#ifdef _WIN32
#define KTX_OPENGL 1
#else
#define KTX_OPENGL_ES2 1 //ES3 ?
#endif
#include <ktx.h>

#include "glhelper.h"
#include "log.h"

// http://docs.unity3d.com/Documentation/ScriptReference/Texture.html
// Reflective Shader - cubemap

class Texture {
protected:    
    enum class FilterMode {
        Point,
        Bilinear,
        Trilinear    
    };

    enum class WrapMode {
        Repeat,
        Clamp,    
    };

    //enum class Dimension {}; // 1,2,3d, array
    
    int width, height;

    static bool isPowerOfTwo(unsigned x) {
        return (x != 0) && ((x & (x - 1)) == 0);
    }
};


class GLTexture : public Texture {
    GLuint textureObject;
    GLenum target;

public:
    GLTexture() {
        glGenTextures(1, &textureObject);    
    }

    ~GLTexture() {
        glDeleteTextures(1, &textureObject);    
    }

    void FromKTXdata(const std::vector<char>& ktxData) {
    	GLenum ktxGLerror;
        GLboolean ktxIsMipmapped;
        KTX_dimensions ktxDimension;

        KTX_error_code ktxError = ktxLoadTextureM(&ktxData[0], ktxData.size(), &textureObject, &target,
                                                  &ktxDimension, &ktxIsMipmapped, &ktxGLerror, 0, nullptr);

        if (ktxError != KTX_SUCCESS && ktxGLerror != GL_NO_ERROR)
        	throw std::runtime_error("Failed to load texture with libktx.");

        if (target != GL_TEXTURE_2D)
        	throw std::runtime_error("Unsupported texture format.");

        GLint minFilter = GL_LINEAR;
        if (ktxIsMipmapped)
        	minFilter = GL_LINEAR_MIPMAP_LINEAR; // trilinear filtering

        glTexParameteri(target, GL_TEXTURE_MIN_FILTER, minFilter);
        glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Log::V() << "KTX texture loaded: " << ktxDimension.width << "x" << ktxDimension.height
        		 << "x" << ktxDimension.depth << ", mipmapped: " << (ktxIsMipmapped ? "true" : "false");
    }

    void FromBitmapData(const std::vector<char>& rawData, int width, int height) {
    	// glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    	// glGenSamplers, glBindSampler, glSamplerParameter, glDeleteSamplers
        // glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGB8_ETC2, width, height, 0, rawData.size(), &rawData[0]);  // SRGB8_ETC2, RGBA8_ETC2_EAC, SRGBA8_ETC2_EAC
    	target = GL_TEXTURE_2D;

    	glBindTexture(target, textureObject);
    	glTexImage2D(target, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, reinterpret_cast<const GLvoid*>(&rawData[0])); // GL_BGR not in ES -> swizzles?

    	GLint minFilter = GL_LINEAR;
    	if (isPowerOfTwo(width) && isPowerOfTwo(height)) { // GL_INVALID_OPERATION if lvl 0 w&h is not power of 2 or is compressed internal format
    		glGenerateMipmap(target);
    		minFilter = GL_LINEAR_MIPMAP_LINEAR;
    	}

    	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, minFilter);
    	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    void Bind(GLenum textureUnit = GL_TEXTURE0) {
        glActiveTexture(textureUnit);
        glBindTexture(target, textureObject);
    }
};
