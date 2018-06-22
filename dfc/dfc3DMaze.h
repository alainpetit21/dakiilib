#ifndef _3D_MAZE_H_
#define _3D_MAZE_H_

#include "dfc3DEntity.h"

//0	Wall
//1	Free
//2	Entrance
//3	StairUp
//4	StairDown
//5	Door
//6	LockedDoor
//7	MagiclyLockedDoor

class CMaze3D : public CEntity3D
{
public :
	CMaze3D();
	~CMaze3D();
	IMPLEMENT_RTTI(Maze3D, Entity3D);

	virtual void LoadInstance(const char* p_dataSource);
	virtual void Manage(u32 p_timeTick);
	virtual void Update(void);
	virtual void SetFrame(u32 p_frame){};
	virtual void SetState(u32 p_state){};

	void Build3D();


	bool TileIsFree(int x, int y, int z)
	{
		return ((m_arMap[(z*(m_nWidth*m_nHeight))+(y*m_nWidth)+x] == 0) || (m_arMap[(z*(m_nWidth*m_nHeight))+(y*m_nWidth)+x] == 3) || (m_arMap[(z*(m_nWidth*m_nHeight))+(y*m_nWidth)+x] == 4));
	}


	u8		GetCollision(int p_nX, int p_nY);
	char	m_strName[128];
	int		m_nWidth;
	int		m_nHeight;
	int		m_nDepth;
	u8		*m_arMap;

	float	m_startingPos[3];
	u32		m_startingAngleY;
	int		m_nCurZ;

//	CVertexBuffer	*m_pVbFloor;
//	CIndexBuffer	*m_pIbFloor;
//	TRuntimeFrame	m_textureFloor;

//	CVertexBuffer	*m_pVbCeiling;
//	CIndexBuffer	*m_pIbCeiling;
//	TRuntimeFrame	m_textureCeiling;
	//Ca va me prendre des Buffer a chaque fois que je veux changer de texture, ie: Porte, Stairs, etc;

#ifdef _DEBUG
	bool	m_bShowDebug;
#endif
};

#endif // _3D_MAZE_H_
