

#include <vector>

#include "glhelper.h"
#include <cassert>


static std::string GLString(GLenum name) {
    return std::string(reinterpret_cast<const char*>(glGetString(name)));
}

void GLHelper::GLInfo() {
    Log::Msg("GL_VENDOR: "                   + GLString(GL_VENDOR                  ));
    Log::Msg("GL_RENDERER: "                 + GLString(GL_RENDERER                ));
    Log::Msg("GL_VERSION: "                  + GLString(GL_VERSION                 ));
    Log::Msg("GL_SHADING_LANGUAGE_VERSION: " + GLString(GL_SHADING_LANGUAGE_VERSION));
    //Log::Msg("GL_EXTENSIONS: " + GLString(GL_EXTENSIONS));  glGetStringi

    // glGet
    // https://www.khronos.org/opengles/sdk/docs/man/xhtml/glGet.xml
    // http://www.opengl.org/sdk/docs/man/html/glGet.xhtml
}


std::string GLHelper::getGlErrorString(GLenum error) {
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
