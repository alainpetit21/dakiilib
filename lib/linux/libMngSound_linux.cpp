#include <SDL.h>

#include "lib.h"
#include "libTypes.h"
#include "libMacros.h"
#include "libMngSound.h"
#include "libMngInput.h"

#include "dfcMusic.h"

CMngSound* CMngSound::singletonInstance= 0;


CMngSound::CMngSound():
m_pMusic(0)
{
	ASSERT2(!singletonInstance, "Another instance of CMngSound is already instanced");
	singletonInstance= this;
}


CMngSound::~CMngSound()
{
	singletonInstance= 0;
}


void
CMngSound::Init(u32 p_nbChannel, u32 p_freq, u32 p_bitdepth)
{
	VERIFY3(SDL_Init(SDL_INIT_AUDIO) >= 0, "Cannot SDL_Init Audio : %s\n", SDL_GetError() );
	VERIFY3(Mix_OpenAudio(p_freq, AUDIO_S16, p_nbChannel, 512) != -1, "Cannot Mix_OpenAudio : %s\n", SDL_GetError() );
	Mix_AllocateChannels(MIX_CHANNELS);

	Mix_Volume(-1, m_nVolume= MIX_MAX_VOLUME);
	Mix_VolumeMusic(MIX_MAX_VOLUME);
	m_isOn= true;
}


void
CMngSound::Exit(void)
{
	Mix_CloseAudio();
	m_isOn= false;
}


void*
CMngSound::NewSound(u32 p_size)
{
	if(!m_isOn)
		return 0;

	return 0;
}


void
CMngSound::FreeSound(void *p_pSound)
{
}


void
CMngSound::LockSound(void *p_pSound, u16 **p_o_pLock1, u32 *p_o_pSizeLock1)
{
}


void
CMngSound::UnlockSound(void *p_pSound, u16 *p_o_pLock1, u32 p_o_pSizeLock1)
{
}


void
CMngSound::PlaySound(void *p_pSound, bool p_isLooping)
{
	if(!p_pSound)
		return ;
}


bool
CMngSound::IsPlaying(void *p_pSound)
{
	if(!p_pSound)
		return 0;

	return false;
}


s32
CMngSound::GetPosPlaying(void *p_pSound)
{
	return 0;
}


void
CMngSound::SetPosPlaying(void *p_pSound, s32 p_pos)
{
}


void
CMngSound::Manage(u32 p_nTimeTick)
{
	if(m_pMusic)
		m_pMusic->Manage(p_nTimeTick);
}
