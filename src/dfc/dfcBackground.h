#ifndef _BACKGROUND_H_
#define _BACKGROUND_H_

#include "libTypes.h"
#include "libMngGraphic.h"
#include "dfcPhysObj.h"

class CBackgroundLayer : public CPhysObj
{
public:
	CBackgroundLayer(){};
	virtual ~CBackgroundLayer();
	IMPLEMENT_RTTI(BackgroundLayer, PhysObj);

	virtual void LoadInstance(const char* p_dataSource);
	virtual void LoadInstance(CXml* p_pXml);

	//From CObject
	virtual void Update(void);

	void			*m_bitmap;
	u32				m_size[2];
};

class CBackgroundLayerLooping : public CBackgroundLayer
{
public:
	CBackgroundLayerLooping(){};
	virtual ~CBackgroundLayerLooping(){};
	IMPLEMENT_RTTI(BackgroundLayerLooping, BackgroundLayer);

	//From CObject
	virtual void Update(void);
};

class CBackgroundLayerTiled : public CBackgroundLayer
{
public:
	CBackgroundLayerTiled();
	virtual ~CBackgroundLayerTiled();
	IMPLEMENT_RTTI(BackgroundLayerTiled, BackgroundLayer);

	virtual void LoadInstance(CXml* p_pXml);

	void	LoadMap(const char *p_mapFile);

	//From CObject
	virtual void Update(void);

	u32	m_nbTiles;
	u32	m_sizeTiled[2];
	u16	*m_pMap;
};


class CBackground : public CPhysObj
{
public:
	CBackground(){};
	virtual ~CBackground();
	IMPLEMENT_RTTI(Background, PhysObj);

	//From CObject
	virtual void LoadInstance(const char* p_dataSource);
	virtual void Manage(u32 p_timeTick){};
	virtual void Update(void){};

	void LoadCollision(char* p_dataSource);

	u32					m_nbLayer;
	CBackgroundLayer	**m_pLayers;
};

#endif //_BACKGROUND_H_
