

#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <glm/glm.hpp>
#include <type_traits>

#include "glinclude.h"
#include "log.h"

// glGet
// https://www.khronos.org/opengles/sdk/docs/man/xhtml/glGet.xml
// http://www.opengl.org/sdk/docs/man/html/glGet.xhtml

class GLQuery {
public:

    enum class Info {
    
    };


    static GLint MaxRenderBufferSize() { return Get<GLint>(GL_MAX_RENDERBUFFER_SIZE); }  // min 2048
    static GLint MaxTextureSize() { return Get<GLint>(GL_MAX_TEXTURE_SIZE); }            // min 2048
    static glm::ivec2 MaxViewportDims() {
        std::vector<GLint> ints = Get<GLint>(GL_MAX_VIEWPORT_DIMS, 2);
        return glm::ivec2(ints[0], ints[1]);
    }

    static std::string Vendor() { return GetString(GL_VENDOR); }
    static std::string Renderer() { return GetString(GL_RENDERER); }
    static std::string Version() { return GetString(GL_VERSION); }
    static std::string ShadingLanguageVersion() { return GetString(GL_SHADING_LANGUAGE_VERSION); }
    
    static std::vector<std::string> Extensions() {
        GLint count = Get<GLint>(GL_NUM_EXTENSIONS);

        std::vector<std::string> ret;
        for (int i = 0; i < count; ++i)
            ret.push_back(GetStringI(GL_EXTENSIONS, i));

        return ret;    
    }

    static bool IsExtensionSupported(const std::string& extension) {
        std::vector<std::string> extensions = Extensions();
        return (std::find(extensions.begin(), extensions.end(), extension) != extensions.end());    
    }

    static void LogInfo() {
        Log::Msg("GL_VENDOR: "                   + Vendor());
        Log::Msg("GL_RENDERER: "                 + Renderer());
        Log::Msg("GL_VERSION: "                  + Version());
        Log::Msg("GL_SHADING_LANGUAGE_VERSION: " + ShadingLanguageVersion());
    }


private:
    static std::string GetString(GLenum name) {
        return std::string(reinterpret_cast<const char*>(glGetString(name)));
    }

    static std::string GetStringI(GLenum name, GLuint index) {
        return std::string(reinterpret_cast<const char*>(glGetStringi(name, index)));
    }

    template <typename T>
    static T Get(GLenum name) {
        return Get<T>(name, 1)[0];
    }
    
    template <typename T>
    static std::vector<T> Get(GLenum name, unsigned count) {
        std::vector<T> ret(count);
        GetFunc<T>(name, ret);
        return ret;
    }

    template <typename T>
    static void GetFunc(GLenum name, std::vector<T>& out) {
    }    
};

template <> inline
void GLQuery::GetFunc(GLenum name, std::vector<GLboolean>& out) {
    glGetBooleanv(name, &out[0]);    
}

template <> inline
void GLQuery::GetFunc(GLenum name, std::vector<GLint>& out) {
    glGetIntegerv(name, &out[0]);    
}

template <> inline
void GLQuery::GetFunc(GLenum name, std::vector<GLint64>& out) {
    glGetInteger64v(name, &out[0]);    
}

template <> inline
void GLQuery::GetFunc(GLenum name, std::vector<GLfloat>& out) {
    glGetFloatv(name, &out[0]);    
}