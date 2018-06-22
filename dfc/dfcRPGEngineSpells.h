#ifndef _RPG_ENGINE_SPELLS_H_
#define _RPG_ENGINE_SPELLS_H_

#include "dfcRPGEngine.h"

///////////
//Magic
class CRPGSpell
{
public:
	CRPGSpell(){};
	virtual ~CRPGSpell(){};

	virtual void Use(CRPGEntity *p_pEntUser)	{m_pEntUser= p_pEntUser;};

	int			m_nIdx;
	int			m_nValue;
	CDString	m_strName;
	CRPGEntity	*m_pEntUser;
	float		m_modElement[NB_ELEMENT];
};

class CRPGSpellOne	: public CRPGSpell
{
public:
	CRPGSpellOne():CRPGSpell(){};
	virtual ~CRPGSpellOne(){};

	virtual void Use(CRPGEntity *p_pEntUser)	{CRPGSpell::Use(p_pEntUser);};
	virtual void SetDst(CRPGEntity *p_pEntDst)	{m_pEntDst= p_pEntDst;};

	CRPGEntity *m_pEntDst;
};

class CRPGSpellList	: public CRPGSpell
{
public:
	CRPGSpellList():CRPGSpell(), m_pArEntDst(0), m_nMaxEntDst(0), m_nCptEntDst(0){}
	virtual ~CRPGSpellList(){SAFE_DELETE_ARRAY(m_pArEntDst);};

	virtual void Use(CRPGEntity *p_pEntUser)	{CRPGSpell::Use(p_pEntUser);};
	virtual	void SetMaxEntDst(int p_nMaxEntDst);
	virtual void AddDst(CRPGEntity *p_pEntDst);

	CRPGEntity	**m_pArEntDst;
	int			m_nMaxEntDst;
	int			m_nCptEntDst;
};

#endif //_RPG_ENGINE_SPELLS_H_
