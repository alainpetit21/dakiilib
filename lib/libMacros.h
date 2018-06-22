#ifndef _MACROS_H_
#define _MACROS_H_

#include <cstring>
#include <cstdio>

#include "lib.h"
#include "dfc.h"
#include "dfcObj.h"
#include "DString.h"

#if defined(_WIN32DX7)
	#include "./win32DX7/libMacros_win32dx7.h"
#elif  defined(_WIN32DX9)
	#include "./win32DX7/libMacros_win32dx9.h"
#elif  defined(_WIN32OPENGL)
	#include "./win32OpenGL/libMacros_win32OpenGL.h"
#elif  defined(_GP2X)
	#include "./gp2x/libMacros_gp2x.h"
#elif defined(_LINUX)
	#include "./linux/libMacros_linux.h"
#else
    #error "please define a platform"
#endif

#undef 	SAFE_DELETE
#define SAFE_DELETE(a)			if(a){delete a; a= 0;}

#undef 	SAFE_RELEASE
#define SAFE_RELEASE(a)			if(a){(a)->Release(); (a)= 0;}

#undef	SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(a)	if(a){delete []a; a= 0;}

#undef	ARRAY_SIZE
#define ARRAY_SIZE(a)			(sizeof(a) / sizeof(a[0]))

#if defined _DEBUG && !defined(DEBUG)
	#define DEBUG
#elif defined DEBUG && !defined(_DEBUG)
	#define _DEBUG
#endif

#undef	SWAP32
#define SWAP32(a)						\
((((long)a) & 0xff000000)	>> 24)  |	\
((((long)a) & 0x00ff0000)	>> 8)	|	\
((((long)a) & 0x0000ff00)	<< 8)	|	\
((((long)a) & 0x000000ff)	<< 24)

#undef	SWAP16
#define SWAP16(a)						\
((((short)a) & 0xff00) >> 8)		|	\
((((short)a) & 0x00ff) << 8)

#undef	PI
#define PI	(3.1415926535f)

#undef	HALF_PI
#define HALF_PI	(1.570796327f)

#undef	IS_ODD
#define IS_ODD(a)	(((u8)a)&1)

#undef	IS_EVEN
#define IS_EVEN(a)	(!IS_ODD(a))

#undef	SIGN
#define SIGN(a)	((p<0)?-1:1)

#undef	DEG_TO_RAD
#define DEG_TO_RAD(a)	(p*PI/180.0f)

#undef	RAD_TO_DEG
#define RAD_TO_DEG(a)	(p*180.0f/PI)

#undef	RAND_RANGE
#define RAND_RANGE(a,b)	((a) + (rand()%((b)-((a)+1))))

#undef	MIN
#define MIN(a,b)	(((a)<(b))?(a):(b))

#undef	MAX
#define MAX(a,b)	(((a)>(b))?(a):(b))

#undef	ABS
#define ABS(a)		(((a)>0)?(a):-(a))

#define memset_Xbit(Destp, Data, Size, Bit)                 \
{															\
	u##Bit * pArray = (u##Bit*) Destp;						\
	u32 i, nbIteration = Size / (Bit>>3);					\
															\
	for(i=0; i<nbIteration ;++i)							\
	{														\
		*pArray = (u##Bit) Data;							\
		++pArray;											\
	}														\
															\
}															\

static inline bool
helper_PointIsInRect(int p_nPointX, int p_nPointY, int p_nRectX, int p_nRectY, int p_nRectW, int p_nRectH)
{
	if(p_nPointX < p_nRectX)
		return false;
	if(p_nPointY < p_nRectY)
		return false;
	if(p_nPointX > (p_nRectX + p_nRectW))
		return false;
	if(p_nPointY > (p_nRectY + p_nRectH))
		return false;

	return true;
}

static inline int
helper_PadPower2Up(int p_param)
{
	if(p_param > 4096)		return 8192;
	else if(p_param > 2048)	return 4096;
	else if(p_param > 1024)	return 2048;
	else if(p_param > 512)	return 1024;
	else if(p_param > 256)	return 512;
	else if(p_param > 128)	return 256;
	else if(p_param > 64)	return 128;
	else if(p_param > 32)	return 64;
	else if(p_param > 16)	return 32;
	else if(p_param > 8)	return 16;
	else if(p_param > 4)	return 8;
	else if(p_param > 2)	return 4;
	else if(p_param > 1)	return 2;
	else if(p_param > 0)	return 1;

	return 0;
}

static inline
int helper_atoi(const char* buffer)
{
int i;
int number;
int multiplier= 1;
int bufferSize= strlen(buffer);

	number= (buffer[bufferSize - 1] - '0');

	for(i= bufferSize - 2; i >= 0 ; i--, multiplier++)
		number+= (buffer[i] - '0') * multiplier * 10;

	return number;
}

static inline u32
helper_FindHigher16(u32 toto)
{
register u32 value= toto;

	if(value&0xFF00)	if(value&0xF000)	if(value&0xC000)	if(value&0x8000)	return 15;
																else				return 14;
											else				if(value&0x2000)	return 13;
																else				return 12;
						else				if(value&0x0C00)	if(value&0x0800)	return 11;
																else				return 10;
											else				if(value&0x0200)	return 9;
																else				return 8;
	else				if(value&0x00F0)	if(value&0x00C0)	if(value&0x0080)	return 7;
																else				return 6;
											else				if(value&0x0020)	return 5;
																else				return 4;
						else				if(value&0x000C)	if(value&0x0008)	return 3;
																else				return 2;
											else				if(value&0x0002)	return 1;
																else				return 0;
}

static inline u32
helper_FindHigher8(u32 toto)
{
register u32 value= toto;

	if(value&0x00F0)	if(value&0x00C0)	if(value&0x0080)	return 7;
											else				return 6;
						else				if(value&0x0020)	return 5;
											else				return 4;
	else				if(value&0x000C)	if(value&0x0008)	return 3;
											else				return 2;
						else				if(value&0x0002)	return 1;
											else				return 0;
}

extern char gTargetResPrefix[];

static inline void
helper_OpenScript(io &p_fichier, CMngIO *&p_mngIO, const char *p_dataSource)
{
CDString	filename;

	filename= "resScript/";
	filename+= gTargetResPrefix;
	filename+= "/";
	filename+= p_dataSource;
	p_fichier	= p_mngIO->OpenNoCheck(filename.m_arBuffer);
	if(u32(p_fichier) == 0x0){
		filename= "resScript/";
		filename+= p_dataSource;
		p_fichier	= p_mngIO->OpenNoCheck(filename.m_arBuffer);
		if(u32(p_fichier) == 0x0){
			char temp[256];

			sprintf(temp, "Couldn't find File: %s\n", p_dataSource);
			ERROR_MESSAGE(temp);
		}
	}
}

static inline void
helper_OpenRes(io &p_fichier, CMngIO *&p_mngIO, const char *p_dataSource)
{
CDString	filename;

	filename= "resInput/";
	filename+= gTargetResPrefix;
	filename+= "/";
	filename+= p_dataSource;
	p_fichier	= p_mngIO->OpenNoCheck(filename.m_arBuffer);
	if(u32(p_fichier) == 0x0){
		filename= "resInput/";
		filename+= p_dataSource;
		p_fichier	= p_mngIO->OpenNoCheck(filename.m_arBuffer);
		if(u32(p_fichier) == 0x0){
			char temp[256];

			sprintf(temp, "Couldn't find File: %s\n", p_dataSource);
			ERROR_MESSAGE(temp);
		}
	}
}

static inline unsigned short
helper_Convert32to16(unsigned long p_nPixel)
{
	unsigned char r= (unsigned char)(((p_nPixel >> 16) & 0xFF)>>3);
	unsigned char g= (unsigned char)(((p_nPixel >> 8) & 0xFF)>>2);
	unsigned char b= (unsigned char)(((p_nPixel >> 0) & 0xFF)>>3);

	return (r<<11) | (g << 5) | (b);
}


static inline int
helper_StringToObjectType(char *p_strType)
{
	if(strcmp(p_strType, "CLSID_CObj") == 0)							return CLSID_CObj;
	else if(strcmp(p_strType, "CLSID_CSceneEngine") == 0)				return CLSID_CSceneEngine;
	else if(strcmp(p_strType, "CLSID_CPhysObj") == 0)					return CLSID_CPhysObj;
	else if(strcmp(p_strType, "CLSID_CBackground") == 0)				return CLSID_CBackground;
	else if(strcmp(p_strType, "CLSID_CBackgroundLayer") == 0)			return CLSID_CBackgroundLayer;
	else if(strcmp(p_strType, "CLSID_CBackgroundLayerLooping") == 0)	return CLSID_CBackgroundLayerLooping;
	else if(strcmp(p_strType, "CLSID_CBackgroundLayerTiled") == 0)		return CLSID_CBackgroundLayerTiled;
	else if(strcmp(p_strType, "CLSID_CCamera2D") == 0)					return CLSID_CCamera2D;
	else if(strcmp(p_strType, "CLSID_CCamera3D") == 0)					return CLSID_CCamera3D;
	else if(strcmp(p_strType, "CLSID_CContext") == 0)					return CLSID_CContext;
	else if(strcmp(p_strType, "CLSID_CEntity") == 0)					return CLSID_CEntity;
	else if(strcmp(p_strType, "CLSID_CEntity3D") == 0)					return CLSID_CEntity3D;
	else if(strcmp(p_strType, "CLSID_CSprite3D") == 0)					return CLSID_CSprite3D;
	else if(strcmp(p_strType, "CLSID_CMaze3D") == 0)					return CLSID_CMaze3D;
	else if(strcmp(p_strType, "CLSID_CLabel") == 0)						return CLSID_CLabel;
	else if(strcmp(p_strType, "CLSID_CLabelGradual") == 0)				return CLSID_CLabelGradual;
	else if(strcmp(p_strType, "CLSID_CFont") == 0)						return CLSID_CFont;
	else if(strcmp(p_strType, "CLSID_CButton") == 0)					return CLSID_CButton;
	else if(strcmp(p_strType, "CLSID_CMusic") == 0)						return CLSID_CMusic;
	else if(strcmp(p_strType, "CLSID_CSound") == 0)						return CLSID_CSound;
	else																return -1;
}

static inline bool
helper_ObjectIDIs(CObj *p_obj, const char *p_strType)
{
	return(strcmp(p_obj->GetTextID(), p_strType) == 0);
}

#endif // _MACROS_H_
