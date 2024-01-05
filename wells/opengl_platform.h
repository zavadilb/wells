#ifndef OPENGL_PLATFORM_H
#define OPENGL_PLATFORM_H
#if defined(__APPLE__)
#include <GLUT/GLUT.h>
#include <OpenGL/gl3.h>
#else
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#include <windows.h>
#endif
#include <GL/glew.h>		// must be downloaded
#include <GL/freeglut.h>	// must be downloaded unless you are on MacOS (I think)
#endif
#endif // !OPENGL_PLATFORM_H