#ifndef _RENDER_H
#define _RENDER_H

#include <math.h>
#include <SDL.h>

#include "libMath.h"

typedef struct tagTRuntimeFrame
{
	u32 	nWidth;
	u32 	nHeight;
	u32 	nHotSpotX;
	u32		nHotSpotY;
	void	*pData;
}TRuntimeFrame;

class CMngFx
{
public :
	CMngFx(){};
	virtual ~CMngFx(){};

	void Init(void);
	void FadeOut(void);
	bool IsFadeOut(void)	{return m_delayFadeBack;};
	void FadeIn(void);

	int		m_delayFadeBack;
};

class CMngGraphic
{
public:

	CMngGraphic();
	virtual ~CMngGraphic();

	//From CMngGraphic
	void			Init				(void* p_hwnd, long FBwidth, long FBheight, long BBwidth, long BBheight, bool IsFullSCreen, long p_pixelDepth);
	void			Exit				();
	void*			NewBitmap			(u32 p_nWidth, u32 p_nHeight, u32 p_nModeSoftware);
	void			FreeBitmap			(void **ppBitmap);
	void			DrawBitmap			(void *pDestBitmap, s32 nDx, s32 nDy, u32 nDw, u32 nDh, u8 p_angle, float p_scale, long nMode, void *pSrcBitmap);
	void			DrawPartialBitmapAlpha(void *pDestBitmap, s32 nDx, s32 nDy, u32 nDw, u32 nDh, s32 nSx, s32 nSy, u32 nSw, u32 nSh, u8 p_angle, float p_scale, long nMode, void *pSrcBitmap);
	void			DrawPartialBitmap	(void *pDestBitmap, s32 nDx, s32 nDy, u32 nDw, u32 nDh, s32 nSx, s32 nSy, u32 nSw, u32 nSh, u8 p_angle, float p_scale, long nMode, void *pSrcBitmap);
	void			DrawPartialBitmapFast(void *pDestBitmap, s32 nDx, s32 nDy, u32 nDw, u32 nDh, s32 nSx, s32 nSy, u32 nSw, u32 nSh, long nMode, void *pSrcBitmap);
	void			DrawBitmapLooping	(void *pDestBitmap, s32 nDx, s32 nDy, u32 nDw, u32 nDh, int p_xOffsetStart, int p_yOffsetStart, long nMode, void *pSrcBitmap);
	void			DrawBitmapInverted	(void *pDestBitmap, s32 nDx, s32 nDy, u32 nDw, u32 nDh, u8 p_angle, float p_scale, long nMode, void *pSrcBitmap);
	void			DrawFrame			(void *pDestBitmap, s32 nDx, s32 nDy, u8 p_angle, float p_scale, long nMode, void *p_src);
	void*			BeginScene			(void);
	void			EndScene			(void **p_bit);
	void			FillBitmap			(void *pBitmap, long color);
	void			FillRect			(void *pBitmap,s32 left, s32 top, s32 right, s32 bottom, long color);
	void			FloodFill			(void *pBitmap, s32 left, s32 top, long color);
	unsigned long*	LockSurface			(void *pBitmap);
	void			PutPixel_locked		(unsigned long *pImage, s32 nX, s32 nY, long color);
	void			DrawLine_locked		(unsigned long *pImage, s32 X0, s32 Y0, s32 X1, s32 Y1, long color);
	void			DrawCircle_locked	(unsigned long *pImage, s32 p_ptX, s32 p_ptY, u32 radius, long color);
	void			UnLockSurface		(void *pBitmap, u32 *pImage);
	void			DrawLine			(void *pBitmap, s32 X1, s32 Y1, s32 X2, s32 Y2, long color);
	unsigned char	BitmapDoClipping	(s32* nDx, s32* nDy, u32* nDw, u32* nDh, s32* nSx, s32* nSy, u32* nSw, u32* nSh);
	unsigned char	LineDoClipping		(s32* nX1, s32* nY1, s32* nX2, s32* nY2, u32* nWidth, u32* nHeight);
	void			SetColorKey			(void *ppBitmap, unsigned long p_colorKey);

	void			DrawLineOctant0		(unsigned long *pImage, long color, s32 X0, s32 Y0, int DeltaX, int DeltaY, long XDirection);
	void			DrawLineOctant1		(unsigned long *pImage, long color, s32 X0, s32 Y0, int DeltaX, int DeltaY, long XDirection);

	bool	IsFullscreen(void)	{return m_isFS;};

	CMngFx*	GetMngFx(void)				{return &m_mngFx;};
	void*	GetBB(void)					{return (void*)m_sdlScreenSurface;}
	long	GetWidth(void *p_pBitmap)	{return long(((SDL_Surface*)p_pBitmap)->w);}
	long	GetPitch(void *p_pBitmap)	{return long(((SDL_Surface*)p_pBitmap)->pitch);}
	long	GetHeight(void *p_pBitmap)	{return long(((SDL_Surface*)p_pBitmap)->h);}
	long	GetLockedPitch(void)		{return m_lastLockedPitch;};
	u32		GetPixelMode(void)			{return m_pixelFormat;};

	SDL_Window* m_sdlWindow;
	SDL_Surface *m_sdlScreenSurface;

	s32		m_isFS;
	s32		m_widthFB, m_heightFB;
	s32		m_widthBB, m_heightBB;
	s32		m_lastLockedPitch;
	u32		m_pixelFormat;

	CMngFx		m_mngFx;

	static CMngGraphic* singletonInstance;
};

inline CMngGraphic*
GetMngGraphic(void)
{
	return CMngGraphic::singletonInstance? CMngGraphic::singletonInstance : new CMngGraphic;
}
#endif // _RENDER_WIN32_H
