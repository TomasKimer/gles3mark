

#pragma once

#include <string>
#include <vector>
#include <algorithm>

#include "glinclude.h"

// glGet
// https://www.khronos.org/opengles/sdk/docs/man/xhtml/glGet.xml
// http://www.opengl.org/sdk/docs/man/html/glGet.xhtml

class GLQuery {
public:
    static std::vector<std::string> Extensions() {
        GLint count = Get<GLint>(GL_NUM_EXTENSIONS);

        std::vector<std::string> ret;
        for (int i = 0; i < count; ++i)
            ret.push_back(reinterpret_cast<const char*>(glGetStringi(GL_EXTENSIONS, i)));

        return ret;    
    }

    static bool IsExtensionSupported(const std::string& extension) {
        std::vector<std::string> extensions = Extensions();
        return (std::find(extensions.begin(), extensions.end(), extension) != extensions.end());    
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

private:
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

template <> inline
void GLQuery::GetFunc(GLenum name, std::vector<std::string>& out) {
    out[0].assign(reinterpret_cast<const char*>(glGetString(name)));
}