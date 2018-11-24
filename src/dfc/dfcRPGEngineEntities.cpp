#include "lib.h"
#include "libMacros.h"
#include "dfcRPGEngineEntities.h"
#include "dfcDictionary.h"

CRPGEntity::CRPGEntity(	char *p_strID, int p_nStrength, int p_nDexterity, int p_nConstitution, int p_nIntellect, \
						int p_nCharisma, int p_nLevel, int p_nXP, int p_nHP, int p_nMP, int p_nMaxHP, \
						int p_nMaxMP, int p_nSizeMaxXPTable, int p_nSizeMaxModElement, int p_nSizeMaxEquipment, \
						int p_nSizeMaxSpells):
m_strID(p_strID),
m_strName(GET_STRING(p_strID)),
m_nStrength(p_nStrength),
m_nDexterity(p_nDexterity),
m_nConstitution(p_nConstitution),
m_nIntellect(p_nIntellect),
m_nCharisma(p_nCharisma),
m_nLevel(p_nLevel),
m_nXP(p_nXP),
m_nHP(p_nHP),
m_nMP(p_nMP),
m_nMaxHP(p_nMaxHP),
m_nMaxMP(p_nMaxMP),
m_nSizeMaxXPTable(p_nSizeMaxXPTable),
m_nSizeMaxModElement(p_nSizeMaxModElement),
m_nSizeMaxEquipment(p_nSizeMaxEquipment),
m_nSizeMaxSpells(p_nSizeMaxSpells)
{
	m_arXPTable		= new int[p_nSizeMaxXPTable];
	m_arModElement	= new float[p_nSizeMaxModElement];
	m_pArEquipment	= new CRPGItem*[p_nSizeMaxEquipment];
	m_pArSpells		= new CRPGSpell*[p_nSizeMaxSpells];
}

CRPGEntity::~CRPGEntity()
{
	delete []m_arXPTable;
	delete []m_arModElement;
	delete []m_pArEquipment;
	SAFE_DELETE_ARRAY(m_pArSpells);
}
