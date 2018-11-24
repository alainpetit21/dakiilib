#ifndef _MNG_SOUND_H_
#define _MNG_SOUND_H_

#include "libTypes.h"

#if defined(_WIN32DX7)
	#include "./win32DX7/libMngSound_win32dx7.h"
#elif defined(_WIN32OPENGL)
	#include "./win32OpenGL/libMngSound_win32OpenGL.h"
#elif defined(_GP2X)
	#include "./gp2x/libMngSound_gp2x.h"
#elif defined(_LINUX)
	#include "./linux/libMngSound_linux.h"
#endif

#endif // _MNG_SOUND_H_
