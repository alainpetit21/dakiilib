#ifndef _SOUND_WIN32_H
#define _SOUND_WIN32_H

//TODO
//#include <vorbis/codec.h>
//#include <SDL/SDL_audio.h>
//#include <SDL/SDL_mixer.h>

class CMusic;
class CMngSound
{
public:

	CMngSound();
	virtual ~CMngSound();

	virtual void	Init			(u32 p_nbChannel, u32 p_freq, u32 p_bitdepth);
	virtual void	Exit			(void);

	void*			NewSound		(u32 p_size);
	void			FreeSound		(void *p_pSound);
	void			LockSound		(void *p_pSound, u16 **p_o_pLock1, u32 *p_o_pSizeLock1);
	void			UnlockSound		(void *p_pSound, u16 *p_o_pLock1, u32 p_o_pSizeLock1);
	void			PlaySound		(void *p_pSound, bool p_isLooping);

	virtual	void	Manage			(unsigned long p_nTimeTick);

	bool			IsOn			(void)			{return m_isOn;};
	virtual void	PauseSystem		(void){};
	virtual void	ResumeSystem	(void){};

	virtual bool	IsInLock		(void){return false;};
	bool			IsPlaying		(void *p_pSound);
	virtual s32		GetPosPlaying	(void *p_pSound);
	virtual void	SetPosPlaying	(void *p_pSound, s32 p_pos);

	virtual void	SetUserFetchSound(void (*p_userFetchSound)(u32)){m_userFetchSound= p_userFetchSound;};

	void (*m_userFetchSound)(u32);

	bool				m_isOn;
	int					m_nVolume;

	CMusic				*m_pMusic;
	static CMngSound	*singletonInstance;
};

inline CMngSound*
GetMngSound(void)
{
	return CMngSound::singletonInstance? CMngSound::singletonInstance : new CMngSound;
}

#endif // _SOUND_WIN32_H
