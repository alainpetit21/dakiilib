#include "lib.h"
#include "libMacros.h"
#include "libMngSound.h"
#include "dfcMusic.h"
#include "dfcContext.h"


void musicFinished(void)
{
	if(GetMngSound()->m_pMusic)
		GetMngSound()->m_pMusic->m_isFinished= true;

	if(!GetMngSound()->m_pMusic->m_isLooping){
		GetMngSound()->m_pMusic->m_isPlaying= false;
	}
	GetMngSound()->m_pMusic= 0;
}

CMusic::~CMusic(void)
{
	if(!GetMngSound()->IsOn())
		return;

	Stop();
	Mix_FreeMusic(m_pMusic);
}

void
CMusic::LoadOGG(const char* p_dataSource)
{
CMngSound *mngSound= GetMngSound();

	if(!mngSound->IsOn())
		return;

	m_strFilename= p_dataSource;
	m_pMusic = Mix_LoadMUS(m_strFilename.m_arBuffer);
	if(u32(m_pMusic) == 0x0){
		m_strFilename= "resInput/";
		m_strFilename+= p_dataSource;
		m_pMusic = Mix_LoadMUS(m_strFilename.m_arBuffer);
		if(u32(m_pMusic) == 0x0){
			m_strFilename= "../resInput/";
			m_strFilename+= p_dataSource;
			m_pMusic = Mix_LoadMUS(m_strFilename.m_arBuffer);
			if(u32(m_pMusic) == 0x0){
				m_strFilename= "../../resInput/";
				m_strFilename+= p_dataSource;
				m_pMusic = Mix_LoadMUS(m_strFilename.m_arBuffer);
				ASSERT3(m_pMusic, "Couldn't find Sound Ogg: %s\n", p_dataSource);
			}
		}
	}
}


void
CMusic::LoadInstance(const char* p_dataSource)
{
	CObj::LoadInstance(p_dataSource);


	m_pXml->FindElem("FILENAME");
	LoadOGG((m_pXml->GetData()).m_arBuffer);
	m_pXml->FindElem("LOOP");
	SetLoop(atoi((m_pXml->GetData()).m_arBuffer));
	if(m_pXml->FindElem("LOOP_OFFSET"))
		m_nOffsetLooping= atoi((m_pXml->GetData()).m_arBuffer);

	SAFE_DELETE(m_pXml);
}

void
CMusic::Manage(u32 p_nTimeTick)
{
CMngSound *mngSound= GetMngSound();

	if(!mngSound->IsOn())
		return;

	if(m_isPlaying && m_isFinished){
		if(m_isLooping){
			Mix_RewindMusic();
			Play();
		}
	}
}

void
CMusic::Update(void)
{
}

void
CMusic::Play(void)
{
	if(!GetMngSound()->IsOn())
		return;

	m_isPlaying	= true;
	m_isFinished= false;
	Mix_PlayMusic(m_pMusic, 0);

	GetMngSound()->m_pMusic= this;
	Mix_HookMusicFinished(musicFinished);
}

void
CMusic::Stop(void)
{
	m_isPlaying= false;
	Mix_HaltMusic();
}
