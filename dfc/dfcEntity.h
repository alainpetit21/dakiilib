#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "libMngGraphic.h"
#include "dfcPhysObj.h"

class CEntity : public CPhysObj
{
public :
	enum eMap
	{
		mapMouseClick	= 0,
		mapMouseOver	= 1,
		maxObjMap		= 2,
	};
	CEntity();
	CEntity(CEntity& p_copy);
	virtual ~CEntity();
	IMPLEMENT_RTTI(Entity, PhysObj);

	virtual void LoadTGA(const char* p_dataSource, int p_idxDest);
	virtual void LoadInstance(const char* p_dataSource);
	virtual void Manage(u32 p_timeTick);
	virtual void Update(void);

	virtual void SetAnimationSpeed(float p_speed)	{m_speed= p_speed;}
	virtual void SetState(u32 p_state);
	virtual void SetFrame(u32 p_frame);

	virtual u32 GetWidth(void);
	virtual u32 GetHeight(void);
	virtual u32 GetState(void)	{return m_curState;};
	virtual u32 GetNbState(void){return m_nbState;};
	virtual u32 GetFrame(void)	{return u32(m_indexFrame);};
	virtual u32 GetNbFrame(void)	{return u32(m_arState[m_curState+1] - m_arState[m_curState]);};

	void (CObj::*OnMsgMap[maxObjMap])(CEntity*);

	u32 GetHotSpotX(void)
	{
		if(!m_pCurAnim)
			return 0;
		return m_pCurAnim[s32(m_indexFrame)].nHotSpotX;
	};
	u32 GetHotSpotY(void)
	{
		if(!m_pCurAnim)
			return 0;
		return m_pCurAnim[s32(m_indexFrame)].nHotSpotY;
	};

	TRuntimeFrame	*m_arFrames;
	TRuntimeFrame	**m_arState;
	TRuntimeFrame	*m_pCurAnim;
	u32				m_nbFrame;
	u32				m_nbState;
	u32				m_curState;
	float			m_indexFrame;
	float			m_speed;
	float			m_zoom;
	u32				*m_arSpeed;
};

#endif //_ENTITY_H_
