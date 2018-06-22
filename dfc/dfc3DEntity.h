#ifndef _ENTITY_3D_H_
#define _ENTITY_3D_H_

#include "dfcEntity.h"
#include "libMng3D.h"

typedef struct tagT_FRAME_3D
{
public:
	TVertex *pResVertex;
}TFrame3D;

class CEntity3D : public CEntity
{
public :
	CEntity3D();
	~CEntity3D();
	IMPLEMENT_RTTI(Entity3D, Entity);

	virtual void LoadInstance(const char* p_dataSource);
	virtual void Manage(u32 p_timeTick);
	virtual void Update(void);

	virtual void SetAnimationSpeed(float p_speed)	{m_speed= p_speed;}
	virtual void SetFrame(u32 p_frame);
	virtual void SetState(u32 p_state)				{m_pCurAnim= m_arState[m_curState= p_state];};

	virtual u32 GetWidth(void);
	virtual u32 GetHeight(void);

	TFrame3D	*m_arFrames;
	TFrame3D	**m_arState;
	TFrame3D	*m_pCurAnim;
	u32			m_nbFrame;
	u32			m_nbState;
	u32			m_curState;
	float		m_indexFrame;

	CVertexBuffer				*m_pVb;
	CIndexBuffer				*m_pIb;
	CTextureCoordonateBuffer	*m_pTb;
	TRuntimeFrame				m_texture;
};

#endif // _ENTITY_3D_H_
