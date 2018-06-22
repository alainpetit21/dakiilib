#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "lib.h"
#include "libMacros.h"
#include "libTypes.h"
#include "libMngGraphic.h"
#include "dfcPhysObj.h"
#include "dfcLabel.h"
#include "dfcCamera.h"
#include "dfcDictionary.h"

CLabel::CLabel():
m_text(0),
m_nLen(0),
m_pFont(0),
m_nOffsetBegin(0),
m_left(0),
m_top(0),
m_right(0),
m_bottom(0),
m_isMultiline(0),
m_mode(0),
m_overflownInY(0)
{

}

CLabel::~CLabel()
{
	SAFE_DELETE_ARRAY(m_text);
}

void
CLabel::LoadInstance(const char* p_dataSource)
{
	CObj::LoadInstance(p_dataSource);

	if(m_pXml->FindElem("TEXT"))
		SetText((m_pXml->GetData()).m_arBuffer);
	else if(m_pXml->FindElem("TEXT_ID"))
		SetText(GET_STRING((m_pXml->GetData()).m_arBuffer));

	m_pXml->FindElem("LEFT");
	m_left= atoi((m_pXml->GetData()).m_arBuffer);

	m_pXml->FindElem("TOP");
	m_top= atoi((m_pXml->GetData()).m_arBuffer);

	m_pXml->FindElem("RIGHT");
	m_right= atoi((m_pXml->GetData()).m_arBuffer);

	m_pXml->FindElem("BOTTOM");
	m_bottom= atoi((m_pXml->GetData()).m_arBuffer);

	m_pXml->FindElem("MULTILINE");
	m_isMultiline= atoi((m_pXml->GetData()).m_arBuffer);

	m_pXml->FindElem("MODE");
	m_mode= atoi((m_pXml->GetData()).m_arBuffer);

	m_pXml->FindElem("FONT_FILENAME_USED");
	SetFont(CFont::Find((m_pXml->GetData()).m_arBuffer));

	SAFE_DELETE(m_pXml);
}


void
CLabel::Manage(u32 p_nTimeTick)
{
}

void
CLabel::Update(void)
{
	if(!m_text)
		return ;

	CMngGraphic	*pRenderer	= GetMngGraphic();
	u32			textWidth	= m_pFont->GetTextWidth(&m_text[m_nOffsetBegin]);
	int			CumulLeft	= (CCamera2D::GetCurrent()->GetIntPosX() + GetIntPosX() + int(GetParentPosX()) + (pRenderer->GetWidth(pRenderer->GetBB()) / 2) + m_left);
	int			CumulRight	= (CCamera2D::GetCurrent()->GetIntPosX() + GetIntPosX() + int(GetParentPosX()) + (pRenderer->GetWidth(pRenderer->GetBB()) / 2) + m_right);
	int			CumulTop	= (CCamera2D::GetCurrent()->GetIntPosY() + GetIntPosY() + int(GetParentPosY()) + (pRenderer->GetHeight(pRenderer->GetBB()) / 2) + m_top);
	int			CumulBottom	= (CCamera2D::GetCurrent()->GetIntPosY() + GetIntPosY() + int(GetParentPosY()) + (pRenderer->GetHeight(pRenderer->GetBB()) / 2) + m_bottom);

	if(!m_bCamRelative){
		CumulLeft	= (CCamera2D::GetCurrent()->GetIntPosX() + GetIntPosX() + int(GetParentPosX()) + (pRenderer->GetWidth(pRenderer->GetBB()) / 2) + m_left);
		CumulRight	= (CCamera2D::GetCurrent()->GetIntPosX() + GetIntPosX() + int(GetParentPosX()) + (pRenderer->GetWidth(pRenderer->GetBB()) / 2) + m_right);
		CumulTop	= (CCamera2D::GetCurrent()->GetIntPosY() + GetIntPosY() + int(GetParentPosY()) + (pRenderer->GetHeight(pRenderer->GetBB()) / 2) + m_top);
		CumulBottom	= (CCamera2D::GetCurrent()->GetIntPosY() + GetIntPosY() + int(GetParentPosY()) + (pRenderer->GetHeight(pRenderer->GetBB()) / 2) + m_bottom);
	}else{
		CumulLeft	= (-CCamera2D::GetCurrent()->GetIntPosX() + GetIntPosX() + int(GetParentPosX()) + (pRenderer->GetWidth(pRenderer->GetBB()) / 2) + m_left);
		CumulRight	= (-CCamera2D::GetCurrent()->GetIntPosX() + GetIntPosX() + int(GetParentPosX()) + (pRenderer->GetWidth(pRenderer->GetBB()) / 2) + m_right);
		CumulTop	= (-CCamera2D::GetCurrent()->GetIntPosY() + GetIntPosY() + int(GetParentPosY()) + (pRenderer->GetHeight(pRenderer->GetBB()) / 2) + m_top);
		CumulBottom	= (-CCamera2D::GetCurrent()->GetIntPosY() + GetIntPosY() + int(GetParentPosY()) + (pRenderer->GetHeight(pRenderer->GetBB()) / 2) + m_bottom);
	}
	ASSERT2(m_pFont, "No font linked");

	if(!m_isShown)
		return;

	if(!m_isMultiline){

		switch(m_mode){
		case 0:
			m_pFont->PutS(CumulLeft, CumulTop, &m_text[m_nOffsetBegin]);
		break;
		case 1:
			m_pFont->PutS(CumulLeft+((CumulRight-CumulLeft)/2)-(textWidth/2), CumulTop, &m_text[m_nOffsetBegin]);
		break;
		case 2:
			m_pFont->PutS(CumulRight-textWidth, CumulTop, &m_text[m_nOffsetBegin]);
		break;
		}

	}else{
		char	temp[256]	= "";
		int		cptSrc		= 0;
		int		y			= CumulTop;

		while(m_text[m_nOffsetBegin+cptSrc]){
			m_overflownInY= false;

			if((y+m_pFont->GetLetterHeight(' ')) > uint(CumulBottom)){
				m_overflownInY= true;
				break;
			}

			int		lastCptSrc		= 0;
			int		lastCptDst		= -1;
			s32		lastWidthLine	= m_pFont->GetTextWidth(&m_text[m_nOffsetBegin+cptSrc]);
			s32		widthLine		= 0;
			int		x				= CumulLeft;
			int		cptDst			= 0;

			memset(temp, 0, 256);

			while(m_text[m_nOffsetBegin+cptSrc]){
				if(((m_text[m_nOffsetBegin+cptSrc] == '\\') && (m_text[m_nOffsetBegin+cptSrc+1] == 'n')) || (m_text[m_nOffsetBegin+cptSrc] == '\n')){
					cptSrc+=2;
					break;
				}

				temp[cptDst++]= m_text[m_nOffsetBegin+cptSrc++];

				if(m_text[m_nOffsetBegin+cptSrc - 1] == ' '){
					lastCptDst		= cptDst;
					lastCptSrc		= cptSrc;
					lastWidthLine	= widthLine;
				}

				// put new line in last good spot
				if((x + m_pFont->GetLetterWidth(temp[cptDst - 1])) > uint(CumulRight)){
					if(!lastCptSrc){
						lastCptDst		= cptDst;
						lastCptSrc		= cptSrc;
						lastWidthLine	= widthLine;
					}

					temp[lastCptDst]	= '\0';
					cptSrc				= lastCptSrc;
					break;
				}

				widthLine	+= m_pFont->GetLetterWidth(temp[cptDst - 1]);
				x			+= m_pFont->GetLetterWidth(temp[cptDst - 1]);
			}
			lastWidthLine	= widthLine;

			switch(m_mode){
			case 0:
				m_pFont->PutS(CumulLeft, y, temp);
			break;
			case 1:
				m_pFont->PutS(CumulLeft+((CumulRight-CumulLeft)/2)-(lastWidthLine/2), y, temp);
			break;
			case 2:
				m_pFont->PutS(CumulRight-lastWidthLine, y, temp);
			break;
			}

			y+= m_pFont->GetLetterHeight(' ');

		}
	}
/*
#ifdef _DEBUG
	u32 *pPixel= pRenderer->LockSurface(pRenderer->GetBB());
	u32	halfWidth=	pRenderer->GetWidth(pRenderer->GetBB())/2;
	u32	halfHeight=	pRenderer->GetHeight(pRenderer->GetBB())/2;

	pRenderer->DrawLine_locked(pPixel, s32(halfWidth+int(GetParentPosX())+GetIntPosX()+m_left),	s32(halfHeight+int(GetParentPosY())+GetIntPosY()+m_top),		s32(halfWidth+int(GetParentPosX())+GetIntPosX()+m_right),	s32(halfHeight+int(GetParentPosY())+GetIntPosY()+m_top),	0x000000FF);
	pRenderer->DrawLine_locked(pPixel, s32(halfWidth+int(GetParentPosX())+GetIntPosX()+m_left),	s32(halfHeight+int(GetParentPosY())+GetIntPosY()+m_bottom),	s32(halfWidth+int(GetParentPosX())+GetIntPosX()+m_right),	s32(halfHeight+int(GetParentPosY())+GetIntPosY()+m_bottom),	0x000000FF);
	pRenderer->DrawLine_locked(pPixel, s32(halfWidth+int(GetParentPosX())+GetIntPosX()+m_left),	s32(halfHeight+int(GetParentPosY())+GetIntPosY()+m_top),		s32(halfWidth+int(GetParentPosX())+GetIntPosX()+m_left),	s32(halfHeight+int(GetParentPosY())+GetIntPosY()+m_bottom),	0x000000FF);
	pRenderer->DrawLine_locked(pPixel, s32(halfWidth+int(GetParentPosX())+GetIntPosX()+m_right),	s32(halfHeight+int(GetParentPosY())+GetIntPosY()+m_top),		s32(halfWidth+int(GetParentPosX())+GetIntPosX()+m_right),	s32(halfHeight+int(GetParentPosY())+GetIntPosY()+m_bottom),	0x000000FF);
	pRenderer->UnLockSurface(pRenderer->GetBB(), pPixel);
#endif //_DEBUG
*/
}

void
CLabel::Format(const char *p_formatString, ...)
{
char res[128];
va_list args;

	va_start(args, p_formatString);
	vsprintf(res, p_formatString, args);
	va_end(args);

	SetText(res);
}


void
CLabel::SetText(const s32 p_nId)
{
char toto[16];

	sprintf(toto, "%d", int(p_nId));
	SetText(toto);
}

void
CLabel::SetText(const char *p_pText)
{
	if(!p_pText)
		return;

	if(m_text && strcmp(m_text, p_pText) == 0)
		return;

	SAFE_DELETE_ARRAY(m_text);

	m_text= new char[m_nLen= (strlen(p_pText)+1)];

	strcpy(m_text, p_pText);
}

void
CLabel::AppendText(const char p_pText)
{
	char carac[2]= {p_pText, 0};
	AppendText(carac);
}

void
CLabel::AppendText(const s32 p_nId)
{
char toto[16];

	sprintf(toto, "%d", int(p_nId));
	AppendText(toto);
}

void
CLabel::AppendText(const char *p_pText)
{
char *pOldText= GetText();

	if(*p_pText == '\b'){
		pOldText[strlen(pOldText)-1]= 0;
		return;
	}

	char *pNewText;
	if(pOldText){
		pNewText= new char[strlen(pOldText)+strlen(p_pText)+1];
		strcpy(pNewText, pOldText);
	}else{
		pNewText= new char[strlen(p_pText)+1];
		memset(pNewText, 0, strlen(p_pText)+1);
	}

	strcat(pNewText, p_pText);

	SetText(pNewText);

	delete [] pNewText;
}

int
CLabel::GetNbCharInFirstLine()
{
CMngGraphic	*pRenderer	= GetMngGraphic();
int			CumulLeft	= (CCamera2D::GetCurrent()->GetIntPosX() + GetIntPosX() + int(GetParentPosX()) + (pRenderer->GetWidth(pRenderer->GetBB()) / 2) + m_left);
int			CumulRight	= (CCamera2D::GetCurrent()->GetIntPosX() + GetIntPosX() + int(GetParentPosX()) + (pRenderer->GetWidth(pRenderer->GetBB()) / 2) + m_right);
int			cptSrc		= 0;
int			lastCptSrc	= 0;
int			x			= CumulLeft;

	while(m_text[m_nOffsetBegin+cptSrc]){
		if(((m_text[m_nOffsetBegin+cptSrc] == '\\') && (m_text[m_nOffsetBegin+cptSrc+1] == 'n')) || (m_text[m_nOffsetBegin+cptSrc] == '\n')){
			cptSrc+=2;
			break;
		}

		if(m_text[m_nOffsetBegin+cptSrc] == ' '){
			lastCptSrc= cptSrc;
		}

		if((x + m_pFont->GetLetterWidth(m_text[m_nOffsetBegin+cptSrc])) > uint(CumulRight)){
			if(!lastCptSrc){
				lastCptSrc= cptSrc;
			}

			cptSrc= lastCptSrc+1;
			break;
		}

		x			+= m_pFont->GetLetterWidth(m_text[m_nOffsetBegin+cptSrc]);
		++cptSrc;
	}
	return cptSrc;
}

CLabelGradual::CLabelGradual():
CLabel(),
m_nCpt(0),
m_nInc(1)
{
}

CLabelGradual::~CLabelGradual()
{
	SAFE_DELETE_ARRAY(m_textWhole);
}

void
CLabelGradual::LoadInstance(char* p_dataSource)
{
	CLabel::LoadInstance(p_dataSource);

	m_textWhole= new char[m_nLen];
	memset(m_textWhole, 0, sizeof(char)*m_nLen);

	//Switch Buffers so that m_text will countain letter by letter dst and m_textWhole the whole src
	char *temp	= m_text;
	m_text		= m_textWhole;
	m_textWhole	= temp;
}

void
CLabelGradual::Manage(u32 p_nTimeTick)
{
	if(m_overflownInY){
		SetOffsetBegin(GetOffsetBegin() + GetNbCharInFirstLine());
	}

	m_nCpt+= m_nInc;
	if(int(m_nCpt) > m_nLen)
		m_nCpt= float(m_nLen);
	else
		if((m_textWhole[int(m_nCpt-1)] == '\\') && (m_textWhole[int(m_nCpt)] == 'n'))
			m_nCpt++;

	memcpy(&m_text[0], &m_textWhole[0], sizeof(char)*int(m_nCpt));
	CLabel::Manage(p_nTimeTick);
}

bool
CLabelGradual::IsDone()
{
	return (int(m_nCpt) == m_nLen);
}

void
CLabelGradual::AppendText(const char *p_pText)
{
	CLabel::AppendText(p_pText);

	SAFE_DELETE_ARRAY(m_textWhole);

	m_textWhole= new char[m_nLen];
	memset(m_textWhole, 0, sizeof(char)*m_nLen);
	memcpy(m_textWhole, m_text, sizeof(char)*u32(m_nCpt));

	//Switch Buffers so that m_text will countain letter by letter dst and m_textWhole the whole src
	char *temp	= m_text;
	m_text		= m_textWhole;
	m_textWhole	= temp;
}

void
CLabelGradual::Clear()
{
	memset(m_text, 0, m_nLen);
	memset(m_textWhole, 0, m_nLen);
	m_nLen	= 0;
	m_nCpt	= 0;
	m_overflownInY= 0;
	m_nOffsetBegin= 0;
}
