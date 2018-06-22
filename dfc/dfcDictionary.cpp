#include "lib.h"
#include "libMacros.h"
#include "dfcDictionary.h"

CDictionary*	CDictionary::m_pInstalledDictionary;

CDictionary::CDictionary():
m_nbLanguage(0),
m_nbEntry(0),
m_nCurLanguage(0),
m_allString(0)
{
	CDictionary::m_pInstalledDictionary= 0;
}

CDictionary::~CDictionary()
{
	if(m_allString){
		for(int i= 0 ; i < m_nbLanguage; ++i)
			SAFE_DELETE_ARRAY(m_allString[i]);
	}

	SAFE_DELETE_ARRAY(m_allString);
	SAFE_DELETE_ARRAY(m_mapString);
	SAFE_DELETE_ARRAY(m_strNameLanguage);
}

void
CDictionary::LoadInstance(const char* p_dataSource)
{
	CObj::LoadInstance(p_dataSource);

	m_pXml->FindElem("NB_LANGUAGES");
	m_nbLanguage= atoi((m_pXml->GetData()).m_arBuffer);

	m_pXml->FindElem("NB_ENTRY");
	m_nbEntry= atoi((m_pXml->GetData()).m_arBuffer);

	m_strNameLanguage	= new CDString[m_nbLanguage];
	m_mapString			= new CDString[m_nbEntry];
	m_allString			= new CDString *[m_nbLanguage];

	for(int i= 0 ; i < m_nbLanguage; ++i){

		m_allString[i]= new CDString[m_nbEntry];

		m_pXml->FindElem("LANGUAGE");
		m_pXml->IntoElem();{

			m_pXml->FindElem("NAME");
			m_strNameLanguage[i]= m_pXml->GetData();

			for(int j= 0 ; j < m_nbEntry; ++j){
				m_pXml->FindElem("ENTRY");
				m_pXml->IntoElem();{

					m_pXml->FindElem("MAP");
					m_mapString[j]= m_pXml->GetData();

					m_pXml->FindElem("TXT");
					m_allString[i][j]= m_pXml->GetData();
				}m_pXml->OutOfElem();
			}
		}m_pXml->OutOfElem();
	}

	CDictionary::m_pInstalledDictionary= this;

	SAFE_DELETE(m_pXml);
}

char*
CDictionary::GetString(const char *p_mapString)
{
	for(int i= 0 ; i < m_nbEntry; ++i)
		if(m_mapString[i] == p_mapString)
			return m_allString[m_nCurLanguage][i].m_arBuffer;

	ASSERT3(0, "Cannot find the requested string in dictionnary for the current language setting: %s", p_mapString);
	return 0;
}

void
CDictionary::SetCurLanguage(const char *p_strCurLanguage)
{
	for(int i= 0 ; i < m_nbLanguage; ++i)
		if(m_strNameLanguage[i] == p_strCurLanguage)
			SetCurLanguage(i);

}
