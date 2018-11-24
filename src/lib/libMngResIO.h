#ifndef _MNG_IMAGE_IO_H_
#define _MNG_IMAGE_IO_H_

#include "libTypes.h"
//#include "GL/GL.h"

typedef struct tagTMapImage
{
	char	strFilename[256];
	void*	pDataImage;
//	GLuint	nIdGL;
	u32		nWidth;
	u32		nHeight;
	u32		cpt;
}TMapImage;

typedef struct tagTMapSound
{
	char	strFilename[256];
	void*	pDataSound;
	u32		cpt;
}TMapSound;

class CMngImageIO
{
public:
	enum {
		VideoMem	= 0,
		TextureMem	= 1,
		SystemMem	= 2,
	};

	CMngImageIO();
	~CMngImageIO();

	void	Open(const char *p_filename, void** p_o_arData, u32* p_o_width, u32* p_o_height, bool p_bTexture);
	bool	Copy(void *p_dataSrc, void** p_o_arData, u32* p_o_width, u32* p_o_height);
	void	Close(void **p_data);
	void	ReallocBuffer(int p_size);

	TMapImage	*m_mapImage;
	int			m_nSizeBuffer;
	int			m_nCptMap;

	static CMngImageIO* singletonInstance;
};

class CMngSoundIO
{
public:
	CMngSoundIO();
	~CMngSoundIO();

	void	Open(const char *p_filename, void** p_o_arData);
	bool	Copy(void *p_dataSrc, void** p_o_arData);
	void	Close(void **p_data);
	void	ReallocBuffer(int p_size);

	TMapSound	*m_mapSound;
	int			m_nSizeBuffer;
	int			m_nCptMap;

	static CMngSoundIO* singletonInstance;
};

static inline CMngImageIO*
GetMngImageIO(void)
{
	return CMngImageIO::singletonInstance? CMngImageIO::singletonInstance : new CMngImageIO;
}

static inline CMngSoundIO*
GetMngSoundIO(void)
{
	return CMngSoundIO::singletonInstance? CMngSoundIO::singletonInstance : new CMngSoundIO;
}

#endif // _MNG_IMAGE_IO_H_
