#ifndef _XML_H_
#define _XML_H_

#include <string.h>
#include "libTypes.h"
#include "DString.h"

class CDString;
class CXml
{
public:
	CXml(void);
	CXml(const char* szDoc);
	CXml(const CXml& markup);
	virtual ~CXml();

	void operator=(const CXml& markup);

	// Create
	CDString& GetDoc(void)	{return m_csDoc;};
	bool AddElem		(const char* szName, const char* szData= 0)	{return x_AddElem(szName, szData, false, false);};
	bool AddChildElem	(const char* szName, const char* szData= 0)	{return x_AddElem(szName, szData, false, true);};
	bool AddAttrib		(const char* szAttrib, const char* szValue)		{return x_SetAttrib(m_iPos, szAttrib, szValue);};
	bool AddChildAttrib	(const char* szAttrib, const char* szValue)		{return x_SetAttrib(m_iPosChild, szAttrib, szValue);};
	bool SetAttrib		(const char* szAttrib, const char* szValue)		{return x_SetAttrib(m_iPos, szAttrib, szValue);};
	bool SetChildAttrib	(const char* szAttrib, const char* szValue)		{return x_SetAttrib(m_iPosChild, szAttrib, szValue);};

	// Navigate
	bool SetDoc			(const char* szDoc );
	bool IsWellFormed	(void);
	bool FindElem		(const char* szName= 0);
	bool FindChildElem	(const char* szName= 0);
	bool IntoElem		(void);
	bool OutOfElem		(void);

	void ResetChildPos(void){x_SetPos(m_iPosParent, m_iPos, 0);};
	void ResetMainPos(void) {x_SetPos(m_iPosParent, 0, 0);};
	void ResetPos(void)		{x_SetPos(0, 0, 0);};

	CDString GetTagName(void);
	CDString GetChildTagName(void)				{return x_GetTagName(m_iPosChild);};
	CDString GetData(void)						{return x_GetData(m_iPos);};
	CDString GetChildData(void)					{return x_GetData(m_iPosChild);};
	CDString GetAttrib(const char* szAttrib)	{return x_GetAttrib(m_iPos,szAttrib);};
	CDString GetChildAttrib(const char* szAttrib){return x_GetAttrib(m_iPosChild,szAttrib);};
	CDString GetError(void)						{return m_csError;};

	enum MarkupNodeType
	{
		MNT_ELEMENT					= 1,  // 0x01
		MNT_TEXT					= 2,  // 0x02
		MNT_WHITESPACE				= 4,  // 0x04
		MNT_CDATA_SECTION			= 8,  // 0x08
		MNT_PROCESSING_INSTRUCTION	= 16, // 0x10
		MNT_COMMENT					= 32, // 0x20
		MNT_DOCUMENT_TYPE			= 64, // 0x40
		MNT_EXCLUDE_WHITESPACE		= 123,// 0x7b
	};

protected:

	CDString m_csDoc;
	CDString m_csError;

	struct ElemPos
	{
		ElemPos(void) {Clear();};
		ElemPos(const ElemPos& pos) {*this = pos;};

		bool IsEmptyElement(void)	{return (nStartR == nEndL + 1);};
		void AdjustStart(int n)		{nStartL+=n; nStartR+=n;};
		void AdjustEnd(int n)		{nEndL+=n; nEndR+=n;};
		void Clear(void)
		{
			nStartL=0; nStartR=0; nEndL=0; nEndR=0; nReserved=0;
			iElemParent=0; iElemChild=0; iElemNext=0;
		};

		int nStartL;
		int nStartR;
		int nEndL;
		int nEndR;
		int nReserved;
		int iElemParent;
		int iElemChild;
		int iElemNext;
	};

	ElemPos	*m_aPos;
	int		m_nPosArrayLenght;

	int m_iPosParent;
	int m_iPos;
	int m_iPosChild;
	int m_iPosFree;
	int m_nNodeType;

	struct TokenPos
	{
		TokenPos(const char* sz)	{Clear(); szDoc = sz;};

		bool IsValid(void)			{return (nL <= nR);};
		void Clear(void)			{nL=0; nR=-1; nNext=0; bIsString=false;};
		bool Match(const char* szName)
		{
			int nLen = nR - nL + 1;
			return ( (strncmp( &szDoc[nL], szName, nLen ) == 0)
				&& ( szName[nLen] == '\0' || strchr(" =/[",szName[nLen]) ) );
		};

		int nL;
		int nR;
		int nNext;
		const char* szDoc;
		bool bIsString;
	};

	void x_SetPos(int iPosParent, int iPos, int iPosChild)
	{
		m_iPosParent= iPosParent;
		m_iPos		= iPos;
		m_iPosChild = iPosChild;
		m_nNodeType = (iPos)? MNT_ELEMENT:0;
	};

	int x_GetFreePos(void);
	int x_ReleasePos(void);
	int x_ParseElem	(int iPos);
	int x_ParseError(const char* szError, const char* szName= 0);

	static bool x_FindChar	(const char* szDoc, int& nChar, char c);
	static bool x_FindAny	(const char* szDoc, int& nChar);
	static bool x_FindToken	(TokenPos& token);

	int			x_FindElem	(int iPosParent, int iPos, const char* szPath);
	CDString	x_GetToken	(const TokenPos& token);
	CDString	x_GetTagName(int iPos);
	CDString	x_GetData	(int iPos);
	CDString	x_GetAttrib	(int iPos, const char* szAttrib);
	bool		x_AddElem	(const char* szName, const char* szValue, bool bInsert, bool bAddChild);
	bool		x_FindAttrib(TokenPos& token, const char* szAttrib= 0);
	bool		x_SetAttrib	(int iPos, const char* szAttrib, const char* szValue);
	void		x_LocateNew	(int iPosParent, int& iPosRel, int& nOffset, int nLength, int nFlags);
	int			x_ParseNode	(TokenPos& token);
	void		x_DocChange	(int nLeft, int nReplace, const CDString& csInsert);
	void		x_Adjust	(int iPos, int nShift, bool bAfterPos= false);
	CDString	x_TextToDoc	(const char* szText, bool bAttrib= false);
	CDString	x_TextFromDoc(int nLeft, int nRight);
};

#endif // _XML_H_
