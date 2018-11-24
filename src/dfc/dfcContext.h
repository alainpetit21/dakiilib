#ifndef _CONTEXT_H_
#define _CONTEXT_H_

#include "libTypes.h"
#include "dfcObj.h"
#include "dfcEntity.h"
#include "dfcBackground.h"
#include "dfcButton.h"
#include "dfcCamera.h"

#ifdef _DEBUG
#define DBG_FPS			0x01
#define DBG_OBJ			0x02
#define DBG_COL_SYS		0x04
#define DBG_SOUND		0x08
#define DBG_SCENE		0x10
#define DBG_3DMAZE		0x20
#define	DBG_MEM			0x40
#define	DBG_MOUSE		0x80
#define	DBG_ALL			0xFF

#endif

typedef void (CObj::*TCallback)(CEntity*);

class CContext;

typedef struct SLine
{
	float	m_start[2];
	float	m_end[2];
	u32		m_angle;
	u32		m_group;
}TLine;

typedef struct STimer
{
	int		nDelayMax;			// in ms
	int		nDelay;			// in ms
	void	(CContext::*pFct)(void);	// Fonction to call
}TTimer;

class CContext : public CObj
{
public:
	CContext(const char* p_pResFilename);
	virtual ~CContext();
	IMPLEMENT_RTTI(Context, Obj);

	//From CObject
	virtual void	LoadInstance(const char* p_dataSource);

	virtual void	Init(void){};
	virtual void	Activate(void){LoadInstance(m_pRes);};
	virtual void	Deactivate(void){FreeInstance();};

	virtual void	Manage(unsigned long p_nTimeTick);
	virtual void	Update(void);

	virtual u32		AddChild(CObj* p_pObj, bool p_isTransformSetRelativity);

	//From CContext
	virtual void FreeInstance();
	virtual void KeyManage(unsigned long p_nTimeTick){};

	//Collision System
	void AddLine(int p_x1, int p_y1, int p_x2, int p_y2, u32 p_group);

	virtual void		OnCollideObj2Obj(THit *p_hitStruct, CPhysObj *p_pPhysObj1, CPhysObj *p_pPhysObj2);
	virtual void		OnCollideObj2Line(THit *p_hitStruct, CPhysObj *p_pPhysObj);
	virtual CPhysObj*	UserInstall(const char *p_dataSource, const char* p_nId){return 0;};

	bool	IsCollidingObj2Obj(THit *p_hitStruct, CPhysObj *p_pPhysObj1, CPhysObj *p_pPhysObj2);
	bool	IsCollidingObj2Line(THit *p_hitStruct, CPhysObj *p_pPhysObj1, TLine *p_pLine);


	void	SetMouse(CEntity *p_pObjMouse);
	void	SetFocus(CObj *p_pRootFocus)
	{
		m_pRootFocus= p_pRootFocus;
		if(!m_pRootFocus)
			m_pRootFocus= this;
	};
	CObj*	GetFocus()		{return m_pRootFocus;};

	CObj*	FindAt(s32 p_x, s32 p_y, s32 p_z);
	CObj*	FindByID(const char * p_stringID);
	CObj*	FindByIDUnder(CObj* p_root, const char * p_szID);
	CObj*	FindByRtti(DFC_CLASSID p_ID);
	CObj*	FindAtByRtti(s32 p_nX, s32 p_nY, s32 p_nZ, DFC_CLASSID p_idClass);

	void	AddMap(u32 p_type, const char * p_idControl, TCallback p_fctMapped);
	void	AddMap(u32 p_type, CEntity* p_pButton, TCallback p_fctMapped);
	void	Click(const char *p_idControl);
	void	Click(CEntity *p_pObj);

	int		AddTimer(int nDelay, void (CContext::*p_pFct)(void));
	void	ResetTimers();

#ifdef _DEBUG
	u8			m_delay;
	char		m_tickDiff[128];
	u32			m_nDbgLevel;
#endif

	u32			m_ID;
	CCamera2D	*m_camera2D;
	CCamera3D	*m_camera3D;

	CBackground *m_pCurBackgroundObject;

	CEntity		*m_pMouse;
	CObj		*m_pRootFocus;

	//Collision System
	TLine		*m_arLines;
	u32			m_cptLines;
	u32			m_maxLines;

	TTimer		m_tTimer[4];
	int			m_nbTimer;
};

#endif // _CONTEXT_H_
