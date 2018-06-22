#ifndef _DICTIONARY_H_
#define _DICTIONARY_H_

#include "libTypes.h"
#include "DString.h"

class CDictionary : public CObj
{
public:
	CDictionary();
	virtual ~CDictionary();
	IMPLEMENT_RTTI(Dictionary, Obj);

	//From CApp
	virtual void LoadInstance(const char* p_dataSource);

	void	SetCurLanguage(int p_nCurLanguage){m_nCurLanguage= p_nCurLanguage;};
	void	SetCurLanguage(const char *p_strCurLanguage);
	int		GetCurLanguage(void){return m_nCurLanguage;};

	char*	GetString(const char *p_map);

	static CDictionary	*m_pInstalledDictionary;

	int m_nbLanguage;
	int m_nbEntry;
	int m_nCurLanguage;

	CDString *m_strNameLanguage;
	CDString *m_mapString;
	CDString **m_allString;
};

#define GET_STRING(stringID)									\
	(CDictionary::m_pInstalledDictionary->GetString(stringID))

#endif // _DICTIONARY_H_
