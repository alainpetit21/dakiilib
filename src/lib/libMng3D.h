#ifndef _3D_H_
#define _3D_H_

#if defined(_WIN32DX7)
	#include "./win32DX7/libMng3D_win32dx7.h"
#elif defined(_WIN32OPENGL)
	#include "./win32OpenGL/libMng3D_win32OpenGL.h"
#elif defined(_GP2X)
	#include "./gp2x/libMng3D_gp2x.h"
#elif defined(_LINUX)
	#include "./linux/libMng3D_linux.h"
#endif


#endif // _3D_H_
