#ifndef _MUSIC_H_
#define _MUSIC_H_

#include <SDL.h>
#include <SDL_mixer.h>

#include "dfcSound.h"

class CMusic : public CSound
{
public :
	CMusic():CSound(), m_pMusic(0),m_nOffsetLooping(0){};
	virtual ~CMusic();
	IMPLEMENT_RTTI(Music, Sound);

	virtual void LoadOGG(const char* p_dataSource);
	virtual void LoadInstance(const char* p_dataSource);
	virtual void Manage(u32 p_timeTick);
	virtual void Update(void);

	void Play(void);
	void Stop(void);

	Mix_Music		*m_pMusic;
	int 			m_nOffsetLooping;
};

class CMusicOgg : public CMusic
{
};

class CMusicSpc : public CMusic
{
};

#endif // _MUSIC_H_
