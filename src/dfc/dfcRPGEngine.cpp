#include "lib.h"
#include "libMacros.h"
#include "dfcRPGEngine.h"
#include "dfcRPGEngineEntities.h"
#include "dfcRPGEngineSpells.h"
#include "dfcRPGEngineItems.h"

CRPGSystem::CRPGSystem():
m_pArAllEntities(0),
m_pArAllSpells(0),
m_pArAllItems(0),
m_nMaxEntities(0),
m_nMaxSpells(0),
m_nMaxItems(0),
m_nCptEntities(0),
m_nCptSpells(0),
m_nCptItems(0)
{

}

CRPGSystem::~CRPGSystem()
{
int i;

	for(i= 0; i < m_nCptEntities; ++i)
		SAFE_DELETE(m_pArAllEntities[i]);
	for(i= 0; i < m_nCptSpells; ++i)
		SAFE_DELETE(m_pArAllSpells[i]);
	for(i= 0; i < m_nCptItems; ++i)
		SAFE_DELETE(m_pArAllItems[i]);

	SAFE_DELETE_ARRAY(m_pArAllEntities);
	SAFE_DELETE_ARRAY(m_pArAllSpells);
	SAFE_DELETE_ARRAY(m_pArAllItems);
}

void
CRPGSystem::SetMaxEntities(int p_nMaxEntities)
{
CRPGEntity **temp= new CRPGEntity*[p_nMaxEntities];

	memset(temp, 0, sizeof(CRPGEntity*)*p_nMaxEntities);

	memcpy(temp, m_pArAllEntities, sizeof(CRPGEntity*)*m_nMaxEntities);
	SAFE_DELETE_ARRAY(m_pArAllEntities);
	
	m_pArAllEntities= temp;
	m_nMaxEntities	= p_nMaxEntities;
}

void
CRPGSystem::SetMaxSpells(int p_nMaxSpell)
{
CRPGSpell **temp= new CRPGSpell*[p_nMaxSpell];

	memset(temp, 0, sizeof(CRPGSpell*)*p_nMaxSpell);

	memcpy(temp, m_pArAllSpells, sizeof(CRPGSpell*)*m_nMaxSpells);
	SAFE_DELETE_ARRAY(m_pArAllSpells);
	
	m_pArAllSpells	= temp;
	m_nMaxSpells	= p_nMaxSpell;
}

void
CRPGSystem::SetMaxItems(int p_nMaxItems)
{
CRPGItem **temp= new CRPGItem*[p_nMaxItems];

	memset(temp, 0, sizeof(CRPGItem*)*p_nMaxItems);

	memcpy(temp, m_pArAllItems, sizeof(CRPGItem*)*m_nMaxItems);
	SAFE_DELETE_ARRAY(m_pArAllItems);
	
	m_pArAllItems	= temp;
	m_nMaxItems		= p_nMaxItems;
}


void
CRPGSystem::AddEntity(CRPGEntity *p_pEntity)
{
	if(m_nMaxEntities == m_nCptEntities)
		SetMaxEntities(1+m_nMaxEntities+(m_nMaxEntities>>1));

	p_pEntity->m_nIdx= m_nCptEntities;
	m_pArAllEntities[m_nCptEntities++]= p_pEntity;
}

void
CRPGSystem::AddItem(CRPGItem *p_pItem)
{
	if(m_nMaxItems == m_nCptItems)
		SetMaxItems(1+m_nMaxItems+(m_nMaxItems>>1));

	p_pItem->m_nIdx= m_nCptItems;
	m_pArAllItems[m_nCptItems++]= p_pItem;
}

void
CRPGSystem::AddSpell(CRPGSpell *p_pSpell)
{
	if(m_nMaxSpells == m_nCptSpells)
		SetMaxSpells(1+m_nMaxSpells+(m_nMaxSpells>>1));

	p_pSpell->m_nIdx= m_nCptSpells;
	m_pArAllSpells[m_nCptSpells++]= p_pSpell;
}
