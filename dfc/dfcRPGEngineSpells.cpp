#include "lib.h"
#include "libMacros.h"
#include "dfcRPGEngineSpells.h"

void 
CRPGSpellList::SetMaxEntDst(int p_nMaxEntDst)
{
CRPGEntity **temp= new CRPGEntity*[p_nMaxEntDst];

	memset(temp, 0, sizeof(CRPGEntity*)*p_nMaxEntDst);

	memcpy(temp, m_pArEntDst, sizeof(CRPGEntity*)*m_nMaxEntDst);
	SAFE_DELETE_ARRAY(m_pArEntDst);
	
	m_pArEntDst	= temp;
	m_nMaxEntDst= p_nMaxEntDst;
}


void 
CRPGSpellList::AddDst(CRPGEntity *p_pEntDst)
{
	if(m_nMaxEntDst == m_nCptEntDst)
		SetMaxEntDst(1+m_nMaxEntDst+(m_nMaxEntDst>>1));

	m_pArEntDst[m_nCptEntDst++]= p_pEntDst;
}

