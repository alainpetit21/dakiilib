#include "lib.h"
#include "libMngIO.h"
#include "libMacros.h"

CMngIO* CMngIO::singletonInstance= 0;

CMngIO::CMngIO(void)
{
	ASSERT2(!singletonInstance, "Another instance of CMngIO is already instanced");

	singletonInstance= this;
}

io
CMngIO::Open(const char *p_filename)
{
io ret= (io)fopen(p_filename, "rb");

	if(!ret){
		char temp[128];
		sprintf(temp, "Cannot Open File: %s", p_filename);
		ERROR_MESSAGE(temp);
	}

	return ret;
}

io
CMngIO::OpenNoCheck(const char *p_filename)
{
	return (io)fopen(p_filename, "rb");
}

u32
CMngIO::GetSize(io p_ioObject)
{
u32	oldPos= ftell((FILE*)p_ioObject);
u32 ret=0;

	fseek((FILE*)p_ioObject, 0, 2);
	ret= ftell((FILE*)p_ioObject);
	fseek((FILE*)p_ioObject, oldPos, 0);
	return ret;
}

void
CMngIO::Read(io p_ioObject, void* p_buffer, u32 p_len)
{
u32 dwRead= fread(p_buffer, 1, p_len, (FILE*)p_ioObject);

	ASSERT2(dwRead == p_len, "Can't Read File");
}

void
CMngIO::Write(io p_ioObject, const void* p_buffer, u32 p_len)
{
u32 dwWritten= fwrite(p_buffer, 1, p_len, (FILE*)p_ioObject);

	ASSERT2(dwWritten == p_len, "Can't Write File");
}

void
CMngIO::Close(io p_ioObject)
{
	fclose((FILE*)p_ioObject);
}
