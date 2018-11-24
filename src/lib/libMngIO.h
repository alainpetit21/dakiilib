#ifndef _FILE_H_
#define _FILE_H_

#include "libTypes.h"

typedef void* io;

class CMngIO
{
public:
	CMngIO();

	io		Open(const char *p_filename);
	io		OpenNoCheck(const char *p_filename);
	u32		GetSize(io p_ioObject);
	void	Read(io p_ioObject, void* p_buffer, u32 p_len);
	void	Write(io p_ioObject, const void* p_buffer, u32 p_len);
	void	Close(io p_ioObject);

	static CMngIO* singletonInstance;
};

static inline CMngIO*
GetMngIO(void)
{
	return CMngIO::singletonInstance? CMngIO::singletonInstance : new CMngIO;
}

#endif // _FILE_H_
