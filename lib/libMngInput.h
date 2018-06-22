#ifndef _INPUT_H_
#define _INPUT_H_


#if defined(_WIN32DX7)
	#include "./win32DX7/libMngInput_win32dx7.h"
#elif defined(_WIN32OPENGL)
	#include "./win32OpenGL/libMngInput_win32OpenGL.h"
#elif defined(_GP2X)
	#include "./gp2x/libMngInput_gp2x.h"
#elif defined(_LINUX)
	#include "./linux/libMngInput_linux.h"
#endif

#endif // _INPUT_H_
