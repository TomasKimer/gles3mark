

#pragma once

#ifdef _WIN32
#include <GL/glew.h>
//#include <GL/GL.h>
//#pragma comment(lib, "glew32.lib")
//#pragma comment(lib, "opengl32.lib")
#else
//#include <GLES3/gl3.h>
#include "gl3stub_egl.h"
#endif