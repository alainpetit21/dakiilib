#include "lib.h"
#include "libTypes.h"
#include "libMacros.h"
#include "libMng3D.h"
#include "libMngGraphic.h"
#include "libMngInput.h"
#include "libMngIO.h"
#include "libMngResIO.h"
#include "libMngSound.h"
#include "dfcApp.h"

string		g_appName	=0;
string		g_className	=0;
int			g_isActive;

extern CApp	*g_TheApp;
int			g_bQuit;

PROFILER_DEC

char gTargetResPrefix[]= "linux";

int
LibMain(const char *p_strGame)
{
CMng3D		*pMng3D			= GetMng3D();
CMngGraphic	*pMngRenderer	= GetMngGraphic();
CMngInput	*pMngInput 		= GetMngInput();
CMngSound	*pMngSound 		= GetMngSound();
CMngImageIO	*pMngImageIO	= GetMngImageIO();
CMngSoundIO	*pMngSoundIO	= GetMngSoundIO();
CMngIO		*pMngIO			= GetMngIO();

	PROFILER_INIT;

	g_TheApp->LoadInstance(g_TheApp->m_pRes);

	SDL_Init(SDL_INIT_VIDEO);

	pMngInput->Init(g_TheApp->m_isFS);

	if(g_TheApp->m_hasAudio)
		pMngSound->Init(g_TheApp->m_nbChannelSound, g_TheApp->m_freqSound, g_TheApp->m_bitdepthSound);

	pMngRenderer->Init(0, g_TheApp->m_width, g_TheApp->m_height, g_TheApp->m_width, g_TheApp->m_height, g_TheApp->m_isFS, g_TheApp->m_pixelDepth);

	pMng3D->Init(0, g_TheApp->m_width, g_TheApp->m_height, g_TheApp->m_width, g_TheApp->m_height, g_TheApp->m_isFS, g_TheApp->m_pixelDepth);
	pMng3D->BufferGeo(g_TheApp->m_nBufferGeoVertex, g_TheApp->m_nBufferGeoIndexes, g_TheApp->m_nBufferGeoTexCoord);

	g_TheApp->m_fontStd= new CFont();
	g_TheApp->m_fontStd->LoadInstance("Font8.font");
	g_TheApp->OnPostCreate();
	g_isActive= true;

	while(System_Run())
		g_TheApp->OnFrame();

	delete g_TheApp;
	delete pMngImageIO;
	delete pMngSoundIO;

	pMngRenderer->Exit();
	pMng3D->Exit();
	pMngInput->Exit();

	if(g_TheApp->m_hasAudio)
		pMngSound->Exit();

	SDL_Quit();

	delete pMng3D;
	delete pMngRenderer;
	delete pMngInput;
	delete pMngSound;
	delete pMngIO;

	return 0;
}

bool
System_Run(void)
{
SDL_Event event;

	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			case SDL_MOUSEBUTTONDOWN:
				GetMngInput()->SetMouseBouton(event.button.button - 1, true);
				break;
			case SDL_MOUSEBUTTONUP:
				GetMngInput()->SetMouseBouton(event.button.button - 1, false);
				break;
			case SDL_QUIT:
                g_bQuit= true;
				break;
		}
	}

	if(g_bQuit)
		return false;

	return true;
}

bool
System_IsActive(void)
{
	return g_isActive;
}

void
System_Quit(void)
{
	g_bQuit= 1;
}
