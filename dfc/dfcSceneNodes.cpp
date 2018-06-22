#include "lib.h"
#include "libMacros.h"
#include "libMath.h"
#include "libMngInput.h"
#include "dfcSceneEngine.h"
#include "dfcSceneNodes.h"
#include "dfcApp.h"
#include "dfcDictionary.h"

//////////////////////////////////////////////
//Node

CNode::CNode():
m_bRunning(false),
m_nMaxChilds(0),
m_nCptChilds(0),
m_arChilds(0)
{
}

CNode::~CNode()
{
	delete []m_arChilds;
}

void
CNode::Start()
{
	m_bRunning= true;
}

void
CNode::End()
{
	m_bRunning= false;
}

void
CNode::SetMaxChilds(int p_nbChilds)
{
int *temp= new int[p_nbChilds];

	memset(temp, 0, sizeof(int)*p_nbChilds);

	memcpy(temp, m_arChilds, sizeof(int)*m_nMaxChilds);
	SAFE_DELETE_ARRAY(m_arChilds);

	m_arChilds= temp;
	m_nMaxChilds= p_nbChilds;
}

void
CNode::AddChild(int p_idxNode)
{
	if(m_nMaxChilds == m_nCptChilds)
		SetMaxChilds(1+m_nMaxChilds+(m_nMaxChilds>>1));

	m_arChilds[m_nCptChilds++]= p_idxNode;
}


CNodeLblGradualAppend::CNodeLblGradualAppend():
CNode(),
m_strToAdd(""),
m_lblDst(0)
{
}

CNodeLblGradualAppend::~CNodeLblGradualAppend()
{
}

void
CNodeLblGradualAppend::LoadInstance(CXml &p_refXml)
{
	p_refXml.FindElem("LABEL_OBJECT");

	CDString temp= p_refXml.GetData();
	m_lblDst= (CLabelGradual*)g_TheApp->GetCurContext()->FindByID(temp.m_arBuffer);

	if(!m_lblDst)
		m_lblDst= (CLabelGradual*)g_TheApp->GetCurContext()->FindByIDUnder(CSceneEngine::singleton, temp.m_arBuffer);

	ASSERT3(m_lblDst, "Cannot find:%s", temp.m_arBuffer);

	p_refXml.FindElem("TEXT_ID");
	m_strToAdd= GET_STRING((p_refXml.GetData()).m_arBuffer);

	p_refXml.FindElem("SPEED");
	m_nSpeed= float(atof((p_refXml.GetData()).m_arBuffer));
}

void
CNodeLblGradualAppend::Start()
{
	CNode::Start();

	m_lblDst->AppendText(m_strToAdd.m_arBuffer);
	m_lblDst->SetSpeed(m_nSpeed);
}

void
CNodeLblGradualAppend::End()
{
	CNode::End();
}

bool
CNodeLblGradualAppend::operator()(u32 p_timeTick)
{
	return m_lblDst->IsDone();
}



CNodeWait::CNodeWait():
CNode(),
m_nMSWait(0)
{
}

CNodeWait::~CNodeWait()
{
}

void
CNodeWait::LoadInstance(CXml &p_refXml)
{
	p_refXml.FindElem("DELAY");
	m_nMSWaitMax= atoi((p_refXml.GetData()).m_arBuffer);
}

void
CNodeWait::Start()
{
	CNode::Start();

	m_nMSWait= m_nMSWaitMax;
}

bool
CNodeWait::operator()(u32 p_timeTick)
{
	m_nMSWait-= p_timeTick;
	return (m_nMSWait <= 0);
}


CNodeWaitRand::CNodeWaitRand():
CNodeWait(),
m_nRandMin(0),
m_nRandMax(0)
{
}

CNodeWaitRand::~CNodeWaitRand()
{
}

void
CNodeWaitRand::LoadInstance(CXml &p_refXml)
{
	p_refXml.FindElem("MIN");
	m_nRandMin= atoi((p_refXml.GetData()).m_arBuffer);

	p_refXml.FindElem("MAX");
	m_nRandMax= atoi((p_refXml.GetData()).m_arBuffer);
}

void
CNodeWaitRand::Start()
{
	SetDelay(m_nRandMin + (rand()%(m_nRandMax-m_nRandMin)));

	CNodeWait::Start();
}

CNodeWaitKey::CNodeWaitKey():
CNode(),
m_nButton(0)
{
}

CNodeWaitKey::~CNodeWaitKey()
{
}

void
CNodeWaitKey::LoadInstance(CXml &p_refXml)
{
	p_refXml.FindElem("KEY");
	m_nButton= GetMngInput()->AsciiToID((p_refXml.GetData()).m_arBuffer);
}

bool
CNodeWaitKey::operator()(u32 p_timeTick)
{
	return GetMngInput()->IsLogicKeyDown(m_nButton);
}

CNodeShutScene::CNodeShutScene():
CNode(),
m_pIdxNodeToShut(-1)
{
}

CNodeShutScene::~CNodeShutScene()
{
}

void
CNodeShutScene::LoadInstance(CXml &p_refXml)
{
	p_refXml.FindElem("NODE");
	m_pIdxNodeToShut= atoi((p_refXml.GetData()).m_arBuffer);
}

bool
CNodeShutScene::operator()(u32 p_timeTick)
{
	WARNING2(m_pIdxNodeToShut != -1, "Node CNodeShutScene Unitialise");

	if(m_pIdxNodeToShut != -1)
		CSceneEngine::singleton->StopWithChilds(m_pIdxNodeToShut);

	return true;
}

CNodeShowPhysObj::CNodeShowPhysObj():
CNode(),
m_pPhysObj(0),
m_bShow(true),
m_nState(0)
{
}

CNodeShowPhysObj::~CNodeShowPhysObj()
{
}

void
CNodeShowPhysObj::LoadInstance(CXml &p_refXml)
{

	p_refXml.FindElem("PHYS_OBJECT");
	CDString temp= p_refXml.GetData();
	m_pPhysObj= (CPhysObj *)g_TheApp->GetCurContext()->FindByID(temp.m_arBuffer);

	if(!m_pPhysObj)
		m_pPhysObj= (CPhysObj*)g_TheApp->GetCurContext()->FindByIDUnder(CSceneEngine::singleton, temp.m_arBuffer);

	ASSERT3(m_pPhysObj, "Cannot find:%s", temp.m_arBuffer);

	p_refXml.FindElem("SHOW");
	m_bShow= atoi((p_refXml.GetData()).m_arBuffer);

	if(p_refXml.FindElem("STATE"))
		m_nState= atoi((p_refXml.GetData()).m_arBuffer);
}

bool
CNodeShowPhysObj::operator()(u32 p_timeTick)
{
	if(m_pPhysObj->IsKindOf(CLSID_CEntity))
		((CEntity*)m_pPhysObj)->SetState(m_nState);
	m_pPhysObj->Show(m_bShow);
	return true;
}


CNodeClearLabelGradual::CNodeClearLabelGradual():
CNode(),
m_pLabel(0)
{
}

CNodeClearLabelGradual::~CNodeClearLabelGradual()
{
}

void
CNodeClearLabelGradual::LoadInstance(CXml &p_refXml)
{
	p_refXml.FindElem("LABEL_OBJECT");
	CDString temp= p_refXml.GetData();
	m_pLabel= (CLabelGradual *)g_TheApp->GetCurContext()->FindByID(temp.m_arBuffer);

	if(!m_pLabel)
		m_pLabel= (CLabelGradual*)g_TheApp->GetCurContext()->FindByIDUnder(CSceneEngine::singleton, temp.m_arBuffer);

	ASSERT3(m_pLabel, "Cannot find:%s", temp.m_arBuffer);
}

bool
CNodeClearLabelGradual::operator()(u32 p_timeTick)
{
	m_pLabel->Clear();
	return true;
}

CNodeSetPosPhysObj::CNodeSetPosPhysObj():
CNode(),
m_pPhysObj(0)
{
}
CNodeSetPosPhysObj::~CNodeSetPosPhysObj()
{
}

void
CNodeSetPosPhysObj::LoadInstance(CXml &p_refXml)
{
	p_refXml.FindElem("PHYS_OBJECT");
	CDString temp= p_refXml.GetData();
	m_pPhysObj= (CPhysObj *)g_TheApp->GetCurContext()->FindByID(temp.m_arBuffer);

	if(!m_pPhysObj)
		m_pPhysObj= (CPhysObj*)g_TheApp->GetCurContext()->FindByIDUnder(CSceneEngine::singleton, temp.m_arBuffer);

	ASSERT3(m_pPhysObj, "Cannot find:%s", temp.m_arBuffer);

	p_refXml.FindElem("POS_X");
	m_pos[0]= (atoi((p_refXml.GetData()).m_arBuffer));

	p_refXml.FindElem("POS_Y");
	m_pos[1]= (atoi((p_refXml.GetData()).m_arBuffer));

	p_refXml.FindElem("POS_Z");
	m_pos[2]= (atoi((p_refXml.GetData()).m_arBuffer));
}

bool
CNodeSetPosPhysObj::operator()(u32 p_timeTick)
{
	m_pPhysObj->SetPos(m_pos);
	return true;
}


CNodeAnimateEntityRandStateRandPos::CNodeAnimateEntityRandStateRandPos():
CNode(),
m_pEntity(0)
{
}

CNodeAnimateEntityRandStateRandPos::~CNodeAnimateEntityRandStateRandPos()
{
}

void
CNodeAnimateEntityRandStateRandPos::LoadInstance(CXml &p_refXml)
{
	p_refXml.FindElem("ENTITY_OBJECT");
	CDString temp= p_refXml.GetData();
	m_pEntity= (CEntity *)g_TheApp->GetCurContext()->FindByID(temp.m_arBuffer);

	if(!m_pEntity)
		m_pEntity= (CEntity*)g_TheApp->GetCurContext()->FindByIDUnder(CSceneEngine::singleton, temp.m_arBuffer);

	ASSERT3(m_pEntity, "Cannot find:%s", temp.m_arBuffer);

	p_refXml.FindElem("MIN_X");
	m_posMinX= atoi((p_refXml.GetData()).m_arBuffer);

	p_refXml.FindElem("MAX_X");
	m_posMaxX= atoi((p_refXml.GetData()).m_arBuffer);

	p_refXml.FindElem("MIN_Y");
	m_posMinY= atoi((p_refXml.GetData()).m_arBuffer);

	p_refXml.FindElem("MAX_Y");
	m_posMaxY= atoi((p_refXml.GetData()).m_arBuffer);
}

void
CNodeAnimateEntityRandStateRandPos::Start()
{
	CNode::Start();
	m_pEntity->Show();
	m_pEntity->SetState(rand()%m_pEntity->GetNbState());
	m_pEntity->SetPosX((m_posMinX+rand()%(m_posMaxX-m_posMinX)));
	m_pEntity->SetPosY((m_posMinY+rand()%(m_posMaxY-m_posMinY)));
}

bool
CNodeAnimateEntityRandStateRandPos::operator()(u32 p_timeTick)
{
	return (m_pEntity->GetFrame() == (m_pEntity->GetNbFrame()-1));

}

void
CNodeAnimateEntityRandStateRandPos::End()
{
	CNode::End();
	m_pEntity->Hide();
}

CNodeMoveToPosPhysObj::CNodeMoveToPosPhysObj():
CNode(),
m_pPhysObj(0)
{
}
CNodeMoveToPosPhysObj::~CNodeMoveToPosPhysObj()
{
}

void
CNodeMoveToPosPhysObj::LoadInstance(CXml &p_refXml)
{
	p_refXml.FindElem("PHYS_OBJECT");
	CDString temp= p_refXml.GetData();
	m_pPhysObj= (CPhysObj *)g_TheApp->GetCurContext()->FindByID(temp.m_arBuffer);

	if(!m_pPhysObj)
		m_pPhysObj= (CPhysObj*)g_TheApp->GetCurContext()->FindByIDUnder(CSceneEngine::singleton, temp.m_arBuffer);

	ASSERT3(m_pPhysObj, "Cannot find:%s", temp.m_arBuffer);

	p_refXml.FindElem("SPEED_MAX");
	m_nSpeedMax= float(atof((p_refXml.GetData()).m_arBuffer));

	p_refXml.FindElem("POS_X");
	m_pos[0]= (atoi((p_refXml.GetData()).m_arBuffer));

	p_refXml.FindElem("POS_Y");
	m_pos[1]= (atoi((p_refXml.GetData()).m_arBuffer));

	p_refXml.FindElem("POS_Z");
	m_pos[2]= (atoi((p_refXml.GetData()).m_arBuffer));
}

void
CNodeMoveToPosPhysObj::Start()
{
	m_pPhysObj->MoveToFreely(m_pos[0], m_pos[1], m_pos[2], .5f, m_nSpeedMax, 1, 0.5f);
}

bool
CNodeMoveToPosPhysObj::operator()(u32 p_timeTick)
{
	return !m_pPhysObj->IsMoving();
}


CNodeIncVariable::CNodeIncVariable():
CNode(),
m_pVarU32(0)
{
}

CNodeIncVariable::~CNodeIncVariable()
{
}

void
CNodeIncVariable::LoadInstance(CXml &p_refXml)
{
}

bool
CNodeIncVariable::operator()(u32 p_timeTick)
{
	if(m_pVarU32)
		++(*m_pVarU32);

	return true;
}

CNodeStartMusic::CNodeStartMusic():
CNode(),
m_pMusic(0)
{
}

CNodeStartMusic::~CNodeStartMusic()
{
}

void
CNodeStartMusic::LoadInstance(CXml &p_refXml)
{
	p_refXml.FindElem("MUSIC");
	CDString temp= p_refXml.GetData();
	m_pMusic= (CMusic *)g_TheApp->GetCurContext()->FindByID(temp.m_arBuffer);

	if(!m_pMusic)
		m_pMusic= (CMusic*)g_TheApp->GetCurContext()->FindByIDUnder(CSceneEngine::singleton, temp.m_arBuffer);

	ASSERT3(m_pMusic, "Cannot find:%s", temp.m_arBuffer);
}

bool
CNodeStartMusic::operator()(u32 p_timeTick)
{
	m_pMusic->Play();

	return true;
}
