#ifdef _LINUX
//TODO#include <ogg/ogg.h>
//TODO#include <vorbis/vorbisfile.h>
//TODO#include <vorbis/codec.h>
#elif defined(_GP2X)
#include <tremor/ogg.h>
#include <tremor/ivorbisfile.h>
#include <tremor/ivorbiscodec.h>
#endif

//#include <SDL/SDL_image.h>

#include "lib.h"
#include "libMacros.h"
#include "libMngResIO.h"
#include "libMngGraphic.h"
#include "libMngSound.h"
//#include "TGALoader.h"

#pragma pack(push, forImage, 1)
typedef struct tagTRgb
{
	unsigned char	r;
	unsigned char	g;
	unsigned char	b;
}TRgb;
#pragma pack(pop, forImage)

extern char gTargetResPrefix[];

CMngImageIO* CMngImageIO::singletonInstance= 0;

CMngImageIO::CMngImageIO():
m_nCptMap(0)
{
	m_mapImage= new TMapImage[m_nSizeBuffer= 16];
	memset(m_mapImage, 0, sizeof(TMapImage)*m_nSizeBuffer);

	CMngImageIO::singletonInstance= this;
}

CMngImageIO::~CMngImageIO()
{/*TODO
	for(int i= 0; i < m_nCptMap; ++i){
		if(m_mapImage[i].pDataImage){
			if(--m_mapImage[i].cpt == 0){
				SDL_FreeSurface((SDL_Surface*)m_mapImage[i].pDataImage);
				m_mapImage[i].pDataImage= 0;
			}
		}
	}

	delete []m_mapImage;*/
}

bool
CMngImageIO::Copy(void *p_dataSrc, void** p_o_arData, u32* p_o_width, u32* p_o_height)
{
	for(int i= 0; i < m_nCptMap; ++i){
		if(m_mapImage[i].pDataImage == p_dataSrc){
			*p_o_width	= m_mapImage[i].nWidth;
			*p_o_height	= m_mapImage[i].nHeight;
			*p_o_arData	= m_mapImage[i].pDataImage;
			++m_mapImage[i].cpt;
			return true;
		}
	}

	return false;
}

void
CMngImageIO::Open(const char *p_filename, void** p_o_arData, u32* p_o_width, u32* p_o_height, bool p_nMode)
{
//	printf("CMngImageIO::Open-> %s\n", p_filename);

	if((m_nCptMap+1) == m_nSizeBuffer)
		ReallocBuffer(m_nSizeBuffer+(m_nSizeBuffer>>1));

	for(int i= 0; i < m_nCptMap; ++i){
		if(strcmp(m_mapImage[i].strFilename, p_filename) == 0){
			*p_o_width	= m_mapImage[i].nWidth;
			*p_o_height	= m_mapImage[i].nHeight;
			*p_o_arData	= m_mapImage[i].pDataImage;
			++m_mapImage[i].cpt;
			return;
		}
	}

//	printf("%s\n", p_filename);
	CDString	tmpFilename= p_filename;

	tmpFilename= "resInput/";
	tmpFilename+= gTargetResPrefix;
	tmpFilename+= "/";
	tmpFilename+= p_filename;
//TODO	SDL_Surface	*tmpLoading= IMG_Load(tmpFilename.m_arBuffer);
/*TODO
	if(u32(tmpLoading) == 0x0){
		tmpFilename= "resInput/";
		tmpFilename+= p_filename;
		tmpLoading = IMG_Load(tmpFilename.m_arBuffer);
		ASSERT3(tmpLoading, "Couldn't find Image: %s\n", tmpFilename.m_arBuffer);
	}

	s32 colorkey= SDL_MapRGB(tmpLoading->format, 0xFF, 0x00, 0xFF);
	SDL_SetColorKey(tmpLoading, SDL_SRCCOLORKEY, colorkey);
	*p_o_arData	= m_mapImage[m_nCptMap].pDataImage	= tmpLoading;
	*p_o_width	= m_mapImage[m_nCptMap].nWidth		= tmpLoading->w;
	*p_o_height	= m_mapImage[m_nCptMap].nHeight		= tmpLoading->h;

	strcpy(m_mapImage[m_nCptMap].strFilename, p_filename);
	++m_mapImage[m_nCptMap].cpt;
	++m_nCptMap;
//	printf("<-CMngImageIO::Open\n");

//	glGenTextures(1, nIdGL)
*/
}

void
CMngImageIO::Close(void** p_data)
{/*TODO
	for(int i= 0; i < m_nCptMap; ++i){
		if(m_mapImage[i].pDataImage == *p_data){
			if(--m_mapImage[i].cpt == 0){
				SDL_FreeSurface((SDL_Surface*)*p_data);
				m_mapImage[i].pDataImage= 0;
				m_mapImage[i].strFilename[0]= 0;
			}

			return;
		}
	}*/
}

void
CMngImageIO::ReallocBuffer(int p_size)
{
	TMapImage *temp= new TMapImage[p_size];
	memset(temp, 0, sizeof(TMapImage)*p_size);

	memcpy(temp, m_mapImage, sizeof(TMapImage)*m_nSizeBuffer);
	delete []m_mapImage;

	m_mapImage= temp;
	m_nSizeBuffer= p_size;
}

//////////////
//Sound

CMngSoundIO* CMngSoundIO::singletonInstance= 0;

CMngSoundIO::CMngSoundIO():
m_nCptMap(0)
{
	m_mapSound= new TMapSound[m_nSizeBuffer= 16];
	memset(m_mapSound, 0, sizeof(TMapSound)*m_nSizeBuffer);

	CMngSoundIO::singletonInstance= this;
}

CMngSoundIO::~CMngSoundIO()
{
	for(int i= 0; i < m_nCptMap; ++i){
		if(m_mapSound[i].pDataSound){
			if(--m_mapSound[i].cpt == 0){
				GetMngSound()->FreeSound(&m_mapSound[i].pDataSound);
				m_mapSound[i].pDataSound= 0;
				m_mapSound[i].strFilename[0]= 0;
			}
		}
	}

	delete []m_mapSound;
}

bool
CMngSoundIO::Copy(void *p_dataSrc, void** p_o_arData)
{
	for(int i= 0; i < m_nCptMap; ++i){
		if(m_mapSound[i].pDataSound == p_dataSrc){
			*p_o_arData		= m_mapSound[i].pDataSound;
			++m_mapSound[i].cpt;
			return true;
		}
	}

	return false;
}


void
CMngSoundIO::Open(const char *p_filename, void** p_o_arData)
{
CMngSound		*mngSound= GetMngSound();

	*p_o_arData		= 0;

	if(!mngSound->IsOn())
		return;

	if((m_nCptMap+1) == m_nSizeBuffer)
		ReallocBuffer(m_nSizeBuffer+(m_nSizeBuffer>>1));

	for(int i= 0; i < m_nCptMap; ++i){
		if(strcmp(m_mapSound[i].strFilename, p_filename) == 0){
			*p_o_arData		= m_mapSound[i].pDataSound;
			++m_mapSound[i].cpt;
			return;
		}
	}

	//Opening
	{
	CDString		filename= p_filename;
/*TODO
		strcpy(m_mapSound[m_nCptMap].strFilename, p_filename);
		*p_o_arData= Mix_LoadWAV(filename.m_arBuffer);
		if(u32(*p_o_arData) == 0x0){
			filename= "resInput/";
			filename+= p_filename;
			*p_o_arData= Mix_LoadWAV(filename.m_arBuffer);
			if(u32(*p_o_arData) == 0x0){
				filename= "../resInput/";
				filename+= p_filename;
				*p_o_arData= Mix_LoadWAV(filename.m_arBuffer);
				if(u32(*p_o_arData) == 0x0){
					filename= "../../resInput/";
					filename+= p_filename;
					*p_o_arData= Mix_LoadWAV(filename.m_arBuffer);
					if(u32(*p_o_arData) == 0x0){
						char temp[256];

						sprintf(temp, "Couldn't find Sound wav: %s", p_filename);
						ERROR_MESSAGE(temp);
					}
				}
			}
		}

		strcpy(m_mapSound[m_nCptMap].strFilename, filename.m_arBuffer);*/
	}

	m_mapSound[m_nCptMap].pDataSound= *p_o_arData;
	++m_mapSound[m_nCptMap].cpt;
	++m_nCptMap;
}

void
CMngSoundIO::Close(void** p_data)
{/*TODO
	for(int i= 0; i < m_nCptMap; ++i){
		if(m_mapSound[i].pDataSound == *p_data){
			if(--m_mapSound[i].cpt == 0){
				Mix_FreeChunk((Mix_Chunk*)m_mapSound[i].pDataSound);

				m_mapSound[i].pDataSound	= 0;
				m_mapSound[i].strFilename[0]= 0;
			}

			return;
		}
	}*/
}

void
CMngSoundIO::ReallocBuffer(int p_size)
{
	TMapSound *temp= new TMapSound[p_size];
	memset(temp, 0, sizeof(TMapSound)*p_size);

	memcpy(temp, m_mapSound, sizeof(TMapSound)*m_nSizeBuffer);
	delete []m_mapSound;

	m_mapSound= temp;
	m_nSizeBuffer= p_size;
}
