#include "lib.h"
#include "libMacros.h"
#include "libMngGraphic.h"
#include "libMng3D.h"

CMngGraphic* CMngGraphic::singletonInstance= 0;

u32 ClearColor;

static unsigned short
Convert32to16(unsigned long p_nPixel)
{
	unsigned char r= (unsigned char)(((p_nPixel >> 16) & 0xFF)>>3);
	unsigned char g= (unsigned char)(((p_nPixel >> 8) & 0xFF)>>2);
	unsigned char b= (unsigned char)(((p_nPixel >> 0) & 0xFF)>>3);

	return (r<<11) | (g << 5) | (b);
}

CMngGraphic::CMngGraphic():
m_pBB(0),
m_pFB(0)
{
	ASSERT2(!singletonInstance, "Another instance of CMngGraphic is already instanced");

	singletonInstance= this;
}

CMngGraphic::~CMngGraphic()
{
	Exit();
	singletonInstance= 0;
};

void
CMngGraphic::Init(void* p_hwnd, long FBwidth, long FBheight, long BBwidth, long BBheight, bool bIsFullSCreen, long p_nPixelDepth)
{
	m_isFS			= bIsFullSCreen;
	m_pixelFormat	= p_nPixelDepth;
	m_widthFB		= FBwidth;
	m_heightFB		= FBheight;
	m_widthBB		= BBwidth;
	m_heightBB		= BBheight;

/*TODO
	if(!m_isFS){
		VERIFY2(m_pBB= SDL_SetVideoMode(FBwidth, FBheight, p_nPixelDepth, SDL_OPENGL), "Unable to set video mode\n");
	}else{
		VERIFY2(m_pBB= SDL_SetVideoMode(FBwidth, FBheight, p_nPixelDepth, SDL_OPENGL|SDL_FULLSCREEN), "Unable to set video mode\n");
	}

//  SDL_WM_SetCaption("Dakiisoft - ", NULL);

	m_mngFx.Init();*/
}

void
CMngGraphic::Exit()
{
}

void
CMngGraphic::SetColorKey(void *pBitmap, unsigned long p_nColorKey)
{
//TODO	SDL_SetColorKey((SDL_Surface*)pBitmap, SDL_SRCCOLORKEY, 0xFFFF00FF);
}

void*
CMngGraphic::NewBitmap(u32 p_nWidth, u32 p_nHeight, u32 p_nModeSoftware)
{/*TODO
SDL_Surface *ret;

	if(p_nModeSoftware == 0)
		ret= SDL_CreateRGBSurface(SDL_HWSURFACE, p_nWidth, p_nHeight, m_pBB->format->BitsPerPixel, m_pBB->format->Rmask, m_pBB->format->Gmask, m_pBB->format->Bmask, m_pBB->format->Amask);
	else
		ret= SDL_CreateRGBSurface(SDL_SWSURFACE, p_nWidth, p_nHeight, m_pBB->format->BitsPerPixel, m_pBB->format->Rmask, m_pBB->format->Gmask, m_pBB->format->Bmask, m_pBB->format->Amask);

	s32 colorkey= SDL_MapRGB(ret->format, 0xFF, 0x00, 0xFF);
	SDL_SetColorKey(ret, SDL_SRCCOLORKEY, colorkey);

	return (void*)ret;*/
}


void
CMngGraphic::FreeBitmap(void **p_ppBitmap)
{
	SDL_FreeSurface((SDL_Surface*)*p_ppBitmap);

	*p_ppBitmap= 0;
}

void
CMngGraphic::DrawPartialBitmapFast(void *pDestBitmap, s32 nDx, s32 nDy, u32 nDw, u32 nDh, s32 nSx, s32 nSy, u32 nSw, u32 nSh, long nMode, void *pSrcBitmap)
{
	DrawPartialBitmap(pDestBitmap, nDx, nDy, nDw, nDh, nSx, nSy, nSw, nSh, 0, 1.0f, nMode, pSrcBitmap);
}

void
CMngGraphic::DrawBitmap(void *pDestBitmap, s32 nDx, s32 nDy, u32 nDw, u32 nDh, u8 p_angle, float p_scale, long nMode, void *pSrcBitmap)
{
	if(pSrcBitmap)
		DrawPartialBitmap(pDestBitmap, nDx, nDy, nDw, nDh, 0, 0, ((SDL_Surface*)pSrcBitmap)->w, ((SDL_Surface*)pSrcBitmap)->h, p_angle, p_scale, nMode, pSrcBitmap);
}

void
CMngGraphic::DrawPartialBitmapAlpha(void *pDestBitmap, s32 nDx, s32 nDy, u32 nDw, u32 nDh, s32 nSx, s32 nSy, u32 nSw, u32 nSh, u8 p_angle, float p_scale, long nMode, void *pSrcBitmap)
{
	if(pSrcBitmap)
		DrawPartialBitmap(pDestBitmap, nDx, nDy, nDw, nDh, nSx, nSy, nSw, nSh, p_angle, p_scale, nMode, pSrcBitmap);
}

void
CMngGraphic::DrawPartialBitmap(void *pDestBitmap, s32 nDx, s32 nDy, u32 nDw, u32 nDh, s32 nSx, s32 nSy, u32 nSw, u32 nSh, u8 p_angle, float p_scale, long nMode, void *pSrcBitmap)
{
SDL_Surface *pBitmapDest= (SDL_Surface*)pDestBitmap;
SDL_Surface *pBitmapSrc	= (SDL_Surface*)pSrcBitmap;

	if(!pDestBitmap)
		pBitmapDest= m_pBB;

	if(!BitmapDoClipping(&nDx, &nDy, &nDw, &nDh, &nSx, &nSy, &nSw, &nSh))
		return;

	SDL_Rect destRect;
	SDL_Rect srcRect;

	destRect.x	= (s32)nDx;
	destRect.y	= (s32)nDy;
	destRect.w	= s32((float(p_scale*nDw)));
	destRect.h	= s32((float(p_scale*nDh)));

	srcRect.x	= (long)nSx;
	srcRect.y	= (long)nSy;
	srcRect.w	= (long)(nSw);
	srcRect.h	= (long)(nSh);

	SDL_BlitSurface(pBitmapSrc, &srcRect, pBitmapDest, &destRect);
}

void
CMngGraphic::DrawBitmapLooping(void *pDestBitmap, s32 nDx, s32 nDy, u32 nDw, u32 nDh, int p_xOffsetStart, int p_yOffsetStart, long nMode, void *pSrcBitmap)
{
SDL_Surface *pBitmapSrc	= (SDL_Surface*)pSrcBitmap;
s32		nSx= u32(p_xOffsetStart);
s32		nSy= u32(p_yOffsetStart);
s32		nSw= GetWidth(pBitmapSrc);
s32		nSh= GetHeight(pBitmapSrc);

	while(nSx < 0)
		nSx+= nSw;
	while(nSy < 0)
		nSy+= nSh;
	while(nSx > s32(nSw))
		nSx-= nSw;
	while(nSy > s32(nSh))
		nSy-= nSh;

	DrawBitmap(pDestBitmap, s32(nSx-nSw), s32(nSy-nSh), nDw, nDh, 0, 1, 0, pBitmapSrc);
	DrawBitmap(pDestBitmap, s32(nSx), s32(nSy-nSh), nDw, nDh, 0, 1, 0, pBitmapSrc);
	DrawBitmap(pDestBitmap, s32(nSx+nSw), s32(nSy-nSh), nDw, nDh, 0, 1, 0, pBitmapSrc);

	DrawBitmap(pDestBitmap, s32(nSx-nSw), s32(nSy), nDw, nDh, 0, 1, 0, pBitmapSrc);
	DrawBitmap(pDestBitmap, s32(nSx), s32(nSy), nDw, nDh, 0, 1, 0, pBitmapSrc);
	DrawBitmap(pDestBitmap, s32(nSx+nSw), s32(nSy), nDw, nDh, 0, 1, 0, pBitmapSrc);

	DrawBitmap(pDestBitmap, s32(nSx-nSw), s32(nSy+nSh), nDw, nDh, 0, 1, 0, pBitmapSrc);
	DrawBitmap(pDestBitmap, s32(nSx), s32(nSy+nSh), nDw, nDh, 0, 1, 0, pBitmapSrc);
	DrawBitmap(pDestBitmap, s32(nSx+nSw), s32(nSy+nSh), nDw, nDh, 0, 1, 0, pBitmapSrc);
}



void
CMngGraphic::DrawFrame(void *pDestBitmap, s32 nDx, s32 nDy, u8 p_angle, float p_scale, long nMode, void *p_src)
{
TRuntimeFrame	*p_srcFrame	= (TRuntimeFrame*)p_src;
SDL_Surface 	*pBitmapDest= (SDL_Surface*)pDestBitmap;
SDL_Surface 	*pBitmapSrc	= (SDL_Surface*)p_srcFrame->pData;

SDL_Rect destRect;
SDL_Rect srcRect;

	if(!pDestBitmap)
		pBitmapDest= m_pBB;

	if((p_scale != 1) && (this->m_pixelFormat == 16)){
//		DrawScaleFrame16(pDestBitmap, nDx, nDy, p_scale, nMode, p_src);
		return;
	}else if((p_angle != 0) && (this->m_pixelFormat == 16)){
//		DrawRotoScaleFrame16(pDestBitmap, nDx, nDy, p_angle, p_scale, nMode, p_src);
		return;
	}else if((p_scale != 1) && (this->m_pixelFormat == 32)){
//		DrawScaleFrame16(pDestBitmap, nDx, nDy, p_scale, nMode, p_src);
		return;
	}else if((p_angle != 0) && (this->m_pixelFormat == 32)){
//		DrawRotoScaleFrame16(pDestBitmap, nDx, nDy, p_angle, p_scale, nMode, p_src);
		return;
	}

	srcRect.x	= (long)0;
	srcRect.y	= (long)0;
	srcRect.w	= (long)(p_srcFrame->nWidth);
	srcRect.h	= (long)(p_srcFrame->nHeight);

	destRect.x	= (long)(nDx-p_srcFrame->nHotSpotX);
	destRect.y	= (long)(nDy-p_srcFrame->nHotSpotY);
	destRect.w	= (long)(p_srcFrame->nWidth);
	destRect.h	= (long)(p_srcFrame->nHeight);

	SDL_BlitSurface(pBitmapSrc, &srcRect, pBitmapDest, &destRect);
}

void*
CMngGraphic::BeginScene(void)
{
	//Debug
	FillRect(m_pBB, 0, 0, m_widthBB, m_heightBB, 0x0);

	return (void*)m_pBB;
}

void
CMngGraphic::EndScene(void** p_bit)
{
//TODO	SDL_Flip(m_pBB);
}

void
CMngGraphic::FillBitmap(void* p_pBitmap, long lColor)
{
int	width= GetWidth(p_pBitmap);
int	pitch= GetPitch(p_pBitmap)>>2;
int	height= GetHeight(p_pBitmap);

	if(m_pixelFormat == 32){
		u32 *pDstPixel	= (u32*)LockSurface(p_pBitmap);
		for(int i= 0; i < width; ++i){
			for(int j= 0; j < height; ++j){
				pDstPixel[j*pitch+i]= u32(lColor);
			}
		}
		UnLockSurface(p_pBitmap, pDstPixel);
	}
}

void
CMngGraphic::FillRect(void* pBitmap, s32 left, s32 top, s32 right, s32 bottom, long lColor)
{
SDL_Rect	rec= {left, top, right-left, bottom-top};

	SDL_FillRect((SDL_Surface*)pBitmap, &rec, lColor);
}

//stack friendly and fast floodfill algorithm
void
floodFillScanline32(u32 *p_buffer, int x, int y, int p, int w, int h, u32 newColor, u32 oldColor)
{
	if(oldColor == newColor)
		return;
	if(p_buffer[y*p+x] != oldColor)
		return;

	int y1;

	//draw current scanline from start position to the topř
	y1= y;
	while((y1 < h) && (p_buffer[y1*p+x] == oldColor)){
		p_buffer[y1*p+x] = newColor;
		y1++;
	}

	//draw current scanline from start position to the bottom
	y1= y - 1;
	while((y1 >= 0) && (p_buffer[y1*p+x] == oldColor)){
		p_buffer[y1*p+x] = newColor;
		y1--;
	}

	//test for new scanlines to the left
	y1= y;
	while((y1 < h) && (p_buffer[y1*p+x] == newColor)){
		if((x > 0) && (p_buffer[y1*p+(x - 1)] == oldColor)){
			floodFillScanline32(p_buffer, x - 1, y1, p, w, h, newColor, oldColor);
		}

		y1++;
	}

	y1 = y - 1;
	while((y1 >= 0) && (p_buffer[y1*p+x] == newColor)){
		if((x > 0) && (p_buffer[y1*p+(x - 1)] == oldColor)){
			floodFillScanline32(p_buffer, x - 1, y1, p, w, h, newColor, oldColor);
		}
		y1--;
	}

	//test for new scanlines to the right
	y1 = y;
	while((y1 < h) && (p_buffer[y1*p+x] == newColor)){
		if((x < (w - 1)) && (p_buffer[y1*p+(x + 1)] == oldColor)){
			floodFillScanline32(p_buffer, x + 1, y1, p, w, h, newColor, oldColor);
		}
		y1++;
	}

	y1 = y - 1;
	while((y1 >= 0) && (p_buffer[y1*p+x] == newColor)){
		if((x < (w - 1)) && (p_buffer[y1*p+(x + 1)] == oldColor)){
			floodFillScanline32(p_buffer, x + 1, y1, p, w, h, newColor, oldColor);
		}
		y1--;
	}
}

void
floodFillScanline8(u8 *p_buffer, int x, int y, int p, int w, int h, u8 newColor, u8 oldColor)
{
	if(oldColor == newColor)
		return;
	if(p_buffer[y*p+x] != oldColor)
		return;

	int y1;

	//draw current scanline from start position to the top
	y1= y;
	while((y1 < h) && (p_buffer[y1*p+x] == oldColor)){
		p_buffer[y1*p+x] = newColor;
		y1++;
	}

	//draw current scanline from start position to the bottom
	y1= y - 1;
	while((y1 >= 0) && (p_buffer[y1*p+x] == oldColor)){
		p_buffer[y1*p+x] = newColor;
		y1--;
	}

	//test for new scanlines to the left
	y1= y;
	while((y1 < h) && (p_buffer[y1*p+x] == newColor)){
		if((x > 0) && (p_buffer[y1*p+(x - 1)] == oldColor)){
			floodFillScanline8(p_buffer, x - 1, y1, p, w, h, newColor, oldColor);
		}

		y1++;
	}

	y1 = y - 1;
	while((y1 >= 0) && (p_buffer[y1*p+x] == newColor)){
		if((x > 0) && (p_buffer[y1*p+(x - 1)] == oldColor)){
			floodFillScanline8(p_buffer, x - 1, y1, p, w, h, newColor, oldColor);
		}
		y1--;
	}

	//test for new scanlines to the right
	y1 = y;
	while((y1 < h) && (p_buffer[y1*p+x] == newColor)){
		if((x < (w - 1)) && (p_buffer[y1*p+(x + 1)] == oldColor)){
			floodFillScanline8(p_buffer, x + 1, y1, p, w, h, newColor, oldColor);
		}
		y1++;
	}

	y1 = y - 1;
	while((y1 >= 0) && (p_buffer[y1*p+x] == newColor)){
		if((x < (w - 1)) && (p_buffer[y1*p+(x + 1)] == oldColor)){
			floodFillScanline8(p_buffer, x + 1, y1, p, w, h, newColor, oldColor);
		}
		y1--;
	}
}

void
CMngGraphic::FloodFill(void* pBitmap, s32 p_x, s32 p_y, long color)
{
int		w				= GetWidth(pBitmap);
int		h				= GetHeight(pBitmap);

	if(((SDL_Surface*)pBitmap)->format->BitsPerPixel == 32){
		u32		*pImageLocked	= (u32*)LockSurface(pBitmap);
		int		p				= GetPitch(pBitmap)>>2;
		u32		colorUnder		= pImageLocked[int(p_y*p+p_x)];

		floodFillScanline32(pImageLocked, int(p_x), int(p_y), p, w, h, color, colorUnder);
		UnLockSurface(pBitmap, pImageLocked);
	}else if(((SDL_Surface*)pBitmap)->format->BitsPerPixel == 8){
		u8	*pImageLocked	= (u8*)LockSurface(pBitmap);
		int	p				= GetPitch(pBitmap);
		u8	colorUnder		= pImageLocked[int(p_y*p+p_x)];


		floodFillScanline8(pImageLocked, int(p_x), int(p_y), p, w, h, color, colorUnder);
		UnLockSurface(pBitmap, (u32*)pImageLocked);
	}
}

u32*
CMngGraphic::LockSurface(void *p_pBitmap)
{
SDL_Surface *pSurface= (SDL_Surface*)p_pBitmap;

	if(!pSurface)
		pSurface= m_pBB;

	m_lastLockedPitch= GetPitch(pSurface);
	if(SDL_MUSTLOCK(pSurface))
		if(SDL_LockSurface(pSurface) < 0)
			return (u32*)pSurface->pixels;

	return (u32*)pSurface->pixels;
}

void
CMngGraphic::PutPixel_locked(u32 *pImage, s32 nX, s32 nY, long lColor)
{
	if(nX < 0)			return;
	if(nY < 0)			return;
	if(nX > m_widthBB)	return;
	if(nY > m_heightBB)	return;

	if(m_pixelFormat == 32){
		pImage[int(nY) * (m_lastLockedPitch>>2) + int(nX)]= lColor;
	}else if(m_pixelFormat == 16){
		u16* pImage16= (u16*)pImage;

		pImage16[int(nY) * (m_lastLockedPitch>>1) + int(nX)]= Convert32to16(lColor);
	}
}

void
CMngGraphic::DrawLineOctant0(u32 *pImage, long lColor, s32 X0, s32 Y0, int DeltaX, int DeltaY, long XDirection)
{
long DeltaYx2				= DeltaY * 2;
long DeltaYx2MinusDeltaXx2	= DeltaYx2 - (DeltaX * 2);
long ErrorTerm				= DeltaYx2 - DeltaX;


	PutPixel_locked(pImage, X0, Y0, lColor);
	while(DeltaX--){
		if(ErrorTerm >= 0){
			Y0++;
			ErrorTerm += DeltaYx2MinusDeltaXx2;
		}else{
			ErrorTerm += DeltaYx2;
		}
		X0 += XDirection;
		PutPixel_locked(pImage, X0, Y0, lColor);
	}
}

void
CMngGraphic::DrawLineOctant1(u32 *pImage, long lColor, s32 X0, s32 Y0, int DeltaX, int DeltaY, long XDirection)
{
long DeltaXx2				= (long)(DeltaX * 2);
long DeltaXx2MinusDeltaYx2	= DeltaXx2 - (long)(DeltaY * 2);
long ErrorTerm				= DeltaXx2 - (long) DeltaY;

	PutPixel_locked(pImage, X0, Y0, lColor);
	while(DeltaY--){
		if(ErrorTerm >= 0){
			X0 += XDirection;
			ErrorTerm += DeltaXx2MinusDeltaYx2;
		}else{
			ErrorTerm += DeltaXx2;
		}
		Y0++;
		PutPixel_locked(pImage, X0, Y0, lColor);
	}
}

void
CMngGraphic::DrawLine_locked(u32 *pImage, s32 X0, s32 Y0, s32 X1, s32 Y1, long lColor)
{
int	DeltaX;
int	DeltaY;
int	Temp;

	if(!pImage)
		return;

	if(Y0 > Y1){
		Temp = Y0;
		Y0 = Y1;
		Y1 = s32(Temp);

		Temp = X0;
		X0 = X1;
		X1 = s32(Temp);
	}

	DeltaX = X1 - X0;
	DeltaY = Y1 - Y0;

	if(DeltaX > 0){
		if(DeltaX > DeltaY)
			DrawLineOctant0(pImage, lColor, X0, Y0, DeltaX, DeltaY, 1);
		else
			DrawLineOctant1(pImage, lColor, X0, Y0, DeltaX, DeltaY, 1);
	}else{
		DeltaX= -DeltaX;
		if(DeltaX > DeltaY)
			DrawLineOctant0(pImage, lColor, X0, Y0, DeltaX, DeltaY, -1);
		else
			DrawLineOctant1(pImage, lColor, X0, Y0, DeltaX, DeltaY, -1);
	}
}

void
CMngGraphic::DrawLine(void* p_pBitmap, s32 X0, s32 Y0, s32 X1, s32 Y1, long lColor)
{
s32	x1= X0;
s32	y1= Y0;
s32	x2= X1;
s32	y2= Y1;
u32	w= u32(GetWidth(p_pBitmap));
u32	h= u32(GetHeight(p_pBitmap));

	if(!LineDoClipping(&x1, &y1, &x2, &y2, &w, &h))
		return;

	u32	*pLocked= LockSurface(p_pBitmap?p_pBitmap:GetBB());

	DrawLine_locked(pLocked, x1, y1, x2, y2, lColor);

	UnLockSurface(p_pBitmap?p_pBitmap:GetBB(), pLocked);
}

void
CMngGraphic::DrawCircle_locked(unsigned long *pImage, s32 p_ptX, s32 p_ptY, u32 radius, long lColor)
{
u32		i;
float	curX;
float	curY;

	for(i= 0; i < 256; ++i){
		curX= cos(i)*radius + p_ptX;
		curY= sin(i)*radius + p_ptY;
		PutPixel_locked(pImage, curX, curY, lColor);
	}
}

void
CMngGraphic::UnLockSurface(void *p_pBitmap, u32 *pImage)
{
SDL_Surface *pSurface= (SDL_Surface*)p_pBitmap;

	if(!pSurface)
		pSurface= m_pBB;

	if(SDL_MUSTLOCK(pSurface))
		SDL_UnlockSurface(pSurface);
}

unsigned char
CMngGraphic::BitmapDoClipping(s32* nDx, s32* nDy, u32* nDw, u32* nDh, s32* nSx, s32* nSy, u32* nSw, u32* nSh)
{
unsigned char ret= 0;
long sortX;
long sortY;

//	+------
//	|					Clip this part
//	|
//	|
	if (s32(*nDx+*nDw) <= 0)
		return ret;
	if (s32(*nDy+*nDh) <= 0)
		return ret;


	if(*nDx < 0){
		*nSx -= *nDx;
		*nSw += *nDx;
		*nDw += *nDx;
		*nDx = 0;
		ret|= 1;
	}
	if (*nDy < 0){
		*nSy -= *nDy;
		*nSh += *nDy;
		*nDh += *nDy;
		*nDy = 0;
		ret|= 1;
	}

//	 			|
//	 			|		Clip this part
//	 			|
//	------+
	if (*nDx >= m_widthBB)
		return 0;
	if (*nDy >= m_heightBB)
		return 0;

	sortX= (u32)((*nDx + *nDw) - m_widthBB);
	sortY= (u32)((*nDy + *nDh) - m_heightBB);

	if(sortX > 0){
		*nSw-= sortX;
		*nDw-= sortX;
	}
	if(sortY > 0){
		*nSh-= sortY;
		*nDh-= sortY;
	}
	return 1;
}

unsigned char
CMngGraphic::LineDoClipping(s32* nX1, s32* nY1, s32* nX2, s32* nY2, u32* nWidth, u32* nHeight)
{
float sortX;
float sortY;

//	+------
//	|					Clip this part
//	|
//	|
	if((*nX2 <= 0) && (*nX1 <= 0))
		return 0;
	if((*nY2 <= 0) && (*nY1 <= 0))
		return 0;

	if(*nX1 < 0)
	{
		*nWidth += *nX1;
		*nX1 = 0;
	}
	if(*nY1 < 0)
	{
		*nHeight += *nY1;
		*nY1 = 0;
	}
	if(*nX2 < 0)
	{
		*nWidth += *nX2;
		*nX2 = 0;
	}
	if(*nY2 < 0)
	{
		*nHeight += *nY2;
		*nY2 = 0;
	}

//	 	  |
//	 	  |		Clip this part
//		  |
//	------+
	if((*nX1 >= s32(*nWidth)) && (*nX2 >= s32(*nWidth)))
		return 0;
	if((*nY1 >= s32(*nHeight)) && (*nY2 >= s32(*nHeight)))
		return 0;

	sortX= *nX2 - *nWidth;
	sortY= *nY2 - *nHeight;

	if(sortX > 0)
	{
		*nWidth-= (sortX);
		*nX2-= (sortX);
	}

	if (sortY > 0)
	{
		*nHeight-= (sortY);
		*nY2-= (sortY);
	}

	sortX= *nX1 - *nWidth;
	sortY= *nY1 - *nHeight;

	if(sortX > 0)
	{
		*nWidth-= (sortX);
		*nX1-= (sortX);
	}

	if (sortY > 0)
	{
		*nHeight-= (sortY);
		*nY1-= (sortY);
	}

	return 1;
}

void
CMngFx::Init(void)
{
	m_delayFadeBack= 0;

	FadeOut();
}

void
CMngFx::FadeOut(void)
{
//CMngGraphic	*mngGraphic	= (CMngGraphic*)GetMngGraphic();
long				cpt			= 256;
long				incCpt		= 24;

//	mngGraphic->EndScene(0);

	while(cpt > 0)
		cpt-= incCpt;

	m_delayFadeBack= 3;
}

void
CMngFx::FadeIn(void)
{
//CMngGraphic *mngGraphic= (CMngGraphic*)GetMngGraphic();

	if(!m_delayFadeBack)
		return;
	if(--m_delayFadeBack)
		return;

	long	cpt		= 0;
	long	incCpt	= 24;

//	mngGraphic->EndScene(0);

	while(cpt < 256)
		cpt+= incCpt;
}
