#ifndef _SOUND_H_
#define _SOUND_H_

#ifdef _GP2X
#include <tremor/ogg.h>
#include <tremor/ivorbisfile.h>
#elif defined(_LINUX)
//#include <ogg/ogg.h>TODO
//#include <vorbis/vorbisfile.h>TODO
#endif
#include "dfcPhysObj.h"


class CSound : public CPhysObj
{
public :
	CSound();
	~CSound();
	IMPLEMENT_RTTI(Sound, PhysObj);

	virtual void LoadOGG(const char* p_dataSource);
	virtual void LoadInstance(const char* p_dataSource);
	virtual void Manage(u32 p_timeTick);
	virtual void Update(void);

	virtual void Play();
	virtual void Stop(void){m_isPlaying= false;};

	void SetLoop(bool p_value)	{m_isLooping= p_value;}
	bool IsPlaying(void)		{return m_isPlaying;};
	bool IsFinished(void)		{return m_isFinished;};

	u32			m_nSize;
	bool		m_isPlaying;
	bool		m_isLooping;
	bool		m_isFinished;
	CDString	m_strFilename;

	void		*m_pData;
	int			m_nChannel;
};

#endif // _SOUND_H_
