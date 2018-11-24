#include "lib.h"
#include "libMacros.h"
#include "libMngSound.h"
#include "libMngResIO.h"
#include "dfcSound.h"
#include "dfcContext.h"

void
CSound::LoadOGG(const char* p_dataSource)
{
CMngSound	*mngSound = GetMngSound();
CMngSoundIO *pMngSoundIO = GetMngSoundIO();

	if(!mngSound->IsOn())
		return;

	m_strFilename= p_dataSource;
	pMngSoundIO->Open(p_dataSource, &m_pData);
}


CSound::CSound():
m_isPlaying(false),
m_isLooping(false),
m_isFinished(false)
{
}

CSound::~CSound()
{
}

void
CSound::LoadInstance(const char* p_dataSource)
{
	CObj::LoadInstance(p_dataSource);

	m_pXml->FindElem("FILENAME");
	LoadOGG((m_pXml->GetData()).m_arBuffer);

	SAFE_DELETE(m_pXml);
}


void
CSound::Manage(u32 p_nTimeTick)
{
	if(m_isPlaying && !Mix_Playing(m_nChannel)){
		m_isFinished= true;
		m_isPlaying= false;
	}
}

void
CSound::Update(void)
{
}


void
CSound::Play()
{
CMngSound *mngSound= GetMngSound();

	if(!mngSound->IsOn())
		return;

	m_isPlaying	= true;
	m_isFinished= false;

	m_nChannel= Mix_PlayChannel(-1, (Mix_Chunk*)m_pData, m_isLooping?-1:0);
}
