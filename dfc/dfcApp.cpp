#ifdef _GP2X
#include <sys/time.h>
#include <unistd.h>
#endif

#include "lib.h"
#include "libMacros.h"
#include "dfcApp.h"
#include "libMngInput.h"
#include "libMngSound.h"
#include "libMng3D.h"

PROFILER_INC

CApp		*g_TheApp;
#ifdef _GP2X
extern vu16 *gRegIO;
#endif

CApp::CApp(const char *p_pRes):
CObj(p_pRes),
m_stackContext(0),
m_nCptSwitchContext(0),
m_pCurContext(0)
{
	g_TheApp= this;
}

CApp::~CApp()
{
int i;

	for(i= 0; i < 255; ++i)
		if(CFont::m_pAllFontSource[i]){
			delete CFont::m_pAllFontSource[i];
			CFont::m_pAllFontSource[i]= 0;
		}

	for(i= 0; i < 255; ++i)
		if(CFont::m_pAllFontInstance[i]){
			delete CFont::m_pAllFontInstance[i];
			CFont::m_pAllFontInstance[i]= 0;
		}

	SAFE_DELETE_ARRAY(m_stackContext);
}

void
CApp::LoadInstance(const char* p_dataSource)
{
	CObj::LoadInstance(p_dataSource);

	m_pXml->FindElem("SCREEN_WIDTH");
	m_width= atoi((m_pXml->GetData()).m_arBuffer);
	m_pXml->FindElem("SCREEN_HEIGHT");
	m_height= atoi((m_pXml->GetData()).m_arBuffer);
	m_pXml->FindElem("SCREEN_BPP");
	m_pixelDepth= atoi((m_pXml->GetData()).m_arBuffer);
	m_pXml->FindElem("FULLSCREEN");
	m_isFS= atoi((m_pXml->GetData()).m_arBuffer);
	m_pXml->FindElem("SOUND_ON");
	m_hasAudio= atoi((m_pXml->GetData()).m_arBuffer);
	m_pXml->FindElem("SOUND_NB_CHANNEL");
	m_nbChannelSound= atoi((m_pXml->GetData()).m_arBuffer);
	m_pXml->FindElem("SOUND_HERTZ");
	m_freqSound= atoi((m_pXml->GetData()).m_arBuffer);
	m_pXml->FindElem("SOUND_BITS");
	m_bitdepthSound= atoi((m_pXml->GetData()).m_arBuffer);
	m_pXml->FindElem("3D_ON");
	m_has3D= atoi((m_pXml->GetData()).m_arBuffer);
	m_pXml->FindElem("3D_BUFFER_VERTEX");
	m_nBufferGeoVertex= atoi((m_pXml->GetData()).m_arBuffer);
	m_pXml->FindElem("3D_BUFFER_INDEXES");
	m_nBufferGeoIndexes= atoi((m_pXml->GetData()).m_arBuffer);
	m_pXml->FindElem("3D_BUFFER_TEXT_COOR");
	m_nBufferGeoTexCoord= atoi((m_pXml->GetData()).m_arBuffer);
	m_pXml->FindElem("DICTIONARY");
	m_dictionary.LoadInstance((m_pXml->GetData()).m_arBuffer);
	m_pXml->FindElem("DEFAULT_LANGUAGE");
	m_dictionary.SetCurLanguage((m_pXml->GetData()).m_arBuffer);

	if(m_pXml->FindElem("PHYSIC_MAP_BLOC_SIZE"))
		m_nPhysicSizeBlocMap=  atoi((m_pXml->GetData()).m_arBuffer);
	else
		m_nPhysicSizeBlocMap=  16;

	if(m_pXml->FindElem("CONTEXT_STACK_SIZE"))
		m_nStackContextSize=  atoi((m_pXml->GetData()).m_arBuffer);
	else
		m_nStackContextSize=  8;

	m_stackContext= new u32[m_nStackContextSize];
	SAFE_DELETE(m_pXml);
}

void
CApp::OnPostCreate(void)
{
	//Fade in
	while(GetMngGraphic()->GetMngFx()->IsFadeOut())
		GetMngGraphic()->GetMngFx()->FadeIn();
}

void
CApp::Init(u32 p_nbContext)
{
	SetNbChilds(p_nbContext);

#if defined(_WIN32DX7) || defined(_WIN32OPENGL)
	QueryPerformanceCounter((LARGE_INTEGER*)&m_thisFrameTick);
#elif defined(_LINUX)
	m_thisFrameTick= SDL_GetTicks();
#elif defined(_GP2X)
	m_thisFrameTick= gRegIO[0x0C04>>1]*1000;
	m_thisFrameTick+= (gRegIO[0x0C08>>1]*1000)>>10;
#endif
	srand(u32(m_thisFrameTick));

	m_lastFrameTick= m_thisFrameTick;
}

void
CApp::RegisterContext(u32 p_nId, CContext* p_pCtx)
{
	u32 ret= AddChild(p_pCtx, false);
	((CContext*)GetChild(ret))->Init();
	((CContext*)GetChild(ret))->m_ID= p_nId;

	WARNING2(ret == p_nId, "Your contexts are you created sequentially, you should");
}

void
CApp::SetCurContext(s32 p_nId)
{
	while(GetMngSound()->IsInLock());
	PROFILER_BEFORE;

	if(m_pCurContext){
		if(p_nId == -1){
			if((--m_nCptSwitchContext) < 0)
				m_nStackContextSize+= m_nStackContextSize;

			p_nId= m_stackContext[m_nCptSwitchContext];
		}else{
			m_stackContext[m_nCptSwitchContext]= GetCurContextID();

			if((++m_nCptSwitchContext) >= s32(m_nStackContextSize))
				m_nCptSwitchContext= 0;
		}

		m_pCurContext->Deactivate();
	}

	GetMng3D()->ResetGeo();

	m_pCurContext= (CContext*)GetChild(p_nId);
	m_pCurContext->Activate();
	PROFILER_AFTER("ChangeContext:", "\n");
}

CContext*
CApp::GetContext(u32 p_nId)
{
	return (CContext*)GetChild(p_nId);
}

CContext*
CApp::GetCurContext(void)
{
	return m_pCurContext;
}

u32
CApp::GetCurContextID(void)
{
	return m_pCurContext->m_ID;
}

void
CApp::OnFrame(void)
{
CMngInput	*pMngInput 	= GetMngInput();

#if defined(_WIN32DX7) || defined(_WIN32OPENGL)
	QueryPerformanceCounter((LARGE_INTEGER*)&m_thisFrameTick);
#elif defined(_LINUX)
	m_thisFrameTick= SDL_GetTicks();
#endif

	u32 nbMsPerFrame= GetFrameTick();

	pMngInput->Manage(nbMsPerFrame);
	GetMngSound()->Manage(nbMsPerFrame);

	if(!m_pCurContext)
		return;

	m_pCurContext->KeyManage(nbMsPerFrame);
	m_pCurContext->Manage(nbMsPerFrame);
	m_pCurContext->Update();

	m_lastFrameTick= m_thisFrameTick;
}

u32
CApp::GetFrameTick(void)
{
#if defined(_WIN32DX7) || defined(_WIN32OPENGL)
	return u32(((m_thisFrameTick - m_lastFrameTick)*1000.0f)/m_clockFreq);
#elif defined(_LINUX)
	return u32(m_thisFrameTick - m_lastFrameTick);
#elif defined(_GP2X)
	m_thisFrameTick= gRegIO[0x0C04>>1]*1000;
	m_thisFrameTick+= (gRegIO[0x0C08>>1]*1000)>>10;
	return u32(m_thisFrameTick - m_lastFrameTick);
#endif
}
