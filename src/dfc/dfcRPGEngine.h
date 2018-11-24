#ifndef _RPG_ENGINE_H_
#define _RPG_ENGINE_H_

#define NB_ELEMENT	4

#include "DString.h"

class CRPGItem;
class CRPGSpell;
class CRPGEntity;

////////////////
//General System 
//	Countains countainer all instances of all items, all entities
class CRPGSystem
{
public:
	CRPGSystem();
	virtual ~CRPGSystem();

	void	SetMaxEntities(int p_nMaxEntities);
	void	SetMaxSpells(int p_nMaxSpell);
	void	SetMaxItems(int p_nMaxItems);

	void	AddEntity(CRPGEntity *p_pEntity);
	void	AddSpell(CRPGSpell *p_pSpell);
	void	AddItem(CRPGItem *p_pItem);

	CRPGEntity	**m_pArAllEntities;
	CRPGSpell	**m_pArAllSpells;
	CRPGItem	**m_pArAllItems;

	int		m_nMaxEntities;
	int		m_nMaxSpells;
	int		m_nMaxItems;

	int		m_nCptEntities;
	int		m_nCptSpells;
	int		m_nCptItems;
};



#endif //_RPG_ENGINE_H_
