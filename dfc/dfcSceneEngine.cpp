#include "lib.h"
#include "libMacros.h"
#include "libMath.h"
#include "dfcSceneEngine.h"
#include "dfcSceneNodes.h"

CSceneEngine* CSceneEngine::singleton= 0;

CSceneEngine::CSceneEngine():
m_arNodes(0),
m_arRunningNodes(0),
m_nCptNodes(0),
m_nMaxNodes(0),
m_nCptRunningNodes(0),
m_nMaxRuningNodes(0)
{
	ASSERT2(!CSceneEngine::singleton, "Cannot have 2 scenaric engine loaded");
	CSceneEngine::singleton= this;

	m_pNodeRoot= new CNode;
}

CSceneEngine::~CSceneEngine()
{
int i;

	for(i= 0; i < m_nCptNodes; ++i){
		delete m_arNodes[i];
	}

	SAFE_DELETE_ARRAY(m_arNodes);
	SAFE_DELETE_ARRAY(m_arRunningNodes);

	delete m_pNodeRoot;

	CSceneEngine::singleton= 0;
}

void
CSceneEngine::LoadInstance(const char* p_dataSource)
{
s32 i, j, k;

	CObj::LoadInstance(p_dataSource);

	if(m_pXml->FindElem("NB_NODES")){
		s32 nNbNodes= atoi((m_pXml->GetData()).m_arBuffer);

		if(m_nMaxNodes < nNbNodes)
			SetMaxNodes(nNbNodes);

		for(i= 0; i < nNbNodes; ++i){
			CDString	strType;
			CNode		*pNode= 0;
			int 		nType;

			m_pXml->FindElem("NODE");
			m_pXml->IntoElem();{

				m_pXml->FindElem("TYPE");
				strType= m_pXml->GetData();
				nType= helper_StringToNodeType(strType.m_arBuffer);

				switch(nType){
				case SECLSID_CNodeLblGradualAppend:					pNode= new CNodeLblGradualAppend;				break;
				case SECLSID_CNodeWait:								pNode= new CNodeWait;							break;
				case SECLSID_CNodeWaitRand:							pNode= new CNodeWaitRand;						break;
				case SECLSID_CNodeWaitKey:							pNode= new CNodeWaitKey;						break;
				case SECLSID_CNodeShutScene:						pNode= new CNodeShutScene;						break;
				case SECLSID_CNodeShowPhysObj:						pNode= new CNodeShowPhysObj;					break;
				case SECLSID_CNodeClearLabelGradual:				pNode= new CNodeClearLabelGradual;				break;
				case SECLSID_CNodeSetPosPhysObj:					pNode= new CNodeSetPosPhysObj;					break;
				case SECLSID_CNodeAnimateEntityRandStateRandPos:	pNode= new CNodeAnimateEntityRandStateRandPos;	break;
				case SECLSID_CNodeMoveToPosPhysObj:					pNode= new CNodeMoveToPosPhysObj;				break;
				case SECLSID_CNodeIncVariable:						pNode= new CNodeIncVariable;					break;
				case SECLSID_CNodeStartMusic:						pNode= new CNodeStartMusic;						break;
				}
				pNode->LoadInstance(*m_pXml);
				AddNode(pNode);

			}m_pXml->OutOfElem();
		}
		m_pXml->FindElem("LINK_SECTION");
		m_pXml->IntoElem();{
			for(j= 0; j < nNbNodes; ++j){

				if(m_pXml->FindElem("NODE")){
					m_pXml->IntoElem();{
						m_pXml->FindElem("NB_CHILDS");
						u32 nNbChilds= atoi((m_pXml->GetData()).m_arBuffer);

						GetNode(j)->SetMaxChilds(nNbChilds);

						for(k= 0; k < s32(nNbChilds); ++k){
							m_pXml->FindElem("CHILD");
							GetNode(j)->AddChild(atoi((m_pXml->GetData()).m_arBuffer));
						}
					}m_pXml->OutOfElem();
				}
			}
		}m_pXml->OutOfElem();

		m_pXml->FindElem("NB_ROOTS");
		u32 nNbRoots= atoi((m_pXml->GetData()).m_arBuffer);

		m_pNodeRoot->SetMaxChilds(nNbRoots);
		for(i= 0; u32(i) < nNbRoots; ++i){
			m_pXml->FindElem("ROOT");
			m_pNodeRoot->AddChild(atoi((m_pXml->GetData()).m_arBuffer));
		}
	}
	SAFE_DELETE(m_pXml);
}

void
CSceneEngine::SetMaxNodes(int p_nbNodes)
{
	CNode **temp= new CNode*[p_nbNodes];
	memset(temp, 0, sizeof(CNode*)*p_nbNodes);

	memcpy(temp, m_arNodes, sizeof(CNode*)*m_nMaxNodes);
	SAFE_DELETE_ARRAY(m_arNodes);

	m_arNodes= temp;
	m_nMaxNodes= p_nbNodes;
}

void
CSceneEngine::SetMaxRunningNodes(int p_nbNodes)
{
	CNode **temp= new CNode*[p_nbNodes];
	memset(temp, 0, sizeof(CNode*)*p_nbNodes);

	memcpy(temp, m_arRunningNodes, sizeof(CNode*)*m_nMaxRuningNodes);
	SAFE_DELETE_ARRAY(m_arRunningNodes);

	m_arRunningNodes= temp;
	m_nMaxRuningNodes= p_nbNodes;
}

void
CSceneEngine::AddNode(CNode *p_pNode)
{
	if(m_nMaxNodes == m_nCptNodes)
		SetMaxNodes(1+m_nMaxNodes+(m_nMaxNodes>>1));

	p_pNode->m_nIdx= m_nCptNodes;
	m_arNodes[m_nCptNodes++]= p_pNode;
}

void
CSceneEngine::Run(CNode *p_pNode)
{
	if(m_nMaxRuningNodes == m_nCptRunningNodes)
		SetMaxRunningNodes(1+m_nMaxRuningNodes+(m_nMaxRuningNodes>>1));

	//Move to Running list
	m_arRunningNodes[m_nCptRunningNodes++]= p_pNode;

	p_pNode->Start();
}

void
CSceneEngine::Stop(CNode *p_node)
{
	for(int i= 0; i < m_nCptRunningNodes; ++i){
		if(m_arRunningNodes[i] == p_node){
			m_arRunningNodes[i]->End();

			memcpy(&m_arRunningNodes[i], &m_arRunningNodes[i+1], sizeof(CNode*)*(m_nCptRunningNodes-i-1));
			--m_nCptRunningNodes;
			break;
		}
	}
}

CNode *gPreventLoop= 0;

void
CSceneEngine::StopWithChildsInner(CNode *p_pNode)
{
CNode *pNode;

	if(p_pNode == gPreventLoop)
		return;

	Stop(p_pNode);
	for(int i= 0; i < p_pNode->m_nCptChilds; ++i){
		pNode= m_arNodes[p_pNode->m_arChilds[i]];
		StopWithChildsInner(pNode);
	}
}

void
CSceneEngine::StopWithChilds(CNode *p_pNode)
{
CNode *pNode;

	gPreventLoop= p_pNode;
	Stop(p_pNode);
	for(int i= 0; i < p_pNode->m_nCptChilds; ++i){
		pNode= m_arNodes[p_pNode->m_arChilds[i]];
		StopWithChildsInner(pNode);
	}
}

void
CSceneEngine::Manage(u32 p_timeTick)
{
	CObj::Manage(p_timeTick);

	for(int i= 0; i < m_nCptRunningNodes; ++i){
		if((*m_arRunningNodes[i])(p_timeTick)){
			m_arRunningNodes[i]->End();

			CNode *pThis= m_arRunningNodes[i];
			for(int j= 0; j < pThis->m_nCptChilds; ++j)
				Run(pThis->m_arChilds[j]);

			memcpy(&m_arRunningNodes[i], &m_arRunningNodes[i+1], sizeof(CNode*)*(m_nCptRunningNodes-i-1));
			--m_nCptRunningNodes;
			break;
		}
	}
}
