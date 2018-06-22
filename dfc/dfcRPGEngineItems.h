#ifndef _RPG_ENGINE_ITEMS_H_
#define _RPG_ENGINE_ITEMS_H_

#include "dfcRPGEngine.h"

///////////
//Item
class CRPGItem
{
public:
	CRPGItem(int p_nValue, char* p_strName, char* p_strID):m_nValue(p_nValue), m_strName(p_strName), m_strID(p_strID){}
	virtual ~CRPGItem(){}

	int			m_nIdx;
	int			m_nValue;
	CDString	m_strName;
	CDString	m_strID;
};

class CRPGItemUsable	: public CRPGItem
{
public:
	CRPGItemUsable(int p_nValue, char* p_strName, char* p_strID):CRPGItem(p_nValue, p_strName, p_strID){}
	virtual ~CRPGItemUsable(){}

	virtual void Use(CRPGEntity *p_pEntUser)	{m_pEntUser= p_pEntUser;};

	CRPGEntity *m_pEntUser;
};

class CRPGItemUsableOne	: public CRPGItemUsable
{
public:
	CRPGItemUsableOne(int p_nValue, char* p_strName, char* p_strID):CRPGItemUsable(p_nValue, p_strName, p_strID){}
	virtual ~CRPGItemUsableOne(){}

	virtual void Use(CRPGEntity *p_pEntUser)	{CRPGItemUsable::Use(p_pEntUser);};
	virtual void SetDst(CRPGEntity *p_pEntDst)	{m_pEntDst= p_pEntDst;};

	CRPGEntity *m_pEntDst;
};

class CRPGItemUsableList	: public CRPGItemUsable
{
public:
	CRPGItemUsableList(int p_nValue, char* p_strName, char* p_strID):CRPGItemUsable(p_nValue, p_strName, p_strID),m_pArEntDst(0), m_nMaxEntDst(0), m_nCptEntDst(0){}
	virtual ~CRPGItemUsableList(){SAFE_DELETE_ARRAY(m_pArEntDst);};

	virtual void Use(CRPGEntity *p_pEntUser)	{CRPGItemUsable::Use(p_pEntUser);};

	virtual	void SetMaxEntDst(int p_nMaxEntDst);
	virtual void AddDst(CRPGEntity *p_pEntDst);

	CRPGEntity	**m_pArEntDst;
	int			m_nMaxEntDst;
	int			m_nCptEntDst;
};

class CRPGWeapon	: public CRPGItem
{
public:
	CRPGWeapon(int p_nValue, char* p_strName, char* p_strID):CRPGItem(p_nValue, p_strName, p_strID){}
	virtual ~CRPGWeapon(){};

	virtual	void SetMaxUsers(int p_nMaxUsers);
	virtual void AddUsers(CRPGEntity *p_pUsers);

	CRPGEntity	**m_pArUsers;
	int			m_nMaxUsers;
	int			m_nCptUsers;

	int		m_nPower;
	float	m_modElement[NB_ELEMENT];
};

class CRPGArmor	: public CRPGItem
{
public:
	CRPGArmor(int p_nValue, char* p_strName, char* p_strID):CRPGItem(p_nValue, p_strName, p_strID){}
	virtual ~CRPGArmor(){};

	virtual	void SetMaxUsers(int p_nMaxUsers);
	virtual void AddUsers(CRPGEntity *p_pUsers);

	CRPGEntity	**m_pArUsers;
	int			m_nMaxUsers;
	int			m_nCptUsers;

	int		m_nDefense;
	float	m_modElement[NB_ELEMENT];
};

//Faire les weapon usable plus tard


#endif //_RPG_ENGINE_ITEMS_H_
