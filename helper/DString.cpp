#include "DString.h"
#include "lib.h"
#include "libMacros.h"


CDString::CDString(void):
m_arBuffer(0),
m_nBufferLenght(0)
{
}

CDString::CDString(const char* p_str):
m_arBuffer(0),
m_nBufferLenght(strlen(p_str))
{
	if(!m_nBufferLenght)
		return;

	m_arBuffer= new char[m_nBufferLenght+10];
	memcpy(m_arBuffer, p_str, m_nBufferLenght+1);
}

CDString::CDString(const CDString& p_str):
m_arBuffer(0),
m_nBufferLenght(p_str.m_nBufferLenght)
{
	if(!m_nBufferLenght)
		return;

	m_arBuffer= new char[m_nBufferLenght+10];
	memcpy(m_arBuffer, p_str.m_arBuffer, m_nBufferLenght+1);
}

CDString::~CDString(void)
{
	m_nBufferLenght= 0;
	SAFE_DELETE_ARRAY(m_arBuffer);
}

void
CDString::operator=(const char* p_str)
{
	if(m_nBufferLenght)
		delete []m_arBuffer;

	m_nBufferLenght= strlen(p_str);
	m_arBuffer= new char[m_nBufferLenght+10];
	memcpy(m_arBuffer, p_str, m_nBufferLenght+1);
}

void
CDString::operator+=(const char* p_str)
{
char *temp= new char [m_nBufferLenght + strlen(p_str)+10];

	if(m_nBufferLenght)
		memcpy(temp, m_arBuffer, m_nBufferLenght);

	memcpy(&temp[m_nBufferLenght], p_str, strlen(p_str)+1);

	if(m_nBufferLenght)
		delete []m_arBuffer;

	m_nBufferLenght= strlen(temp);
	m_arBuffer= temp;
}

int
CDString::operator==(const char* p_str)
{
	return strcmp(m_arBuffer, p_str) == 0;
}

void
CDString::Format(const char* p_strFormat, const char* p_strString)
{
	char temp[256];
	sprintf(temp, p_strFormat, p_strString);

	operator=(temp);
}

void
CDString::Format(const char* p_strFormat, int p_nInt)
{
	char temp[256];
	sprintf(temp, p_strFormat, p_nInt);

	operator=(temp);
}

CDString
CDString::Mid(int p_nFirst, int p_nCount)
{
	// out-of-bounds requests return sensible things
	if(p_nFirst < 0)
		p_nFirst= 0;
	if(p_nCount < 0)
		p_nCount= 0;

	if((p_nFirst + p_nCount) > m_nBufferLenght)
		p_nCount= m_nBufferLenght - p_nFirst;
	if(p_nFirst > m_nBufferLenght)
		p_nCount = 0;

	// optimize case of returning entire string
	if((p_nFirst == 0) && ((p_nFirst + p_nCount) == m_nBufferLenght))
		return *this;

	CDString dest;
	dest= &m_arBuffer[p_nFirst];
	dest.SetLength(p_nCount);

	return dest.m_arBuffer;
}

int
CDString::Find(const char* p_strRequest, int p_nStart)
{
	if(!p_strRequest)
		return -1;

	int iPos;
	int iLen	= m_nBufferLenght;
	int iSLen	= strlen(p_strRequest);

	if(iSLen > (iLen - p_nStart))
		return -1;  // is substring longer than my string, return -1

	for(iPos= p_nStart; iPos < (iLen - iSLen); ++iPos)
		if(Mid(iPos, iSLen) == p_strRequest)
			return iPos;

	return -1;
}

char*
CDString::GetBuffer(int p_nNewSize)
{
	if(m_nBufferLenght > p_nNewSize)
		return m_arBuffer;

	char *temp= new char [p_nNewSize+10];

	if(m_nBufferLenght)
		memcpy(temp, m_arBuffer, m_nBufferLenght+1);

	if(m_nBufferLenght)
		delete []m_arBuffer;

	m_nBufferLenght	= p_nNewSize;
	m_arBuffer		= temp;

	return m_arBuffer;
}

void
CDString::ReleaseBuffer(int p_nNewSize)
{
	SetLength(p_nNewSize);
}
