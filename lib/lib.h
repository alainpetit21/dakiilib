#ifndef _LIB_H_
#define _LIB_H_

#if defined(_WIN32DX7)
	#include "./win32DX7/lib_win32dx7.h"
#elif defined(_WIN32DX9)
	#include "./win32DX9/lib_win32dx9.h"
#elif defined(_WIN32OPENGL)
	#include "./win32OpenGL/lib_win32OpenGL.h"
#elif defined(_GP2X)
	#include "./gp2x/lib_gp2x.h"
#elif defined(_LINUX)
	#include "./linux/lib_linux.h"
#else
    #error "please define a platform"
#endif

#endif // _LIB_H_
