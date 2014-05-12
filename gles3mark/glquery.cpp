
#include <algorithm>

#include "glquery.h"

std::vector<std::string> GLQuery::Extensions() {
    GLint count = Get<GLint>(GL_NUM_EXTENSIONS);

    std::vector<std::string> ret;
    for (int i = 0; i < count; ++i)
        ret.push_back(reinterpret_cast<const char*>(glGetStringi(GL_EXTENSIONS, i)));

    return ret;    
}

bool GLQuery::IsExtensionSupported(const std::string& extension) {
    std::vector<std::string> extensions = Extensions();
    return (std::find(extensions.begin(), extensions.end(), extension) != extensions.end());    
}