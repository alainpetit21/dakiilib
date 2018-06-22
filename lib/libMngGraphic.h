#ifndef _RENDER_H_
#define _RENDER_H_

#include "libTypes.h"

#if defined(_WIN32DX7)
	#include "./win32DX7/libMngGraphic_win32dx7.h"
#elif defined(_WIN32OPENGL)
	#include "./win32OpenGL/libMngGraphic_win32OpenGL.h"
#elif defined(_GP2X)
	#include "./gp2x/libMngGraphic_gp2x.h"
#elif defined(_LINUX)
	#include "./linux/libMngGraphic_linux.h"
#endif

#endif // _RENDER_H_
