#ifndef _SCENE_ENGINE_H_
#define _SCENE_ENGINE_H_

#include "DString.h"
#include "XMLLoader.h"

typedef enum tagSE_CLASSID
{
	SECLSID_CNode								= 0,
	SECLSID_CNodeShutScene						= 1,
	SECLSID_CNodeIncVariable					= 2,
	SECLSID_CNodeWait							= 3,
	SECLSID_CNodeWaitRand						= 4,
	SECLSID_CNodeWaitKey						= 5,
	SECLSID_CNodeShowPhysObj					= 6,
	SECLSID_CNodeSetPosPhysObj					= 7,
	SECLSID_CNodeLblGradualAppend				= 8,
	SECLSID_CNodeClearLabelGradual				= 9,
	SECLSID_CNodeAnimateEntityRandStateRandPos	= 10,
	SECLSID_CNodeMoveToPosPhysObj				= 11,
	SECLSID_CNodeStartMusic						= 12,
	SECLSID_LIBMAX								= 13,

}SE_CLASSID;

#define IMPLEMENT_SCN_RTTI(className, derivateFrom)		\
	virtual SE_CLASSID Rtti(void)						\
	{													\
		return (SE_CLASSID)SECLSID_C##className;		\
	};													\
	virtual bool IsKindOf(SE_CLASSID p_id)				\
	{													\
		bool ret;										\
		if(p_id == (SE_CLASSID)SECLSID_C##className)	\
			return true;								\
		ret= C##derivateFrom::IsKindOf(p_id);			\
		return ret;										\
	}

class CNode;
class CSceneEngine	: public CObj
{
public:
	CSceneEngine();
	~CSceneEngine();
	IMPLEMENT_RTTI(SceneEngine, Obj);

	virtual void LoadInstance(const char* p_dataSource);
	virtual void Manage(u32 p_timeTick);

	void SetMaxNodes(int p_nbNodes);
	void SetMaxRunningNodes(int p_nbNodes);
	void AddNode(CNode *p_pNode);
	void Run(int p_idxNode)	{Run(m_arNodes[p_idxNode]);};
	void Run(CNode *p_node);
	void Stop(int p_idxNode){Stop(m_arNodes[p_idxNode]);};
	void Stop(CNode *p_node);
	void StopWithChilds(int p_idxNode){StopWithChilds(m_arNodes[p_idxNode]);};
	void StopWithChilds(CNode *p_node);
	void StopWithChildsInner(CNode *p_node);

	CNode*	GetNode(int p_idxNode)	{return m_arNodes[p_idxNode];};
	CNode*	GetNodeRoot()			{return m_pNodeRoot;};
	bool	IsDone()				{return m_nCptRunningNodes == 0;};

	CNode	*m_pNodeRoot;
	CNode	**m_arNodes;			//Countainer of all the Nodes
	CNode	**m_arRunningNodes;

	int		m_nCptNodes;
	int		m_nMaxNodes;
	int		m_nCptRunningNodes;
	int		m_nMaxRuningNodes;

	static CSceneEngine* singleton;
};

static inline int
helper_StringToNodeType(char *p_strType)
{
	if(strcmp(p_strType, "SECLSID_CNode") == 0)										return SECLSID_CNode;
	else if(strcmp(p_strType, "SECLSID_CNodeLblGradualAppend") == 0)				return SECLSID_CNodeLblGradualAppend;
	else if(strcmp(p_strType, "SECLSID_CNodeWait") == 0)							return SECLSID_CNodeWait;
	else if(strcmp(p_strType, "SECLSID_CNodeWaitRand") == 0)						return SECLSID_CNodeWaitRand;
	else if(strcmp(p_strType, "SECLSID_CNodeWaitKey") == 0)							return SECLSID_CNodeWaitKey;
	else if(strcmp(p_strType, "SECLSID_CNodeShutScene") == 0)						return SECLSID_CNodeShutScene;
	else if(strcmp(p_strType, "SECLSID_CNodeShowPhysObj") == 0)						return SECLSID_CNodeShowPhysObj;
	else if(strcmp(p_strType, "SECLSID_CNodeClearLabelGradual") == 0)				return SECLSID_CNodeClearLabelGradual;
	else if(strcmp(p_strType, "SECLSID_CNodeSetPosPhysObj") == 0)					return SECLSID_CNodeSetPosPhysObj;
	else if(strcmp(p_strType, "SECLSID_CNodeAnimateEntityRandStateRandPos") == 0)	return SECLSID_CNodeAnimateEntityRandStateRandPos;
	else if(strcmp(p_strType, "SECLSID_CNodeMoveToPosPhysObj") == 0)				return SECLSID_CNodeMoveToPosPhysObj;
	else if(strcmp(p_strType, "SECLSID_CNodeIncVariable") == 0)						return SECLSID_CNodeIncVariable;
	else if(strcmp(p_strType, "SECLSID_CNodeStartMusic") == 0)						return SECLSID_CNodeStartMusic;
	else																			return -1;
}

static inline bool
helper_NodeIDIs(CObj *p_obj, char *p_strType)
{
	return(strcmp(p_obj->GetTextID(), p_strType) == 0);
}



#endif //_SCENE_ENGINE_H_

