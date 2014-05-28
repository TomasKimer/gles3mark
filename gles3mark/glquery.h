/**
  *  gles3mark v1.0
  *
  *  \date   2014-05-28
  *  \author Tomas Kimer <xkimer00@stud.fit.vutbr.cz>
  */

#pragma once

#include <string>
#include <vector>

#include "glinclude.h"

/**
 * \brief Querying GL info.
 */
class GLQuery {
public:
    static std::vector<std::string> Extensions();
    static bool IsExtensionSupported(const std::string& extension);

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