#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "libTypes.h"
#include "dfcPhysObj.h"


class CCamera2D : public CPhysObj
{
public:
	CCamera2D(){};
	virtual ~CCamera2D(){};
	IMPLEMENT_RTTI(Camera2D, PhysObj);

	//From CObject
	virtual void		Manage(unsigned long p_timeTick);
	virtual void		Update(void);
	virtual u32			AddChild(CObj* p_pObj, bool p_bTransformSetRelativity);
	virtual CObj*		RemoveChild(CObj* p_pObj);

	//From CCamera
	void				FocusOn(CPhysObj *p_focused);
	void				Project(CPhysObj *p_obj);

	void				SetCurrent(void);
	static CCamera2D*	GetCurrent(void)	{return m_pCurCamera;};
	static CCamera2D*	m_pCurCamera;
};


class CCamera3D : public CPhysObj
{
public:
	CCamera3D(){};
	virtual ~CCamera3D(){};
	IMPLEMENT_RTTI(Camera3D, PhysObj);

	//From CObject
	virtual void		Manage(unsigned long p_timeTick);
	virtual void		Update(void);

	//From CCamera
	void				FocusOn(CPhysObj *p_focused);

	void				SetCurrent(void);
	static CCamera3D*	GetCurrent(void)	{return m_pCurCamera;};
	static CCamera3D*	m_pCurCamera;
};

#endif	//_CAMERA_H_
