#include "lib.h"
#include "libMacros.h"
#include "libMngGraphic.h"
#include "dfcEntity.h"
#include "dfcCamera.h"

#include "libMath.h"
#include "libMngSound.h"
#include "dfcApp.h"
#include "dfcContext.h"
#include "dfc3DEntity.h"
#include "dfcLabel.h"
#include "dfcButton.h"
#include "libMngInput.h"
#include "libMngResIO.h"


CEntity::CEntity():
m_arFrames(0),
m_arState(0),
m_pCurAnim(0),
m_nbFrame(0),
m_zoom(1.0f),
m_arSpeed(0)
{
	for(int i= 0; i < CEntity::maxObjMap; ++i)
		OnMsgMap[i]= 0;

	m_isCollidable	= false;
	m_radius		= 8;
}


CEntity::CEntity(CEntity& p_dataSource):
CPhysObj(),
m_nbFrame(p_dataSource.m_nbFrame),
m_nbState(p_dataSource.m_nbState),
m_zoom(p_dataSource.m_zoom)
{
CMngImageIO	*mngImageIO	= GetMngImageIO();
u32 i;

	m_arFrames= new TRuntimeFrame[m_nbFrame+1];
	m_arState= new TRuntimeFrame*[m_nbState+1];
	m_arSpeed= new u32[m_nbState+1];

	for(i= 0; i < m_nbFrame; ++i){
		mngImageIO->Copy(p_dataSource.m_arFrames[i].pData, &m_arFrames[i].pData, &m_arFrames[i].nWidth, &m_arFrames[i].nHeight);
		m_arFrames[i].nHotSpotX= p_dataSource.m_arFrames[i].nHotSpotX;
		m_arFrames[i].nHotSpotY= p_dataSource.m_arFrames[i].nHotSpotY;
	}

	for(i= 0; i < m_nbState; ++i)
		m_arState[i]= &m_arFrames[p_dataSource.m_arState[i]-(&p_dataSource.m_arFrames[0])];

	m_arState[m_nbState]= &m_arFrames[p_dataSource.m_arState[m_nbState]-(&p_dataSource.m_arFrames[0])];

	for(i= 0; i < m_nbState; ++i)
		m_arSpeed[i]= p_dataSource.m_arSpeed[i];

	m_pCurAnim	= m_arState[m_curState= 0];
	m_speed		= float(float(m_arSpeed[0]));

	for(i= 0; i < CEntity::maxObjMap; ++i)
		OnMsgMap[i]= 0;
}

CEntity::~CEntity()
{
CMngImageIO	*mngImageIO	= GetMngImageIO();
s32			i;

	for(i= 0; i < (s32)m_nbFrame; ++i)
		mngImageIO->Close(&m_arFrames[i].pData);

	SAFE_DELETE_ARRAY(m_arFrames);
	SAFE_DELETE_ARRAY(m_arState);
	SAFE_DELETE_ARRAY(m_arSpeed);
}

void
CEntity::LoadTGA(const char* p_dataSource, int p_idxDest)
{
CMngImageIO	*mngImageIO	= GetMngImageIO();

	mngImageIO->Open(p_dataSource, &m_arFrames[p_idxDest].pData, &m_arFrames[p_idxDest].nWidth, &m_arFrames[p_idxDest].nHeight, false);
}

void
CEntity::LoadInstance(const char* p_dataSource)
{
	CObj::LoadInstance(p_dataSource);

	m_pXml->FindElem("NB_FRAMES");
	m_nbFrame= atoi((m_pXml->GetData()).m_arBuffer);

	u32		i;

	m_arFrames= new TRuntimeFrame[m_nbFrame+1];

	for(i= 0; i < m_nbFrame; ++i){
		m_pXml->FindElem("FRAME");
		m_pXml->IntoElem();{

			m_pXml->FindElem("FILENAME");
			LoadTGA((m_pXml->GetData()).m_arBuffer, i);

			if(m_pXml->FindElem("POS_X"))
				m_arFrames[i].nHotSpotX= atoi((m_pXml->GetData()).m_arBuffer);
			else
				m_arFrames[i].nHotSpotX= m_arFrames[i].nWidth>>1;

			if(m_pXml->FindElem("POS_Y"))
				m_arFrames[i].nHotSpotY= atoi((m_pXml->GetData()).m_arBuffer);
			else
				m_arFrames[i].nHotSpotX= m_arFrames[i].nHeight>>1;

		}m_pXml->OutOfElem();
	}

	m_pXml->FindElem("NB_STATES");
	m_nbState= atoi((m_pXml->GetData()).m_arBuffer);

	m_arState= new TRuntimeFrame*[m_nbState+1];
	m_arSpeed= new u32[m_nbState+1];

	for(i= 0; i < m_nbState; ++i){
		m_pXml->FindElem("STATE");
		m_pXml->IntoElem();{

			m_pXml->FindElem("FLAG");
			m_arState[i]= &m_arFrames[atoi((m_pXml->GetData()).m_arBuffer)];

			m_pXml->FindElem("SPEED");
			m_arSpeed[i]= atoi((m_pXml->GetData()).m_arBuffer)<<8;
		}m_pXml->OutOfElem();

	}

	m_arState[m_nbState]= &m_arFrames[m_nbFrame];

	m_pCurAnim	= m_arState[m_curState= 0];
	m_indexFrame= 0.0f;
	m_speed		= float(m_arSpeed[0]<<8);


	SAFE_DELETE(m_pXml);
}


void
CEntity::Manage(u32 p_nTimeTick)
{
	CPhysObj::Manage(p_nTimeTick);

//	m_indexFrame+= (m_speed*p_nTimeTick) / 1000;
//	m_indexFrame+= m_speed*60;
	m_indexFrame+= (m_speed*16) / 1000;
	if(&m_pCurAnim[int(m_indexFrame)] >= m_arState[m_curState+1])
		m_indexFrame= 0;
}

void
CEntity::Update(void)
{
CMngGraphic	*pRenderer	= GetMngGraphic();

	if(!m_isShown)
		return;

	CCamera2D::GetCurrent()->Project(this);

	if(m_bCamRelative){
		m_posProjected[0]= (float)(-CCamera2D::GetCurrent()->GetPosX() + GetPosX() + GetParentPosX() + (pRenderer->GetWidth(pRenderer->GetBB()) / 2));
		m_posProjected[1]= (float)(-CCamera2D::GetCurrent()->GetPosY() + GetPosY() + GetParentPosY() + (pRenderer->GetHeight(pRenderer->GetBB()) / 2));
	}
	pRenderer->DrawFrame(pRenderer->GetBB(), m_posProjected[0], m_posProjected[1], 0, m_zoom, 0, &m_pCurAnim[int(m_indexFrame)]);

	CPhysObj::Update();
/*
#ifdef _DEBUG
	if(m_isCollidable){
		u32 *pPixel= pRenderer->LockSurface(pRenderer->GetBB());
		u32	halfWidth=	pRenderer->GetWidth(pRenderer->GetBB())/2;
		u32	halfHeight=	pRenderer->GetHeight(pRenderer->GetBB())/2;

		pRenderer->DrawCircle_locked(pPixel, s32(halfWidth+GetParentPosX().m_nDec+GetPosX().m_nDec), s32(halfHeight+GetParentPosY().m_nDec+GetPosY().m_nDec), m_radius, 0x00FF0000);
		pRenderer->UnLockSurface(pRenderer->GetBB(), pPixel);
	}
#endif //_DEBUG
*/
}

void
CEntity::SetState(u32 p_nState)
{
	if(p_nState >= m_nbState)
		return;
	if(m_curState == p_nState)
		return;

	m_pCurAnim	= m_arState[m_curState= p_nState];
	m_speed		= float(m_arSpeed[m_curState]);
}

void
CEntity::SetFrame(u32 p_nFrame)
{
	m_indexFrame= (float)p_nFrame;
	if(&m_pCurAnim[int(m_indexFrame)] >= m_arState[m_curState+1]){
		int maxShouldBe= m_arState[m_curState+1]-m_arState[m_curState];
		m_indexFrame= (int(m_indexFrame) % maxShouldBe);
	}
}

u32
CEntity::GetWidth(void)
{
	return m_arFrames[int(m_indexFrame)].nWidth;
}

u32
CEntity::GetHeight(void)
{
	return m_arFrames[int(m_indexFrame)].nHeight;
}
