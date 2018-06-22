#ifndef _FONT_H_
#define _FONT_H_

#include "libMngGraphic.h"
#include "dfcObj.h"

class CFont : public CObj
{
public :
	CFont();
	virtual ~CFont();
	IMPLEMENT_RTTI(Font, Obj);

	//CObject Functions
	virtual void LoadTGA(const char* p_dataSource, int p_idxDest);
	virtual void LoadInstance(const char* p_dataSource);
	virtual void Manage(u32 p_timeTick);
	virtual void Update(void);

	u32 GetTextWidth(const char* p_text);
	u32 GetLetterWidth(const char p_char)	{return m_arLetters[int(p_char)].nWidth;};
	u32 GetLetterHeight(const char p_char)	{return m_arLetters[int(p_char)].nHeight;};

	//CFont functions
	void PutS(int posX, int posY, char *buffer);
	static CFont* Find(const char* p_filenameSource);

	static CDString	*m_pAllFontSource[255];
	static CFont	*m_pAllFontInstance[255];
	static u32		m_nbFontInstalled;

	TRuntimeFrame	m_arLetters[255];
	u32				m_nbLetters;
};

#endif //_FONT_H_
