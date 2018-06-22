#ifndef _TYPESS_H_
#define _TYPESS_H_

#if defined(_WIN32DX7)
	#include "./win32DX7/libTypes_win32dx7.h"
#elif defined(_WIN32DX9)
	#include "./linux/libTypes_win32dx9.h"
#elif defined(_WIN32OPENGL)
	#include "./win32OpenGL/libTypes_win32OpenGL.h"
#elif defined(_GP2X)
	#include "./gp2x/libTypes_gp2x.h"
#elif defined(_LINUX)
	#include "./linux/libTypes_linux.h"
#endif


typedef unsigned char			byte;
typedef unsigned short			word;
typedef unsigned long			dword;
typedef unsigned char			uchar;
typedef unsigned short			ushort;
typedef unsigned int			uint;
typedef unsigned long			ulong;
typedef const unsigned char		cuchar;
typedef const unsigned short	cushort;
typedef const unsigned int		cuint;
typedef const unsigned long		culong;
typedef const char				cchar;
typedef const short				cshort;
typedef const int				cint;
typedef const long				clong;
typedef const unsigned char		cbyte;
typedef const unsigned short	cword;
typedef const unsigned long		cdword;

typedef char		i8;
typedef short		i16;
typedef long		i32;
typedef char		s8;
typedef short		s16;
typedef long		s32;
typedef byte		ui8;
typedef word		ui16;
typedef dword		ui32;
typedef byte		u8;
typedef word		u16;
typedef dword		u32;
typedef cchar		ci8;
typedef cshort		ci16;
typedef clong		ci32;
typedef cchar		cs8;
typedef cshort		cs16;
typedef clong		cs32;
typedef cbyte		cui8;
typedef cword		cui16;
typedef cdword		cui32;
typedef cbyte		cu8;
typedef cword		cu16;
typedef cdword		cu32;

typedef volatile u8		vu8;
typedef volatile u16	vu16;
typedef volatile u32	vu32;

#endif //#define _TYPESS_H_
