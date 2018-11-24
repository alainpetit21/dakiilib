#ifndef _SCENE_NODES_H_
#define _SCENE_NODES_H_

#include "dfcSceneEngine.h"
#include "dfcLabel.h"
#include "dfcEntity.h"
#include "dfcMusic.h"

class CNode
{
public:
	CNode();
	virtual ~CNode();
	virtual SE_CLASSID Rtti(void)			{return SECLSID_CNode;};
	virtual bool IsKindOf(SE_CLASSID p_id)	{return (p_id == SECLSID_CNode);};

	virtual void LoadInstance(CXml &p_refXml){};

	void SetMaxChilds(int p_nbChilds);
	void AddChild(int p_idxNode);

	virtual void Start();
	virtual bool operator()(u32 p_timeTick){return false;};
	virtual void End();

	int		m_nIdx;
	bool	m_bRunning;
	int		m_nMaxChilds;
	int		m_nCptChilds;
	int		*m_arChilds;
};

class CNodeLblGradualAppend	: public CNode
{
public:
	CNodeLblGradualAppend();
	virtual ~CNodeLblGradualAppend();
	IMPLEMENT_SCN_RTTI(NodeLblGradualAppend, Node);

	virtual void LoadInstance(CXml &p_refXml);

	void SetLabelDst(CLabelGradual *p_lblDst)	{m_lblDst= p_lblDst;};
	void SetTxtFinal(char *p_txt)				{m_strToAdd= p_txt;};
	void SetSpeed(float p_speed)				{m_nSpeed= p_speed;};

	virtual void Start();
	virtual bool operator()(u32 p_timeTick);
	virtual void End();

	CDString		m_strToAdd;
	CLabelGradual	*m_lblDst;
	float			m_nSpeed;
};

class CNodeWait	: public CNode
{
public:
	CNodeWait();
	virtual ~CNodeWait();
	IMPLEMENT_SCN_RTTI(NodeWait, Node);

	virtual void LoadInstance(CXml &p_refXml);

	void SetDelay(u32 p_nTimeMS)	{m_nMSWaitMax= s32(p_nTimeMS);};

	virtual void Start();
	virtual bool operator()(u32 p_timeTick);

	s32	m_nMSWait;
	s32	m_nMSWaitMax;
};

class CNodeWaitRand	: public CNodeWait
{
public:
	CNodeWaitRand();
	virtual ~CNodeWaitRand();
	IMPLEMENT_SCN_RTTI(NodeWaitRand, NodeWait);

	virtual void LoadInstance(CXml &p_refXml);

	void SetMinMax(u32 p_nMin, u32 p_nMax)	{m_nRandMin= p_nMin; m_nRandMax= p_nMax;};

	virtual void Start();

	u32	m_nRandMin;
	u32	m_nRandMax;
};

class CNodeWaitKey	: public CNode
{
public:
	CNodeWaitKey();
	virtual ~CNodeWaitKey();
	IMPLEMENT_SCN_RTTI(NodeWaitKey, Node);

	virtual void LoadInstance(CXml &p_refXml);

	void SetButton(u32 p_nButton)	{m_nButton= p_nButton;};

	virtual bool operator()(u32 p_timeTick);

	u32	m_nButton;
};

class CNodeShutScene	: public CNode
{
public:
	CNodeShutScene();
	virtual ~CNodeShutScene();
	IMPLEMENT_SCN_RTTI(NodeShutScene, Node);

	virtual void LoadInstance(CXml &p_refXml);

	void SetNode(int p_idxNode)		{m_pIdxNodeToShut= p_idxNode;};

	virtual bool operator()(u32 p_timeTick);

	s32		m_pIdxNodeToShut;
};

class CNodeShowPhysObj	: public CNode
{
public:
	CNodeShowPhysObj();
	virtual ~CNodeShowPhysObj();
	IMPLEMENT_SCN_RTTI(NodeShowPhysObj, Node);

	virtual void LoadInstance(CXml &p_refXml);

	void SetObj(CPhysObj *p_pPhysObj, bool p_bShow= true)	{m_pPhysObj= p_pPhysObj;	m_bShow= p_bShow;};

	virtual bool operator()(u32 p_timeTick);

	CPhysObj	*m_pPhysObj;
	bool		m_bShow;
	int			m_nState;
};

class CNodeClearLabelGradual	: public CNode
{
public:
	CNodeClearLabelGradual();
	virtual ~CNodeClearLabelGradual();
	IMPLEMENT_SCN_RTTI(NodeClearLabelGradual, Node);

	virtual void LoadInstance(CXml &p_refXml);

	void SetLbl(CLabelGradual *p_pLabel)	{m_pLabel= p_pLabel;};

	virtual bool operator()(u32 p_timeTick);

	CLabelGradual	*m_pLabel;
};

class CNodeSetPosPhysObj	: public CNode
{
public:
	CNodeSetPosPhysObj();
	virtual ~CNodeSetPosPhysObj();
	IMPLEMENT_SCN_RTTI(NodeSetPosPhysObj, Node);

	virtual void LoadInstance(CXml &p_refXml);

	void SetObj(CPhysObj *p_pPhysObj)			{m_pPhysObj= p_pPhysObj;};
	void SetPos(int p_nX, int p_nY, int p_nZ)	{m_pos[0]= p_nX; m_pos[1]= p_nY; m_pos[2]= p_nZ;};
	void SetPos(int *p_pos)						{m_pos[0]= p_pos[0]; m_pos[1]= p_pos[1]; m_pos[2]= p_pos[2];};

	virtual bool operator()(u32 p_timeTick);

	CPhysObj	*m_pPhysObj;
	int			m_pos[3];
};


class CNodeAnimateEntityRandStateRandPos	: public CNode
{
public:
	CNodeAnimateEntityRandStateRandPos();
	virtual ~CNodeAnimateEntityRandStateRandPos();
	IMPLEMENT_SCN_RTTI(NodeAnimateEntityRandStateRandPos, Node);

	virtual void LoadInstance(CXml &p_refXml);

	void SetObj(CEntity *p_pEntity)											{m_pEntity= p_pEntity;};
	void SetPosMinMax(s32 p_nXMin, s32 p_nYMin, s32 p_nXMax, s32 p_nYMax)	{m_posMinX= p_nXMin; m_posMinY= p_nYMin; m_posMaxX= p_nXMax; m_posMaxY= p_nYMax;};

	virtual void Start();
	virtual bool operator()(u32 p_timeTick);
	virtual void End();

	CEntity		*m_pEntity;
	s32			m_posMinX;
	s32			m_posMinY;
	s32			m_posMaxX;
	s32			m_posMaxY;
};

class CNodeMoveToPosPhysObj	: public CNode
{
public:
	CNodeMoveToPosPhysObj();
	virtual ~CNodeMoveToPosPhysObj();
	IMPLEMENT_SCN_RTTI(NodeMoveToPosPhysObj, Node);

	virtual void LoadInstance(CXml &p_refXml);

	void SetObj(CPhysObj *p_pPhysObj)			{m_pPhysObj= p_pPhysObj;};
	void SetPos(int p_nX, int p_nY, int p_nZ)	{m_pos[0]= p_nX; m_pos[1]= p_nY; m_pos[2]= p_nZ;};
	void SetPos(int *p_pos)						{m_pos[0]= p_pos[0]; m_pos[1]= p_pos[1]; m_pos[2]= p_pos[2];};

	virtual void Start();
	virtual bool operator()(u32 p_timeTick);

	CPhysObj	*m_pPhysObj;
	float		m_nSpeedMax;
	int			m_pos[3];
};

class CNodeIncVariable	: public CNode
{
public:
	CNodeIncVariable();
	virtual ~CNodeIncVariable();
	IMPLEMENT_SCN_RTTI(NodeIncVariable, Node);

	virtual void LoadInstance(CXml &p_refXml);

	void SetVarU32(u32* p_var)					{m_pVarU32= p_var;};

	virtual bool operator()(u32 p_timeTick);

	u32*	m_pVarU32;
};

class CNodeStartMusic	: public CNode
{
public:
	CNodeStartMusic();
	virtual ~CNodeStartMusic();
	IMPLEMENT_SCN_RTTI(NodeStartMusic, Node);

	virtual void LoadInstance(CXml &p_refXml);

	void SetMusic(CMusic* p_pMusic)				{m_pMusic= p_pMusic;};

	virtual bool operator()(u32 p_timeTick);

	CMusic*	m_pMusic;
};


#endif //_SCENE_NODES_H_
