#ifndef _APP_H_
#define _APP_H_

#include "libTypes.h"
#include "dfcContext.h"
#include "dfcDictionary.h"
#include "dfcFont.h"

class CApp : public CObj
{
public:
	CApp(const char *p_res);
	virtual ~CApp();
	IMPLEMENT_RTTI(App, Obj);

	//From CApp
	virtual void OnPostCreate(void);
	virtual void LoadInstance(const char* p_dataSource);

	void		Init(u32 m_nbContext);

	void		RegisterContext(u32 p_id, CContext* p_pCtx);
	void		SetCurContext(s32 p_id);
	CContext*	GetContext(u32);
	CContext*	GetCurContext(void);
	u32			GetCurContextID(void);

	virtual void		OnFrame(void);

	u32			GetFrameTick(void);
	void		CalcCpuMHZ(void);


	CFont		*m_fontStd;
	CDictionary	m_dictionary;
	u32			m_width;
	u32			m_height;
	bool		m_isFS;
	u32			m_pixelDepth;
	u32			m_nbChannelSound;
	u32			m_freqSound;
	u32			m_bitdepthSound;
	bool		m_hasAudio;
	u32			m_nStackContextSize;
	u32			*m_stackContext;
	s32			m_nCptSwitchContext;

	bool		m_has3D;
	u32			m_nBufferGeoVertex;
	u32			m_nBufferGeoIndexes;
	u32			m_nBufferGeoTexCoord;

	u32			m_nPhysicSizeBlocMap;

	#if defined(_WIN32DX7) || defined(_WIN32OPENGL)
	__int64		m_clockFreq;
	__int64		m_lastFrameTick;
	__int64		m_thisFrameTick;
	#elif defined(_LINUX)
	double		m_lastFrameTick;
	double		m_thisFrameTick;
	#elif defined(_GP2X)
	float		m_fCpuMHZ1000;

	unsigned long long m_lastFrameTick;
	unsigned long long m_thisFrameTick;
	#endif

private:
	CContext	*m_pCurContext;
};

extern CApp		*g_TheApp;

#endif // _APP_H_
