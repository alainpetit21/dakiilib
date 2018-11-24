#ifndef _RPG_ENGINE_ENTITIES_H_
#define _RPG_ENGINE_ENTITIES_H_

#include "dfcRPGEngine.h"

///////////
//Character
class CRPGEntity 
{
public:
	CRPGEntity(	char *p_strID, int p_nStrength, int p_nDexterity, int p_nConstitution, int p_nIntellect, \
				int p_nCharisma, int p_nLevel, int p_nXP, int p_nHP, int p_nMP, int p_nMaxHP, \
				int p_nMaxMP, int m_nSizeMaxXPTable, int m_nSizeMaxModElement, int m_nSizeMaxEquipment, \
				int m_nSizeMaxSpells);
	~CRPGEntity();

	int			m_nIdx;
	CDString	m_strID;
	CDString	m_strName;

	int			m_nStrength;
	int			m_nDexterity;
	int			m_nConstitution;
	int			m_nIntellect;
	int			m_nCharisma;

	int			m_nLevel;
	int			m_nXP;
	int			m_nHP;
	int			m_nMP;
	int			m_nMaxHP;
	int			m_nMaxMP;

	int			m_nSizeMaxXPTable;
	int			m_nSizeMaxModElement;
	int			m_nSizeMaxEquipment;
	int			m_nSizeMaxSpells;

	int			*m_arXPTable;
	float		*m_arModElement;
	CRPGItem	**m_pArEquipment;
	CRPGSpell	**m_pArSpells;

};


#endif //_RPG_ENGINE_ENTITIES_H_
