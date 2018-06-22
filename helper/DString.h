#ifndef _DSTRING_H_
#define _DSTRING_H_

class CDString
{
public:
	CDString(void);
	CDString(const CDString& p_str);
	CDString(const char* p_str);
	virtual ~CDString(void);

	void operator=(const char* p_str);
	void operator=(const CDString& p_str)	{operator=(p_str.m_arBuffer);};
	void operator+=(const char* p_str);
	void operator+=(const CDString& p_str)	{operator+=(p_str.m_arBuffer);};
	int operator==(const char* p_str);
	int operator==(const CDString& p_str)	{return operator==(p_str.m_arBuffer);};
	char operator[](int p_nIdx)				{return GetAt(p_nIdx);};

	void		Empty(void)					{delete []m_arBuffer; m_arBuffer= 0; m_nBufferLenght= 0;}
	void		Format(const char* p_strFormat, int p_nInt);
	void		Format(const char* p_strFormat, const char* p_strString);
	CDString	Mid(int p_nFirst, int p_nCount);
	int			Find(const char* p_strRequest, int p_nStart);
	char*		GetBuffer(int p_nNewSize);
	void		ReleaseBuffer(int p_nNewSize);

	int			GetLength(void)	const	{return m_nBufferLenght;};
	char		GetAt(int p_nIdx)		{return m_arBuffer[p_nIdx];};
	int			IsEmpty(void)			{return m_nBufferLenght==0;};

	int			SetLength(int p_nIdx)	{return m_arBuffer[m_nBufferLenght= p_nIdx]= 0;};

	char 	*m_arBuffer;
	int		m_nBufferLenght;
};

#endif // _DSTRING_H_
