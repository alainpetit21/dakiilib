#ifndef _LABEL_H_
#define _LABEL_H_

#include "dfcPhysObj.h"
#include "dfcFont.h"

class CLabel : public CPhysObj
{
public :
	CLabel();
	virtual ~CLabel();
	IMPLEMENT_RTTI(Label, PhysObj);

	virtual void LoadInstance(const char* p_dataSource);

	virtual void Manage(u32 p_nTimeTick);
	virtual void Update(void);
	virtual u32 GetWidth(void)	{return m_right - m_left;};
	virtual u32 GetHeight(void)	{return m_bottom - m_top;};

	virtual void AppendText(const char *p_pText);
	void AppendText(const s32 p_nId);
	void AppendText(const int p_nId)	{AppendText((s32)p_nId);}
	void AppendText(const char p_pText);

	void SetOffsetBegin(u32 p_nOffset){m_nOffsetBegin= p_nOffset;};
	int GetOffsetBegin(){return m_nOffsetBegin;};
	int	GetNbCharInFirstLine();

	char* GetText()	{return m_text;};
	void Format(const char *p_formatString, ...);
	void SetText(const s32 p_nId);
	void SetText(const char *p_pText);
	void SetFont(CFont *p_pFont){m_pFont= p_pFont;};

	bool	IsEmpty(void) {return m_nLen<=1;};

	s32		m_id;
	char	*m_text;
	int		m_nLen;
	CFont	*m_pFont;
	u32		m_nOffsetBegin;


	s32		m_left;
	s32		m_top;
	s32		m_right;
	s32		m_bottom;
	bool	m_isMultiline;
	bool	m_mode;
	bool	m_overflownInY;
};

//Label qui apparait letter by letter
class CLabelGradual	: public CLabel
{
public:
	CLabelGradual();
	virtual ~CLabelGradual();
	IMPLEMENT_RTTI(LabelGradual, Label);

	virtual void LoadInstance(char* p_dataSource);
	virtual void Manage(u32 p_nTimeTick);

	virtual void AppendText(const char *p_pText);

	void Clear();
	void SetSpeed(float p_nInc)	{m_nInc= p_nInc;};
	bool IsDone();

	char	*m_textWhole;
	float	m_nCpt;
	float	m_nInc;

};



#endif // _LABEL_H_
