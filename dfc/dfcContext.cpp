#include "lib.h"
#include "libMacros.h"
#include "libMath.h"
#include "libMngInput.h"
#include "libMngSound.h"
#include "dfcSound.h"
#include "dfcMusic.h"
#include "dfcSceneEngine.h"


#include "dfcContext.h"
#include "dfcApp.h"
#include "dfcEntity.h"
#include "dfc3DEntity.h"
#include "dfc3DMaze.h"
#include "dfcLabel.h"
#include "dfcButton.h"
#include "dfcSceneEngine.h"
#include "dfcSceneNodes.h"

CContext::CContext(const char* p_pRes):
CObj(p_pRes),
#ifdef _DEBUG
m_delay(0),
m_nDbgLevel(DBG_ALL),
#endif
m_pCurBackgroundObject(0),
m_pMouse(0),
m_arLines(0),
m_cptLines(0),
m_maxLines(0)
{
	m_pRootFocus= this;

#ifdef _DEBUG
	memset(m_tickDiff, 0x0, sizeof(char) * 128);
#endif
}

CContext::~CContext()
{
	FreeInstance();
}

void
CContext::LoadInstance(const char* p_dataSource)
{
	CObj::LoadInstance(p_dataSource);

	m_camera2D= new CCamera2D;
	m_camera2D->SetPos(0, 0, -30);
	m_camera2D->SetTextID("Camera2D");
	m_camera2D->SetAngle(0, 0, 0);
	m_camera2D->SetCurrent();

	m_camera3D= new CCamera3D;
	m_camera3D->SetPos(0, 0, -30);
	m_camera3D->SetTextID("Camera3D");
//	m_camera3D->SetAngle(3.14159f, 0, 0);
	m_camera3D->SetAngle(0, 0, 0);
	m_camera3D->SetCurrent();

	AddChild(m_camera2D, false);
	AddChild(m_camera3D, false);
	m_nbTimer= 0;

	SAFE_DELETE(m_pXml);
}

void
CContext::FreeInstance(void)
{
s32 i;

	for(i= 0; i < (s32)m_nbChild; ++i){
		SAFE_DELETE(m_arChilds[i]);
	}

	m_nbChild	= 0;
	m_pMouse	= 0;

	SAFE_DELETE(m_pCurBackgroundObject);
	SAFE_DELETE_ARRAY(m_arLines);
}

void
CContext::AddLine(int p_x1, int p_y1, int p_x2, int p_y2, u32 p_group)
{
	if((m_cptLines+1) > m_maxLines){
		TLine *pTemp= new TLine[m_maxLines+(m_maxLines>>1)];

		memcpy(pTemp, m_arLines, sizeof(TLine)*m_maxLines);
		delete [] m_arLines;
		m_arLines= pTemp;
		m_maxLines= m_maxLines+(m_maxLines>>1);
	}

	m_arLines[m_cptLines].m_start[0]= p_x1;
	m_arLines[m_cptLines].m_start[1]= p_y1;
	m_arLines[m_cptLines].m_end[0]	= p_x2;
	m_arLines[m_cptLines].m_end[1]	= p_y2;
	m_arLines[m_cptLines].m_group	= p_group;
	m_arLines[m_cptLines].m_angle	= u32(atan2(p_y1-p_y2, p_x1-p_x2)*256/6.2832f);

	m_cptLines++;
}

void
CContext::OnCollideObj2Obj(THit *p_hitStruct, CPhysObj *p_pPhysObj1, CPhysObj *p_pPhysObj2)
{
float	vel1[3]	={p_pPhysObj1->m_vel[0], p_pPhysObj1->m_vel[1], 0};
float	vel2[3]	={p_pPhysObj2->m_vel[0], p_pPhysObj2->m_vel[1], 0};
float	nor1[3]	={-p_hitStruct->norm[0], -p_hitStruct->norm[1], 0};
float	para1[3]	={-nor1[1], nor1[0], 0};
float	nor2[3]	={-nor1[0], -nor1[1], 0};
float	para2[3]	={-para1[0], -para1[1], 0};
float	scale;
float	tempR2[3];
float	tempR1[3];

	scale		= CMngMath::VectorDot(vel1, para1);
	para1[0]	= para1[0] * scale;
	para1[1]	= para1[1] * scale;

	scale		= CMngMath::VectorDot(vel1, nor1);
	tempR1[0]	= nor1[0] * scale;
	tempR1[1]	= nor1[1] * scale;

	scale		= CMngMath::VectorDot(vel2, para2);
	para2[0]	= para2[0] * scale;
	para2[1]	= para2[1] * scale;

	scale		= CMngMath::VectorDot(vel2, nor2);
	tempR2[0]	= nor2[0] * scale;
	tempR2[1]	= nor2[1] * scale;

	p_pPhysObj1->m_vel[0]	= para1[0] + tempR2[0];
	p_pPhysObj1->m_vel[1]	= para1[1] + tempR2[1];
	p_pPhysObj2->m_vel[0]	= para2[0] + tempR1[0];
	p_pPhysObj2->m_vel[1]	= para2[1] + tempR1[1];
}

void
CContext::OnCollideObj2Line(THit *p_hitStruct, CPhysObj *p_pPhysObj)
{
float *pos				= p_pPhysObj->GetPos();
float velNormalized[3]	= {p_pPhysObj->GetVelX(), p_pPhysObj->GetVelY(), 0};
float len				= (velNormalized[0]*velNormalized[0]+velNormalized[1]*velNormalized[1]);
float nor[3]			= {-p_hitStruct->norm[1], p_hitStruct->norm[0], 0};
float scale;
float u[2];
float v[2];
float numer;
float denom;

	velNormalized[0]/= len;
	velNormalized[1]/= len;

	pos[0]= p_hitStruct->ptHit[0];
	pos[1]= p_hitStruct->ptHit[1];

	//Calculate Bounce
	numer	= CMngMath::VectorDot(velNormalized, nor);
	denom	= CMngMath::VectorDot(nor, nor);
	scale	= numer / denom;
	u[0]	= nor[0] * scale;
	u[1]	= nor[1] * scale;
	v[0]	= (u[0] - (velNormalized[0])) + u[0];
	v[1]	= (u[1] - (velNormalized[1])) + u[1];

	p_pPhysObj->SetVelX(v[0] * len);
	p_pPhysObj->SetVelY(v[1] * len);
}

bool
CContext::IsCollidingObj2Obj(THit *p_hitStruct, CPhysObj *p_pPhysObj1, CPhysObj *p_pPhysObj2)
{/*
float	dpx		= p_pPhysObj1->m_pos[0] - p_pPhysObj2->m_pos[0];
float	dpy		= p_pPhysObj1->m_pos[1] - p_pPhysObj2->m_pos[1];
float	lenPSqr	= (dpx*dpx) + (dpy*dpy);

float	dvpx	= (p_pPhysObj1->m_pos[0]+p_pPhysObj1->m_vel[0]) - (p_pPhysObj2->m_pos[0]+p_pPhysObj2->m_vel[0]);
float	dvpy	= (p_pPhysObj1->m_pos[1]+p_pPhysObj1->m_vel[1]) - (p_pPhysObj2->m_pos[1]+p_pPhysObj2->m_vel[1]);
float	lenVPSqr= (dvpx*dvpx) + (dvpy*dvpy);

//float	dvx		= p_pPhysObj1->m_vel[0]-p_pPhysObj2->m_vel[0];
//float	dvy		= p_pPhysObj1->m_vel[1]-p_pPhysObj2->m_vel[1];

	if(lenVPSqr <= ((p_pPhysObj1->m_radius+p_pPhysObj2->m_radius) * (p_pPhysObj1->m_radius+p_pPhysObj2->m_radius)))
	{
		float dX	= p_pPhysObj1->m_pos[0] - p_pPhysObj2->m_pos[0];
		float dY	= p_pPhysObj1->m_pos[1] - p_pPhysObj2->m_pos[1];
		float len	= CMngMath::VectorGetLenght(&lenPSqr);

		p_hitStruct->ptHit[0]	= p_pPhysObj1->m_pos[0];
		p_hitStruct->ptHit[1]	= p_pPhysObj1->m_pos[1];
		p_hitStruct->norm[0]	= dX/len;
		p_hitStruct->norm[1]	= dY/len;
		p_hitStruct->u2			= 0;
		p_hitStruct->idHitten	= 1;
		return true;

	}
*/	return false;
}

bool
CContext::IsCollidingObj2Line(THit *p_hitStruct, CPhysObj *p_pPhysObj1, TLine *p_pLine)
{
float s_x1	=	p_pPhysObj1->GetPosX();
float s_x2	=	p_pPhysObj1->GetPosX() + p_pPhysObj1->GetVelX();
float s_x3	=	p_pLine->m_start[0];
float s_x4	=	p_pLine->m_end[0];
float s_y1	=	p_pPhysObj1->GetPosY();
float s_y2	=	p_pPhysObj1->GetPosY() + p_pPhysObj1->GetVelY();
float s_y3	=	p_pLine->m_start[1];
float s_y4	=	p_pLine->m_end[1];

	{
		float	y1My3	=	(s_y1-s_y3);
		float	x1Mx3	=	(s_x1-s_x3);
		float	denom	=	((s_y4-s_y3) * (s_x2-s_x1)) - ((s_x4-s_x3) * (s_y2-s_y1));
		float	u1	,u2;
		float	xI1	,yI1;
		float	xI2	,yI2;
		u32		angle;

		if(denom == 0)
			return false;

		float	vel[3]=	{(s_x2-s_x1), (s_y2-s_y1), 0};
		float	normale[3]= {0, 0, 0};

		u1=	(((s_x4-s_x3) * y1My3) - ((s_y4-s_y3) * x1Mx3)) / denom;
		u2=	(((s_x2-s_x1) * y1My3) - ((s_y2-s_y1) * x1Mx3)) / denom;

		if((u1 > 0) && (u2 >= 0) && (u1 <= 1) && (u2 <= 1))
		{
			xI1	=	s_x1 + (u1 * (s_x2 - s_x1));
			yI1	=	s_y1 + (u1 * (s_y2 - s_y1));
			xI2	=	s_x3 + (u2 * (s_x4 - s_x3));
			yI2	=	s_y3 + (u2 * (s_y4 - s_y3));

			angle= p_pLine->m_angle;
			normale[0]= -sin(angle);
			normale[1]= cos(angle);

			if(CMngMath::VectorDot(vel, normale) <= 0)
			{
				p_hitStruct->angle		= angle;
				p_hitStruct->ptHit[0]	= xI1;
				p_hitStruct->ptHit[1]	= yI1;
				p_hitStruct->u1			= u1;
				p_hitStruct->u2			= u2;
				p_hitStruct->norm[0]	= normale[0];
				p_hitStruct->norm[1]	= normale[1];
				p_hitStruct->lineHitten	= p_pLine;
				p_hitStruct->idHitten	= (u8)p_pLine->m_group;
				return true;
			}
		}
	}

	return false;
}

void
CContext::Manage(u32 p_nTimeTick)
{
//	printf("CContext::Manage->\n");
CMngInput	*pMngInput= GetMngInput();
s32 i;
u32 j;

	//Manage Timer
	for(i= 0; i < m_nbTimer; ++i){
	#if defined(_WIN32DX7) || defined(_WIN32OPENGL)
		m_tTimer[i].nDelay-= int(((g_TheApp->m_thisFrameTick - g_TheApp->m_lastFrameTick)*1000)/g_TheApp->m_clockFreq);
	#elif defined(_LINUX)
		m_tTimer[i].nDelay-= int(((g_TheApp->m_thisFrameTick - g_TheApp->m_lastFrameTick)));
	#endif
		if(m_tTimer[i].nDelay < 0){
			m_tTimer[i].nDelay= m_tTimer[i].nDelayMax;
			(this->*(m_tTimer[i].pFct))();
		}
	}

	//Collide Obj
	for(i= 0; u32(i) < m_nbChild; ++i)
	{
		CPhysObj *pPhysObj1= (CPhysObj *)m_arChilds[i];

		if(pPhysObj1 && pPhysObj1->IsKindOf(CLSID_CPhysObj) && pPhysObj1->m_isCollidable)
		{
			for(j= i+1; j < m_nbChild; ++j)
			{
				CPhysObj *pPhysObj2= (CPhysObj *)m_arChilds[j];

				if(pPhysObj2 && pPhysObj2->IsKindOf(CLSID_CPhysObj) && pPhysObj2->m_isCollidable)
				{
					pPhysObj1->SetVelX(pPhysObj1->GetVelX()+pPhysObj1->GetAccelX());
					pPhysObj1->SetVelY(pPhysObj1->GetVelY()+pPhysObj1->GetAccelY());
					pPhysObj1->SetVelZ(pPhysObj1->GetVelZ()+pPhysObj1->GetAccelZ());
					pPhysObj2->SetVelX(pPhysObj2->GetVelX()+pPhysObj2->GetAccelX());
					pPhysObj2->SetVelY(pPhysObj2->GetVelY()+pPhysObj2->GetAccelY());
					pPhysObj2->SetVelZ(pPhysObj2->GetVelZ()+pPhysObj2->GetAccelZ());

					if(IsCollidingObj2Obj(&pPhysObj1->m_stHit, pPhysObj1, pPhysObj2))
						OnCollideObj2Obj(&pPhysObj1->m_stHit, pPhysObj1, pPhysObj2);

					pPhysObj1->SetVelX(pPhysObj1->GetVelX()-pPhysObj1->GetAccelX());
					pPhysObj1->SetVelY(pPhysObj1->GetVelY()-pPhysObj1->GetAccelY());
					pPhysObj1->SetVelZ(pPhysObj1->GetVelZ()-pPhysObj1->GetAccelZ());
					pPhysObj2->SetVelX(pPhysObj2->GetVelX()-pPhysObj2->GetAccelX());
					pPhysObj2->SetVelY(pPhysObj2->GetVelY()-pPhysObj2->GetAccelY());
					pPhysObj2->SetVelZ(pPhysObj2->GetVelZ()-pPhysObj2->GetAccelZ());
				}
			}

			//Find Nearest Line
			int	nearestIdx	= -1;
			float	nearestValue= 1000000000;
			do
			{
				nearestIdx	= -1;
				nearestValue= 1000000000;

				for(j= 0; j < m_cptLines; ++j)
				{
					pPhysObj1->SetVelX(pPhysObj1->GetVelX()+pPhysObj1->GetAccelX());
					pPhysObj1->SetVelY(pPhysObj1->GetVelY()+pPhysObj1->GetAccelY());
					pPhysObj1->SetVelZ(pPhysObj1->GetVelZ()+pPhysObj1->GetAccelZ());

					if((pPhysObj1->m_stHit.lineHitten != &m_arLines[j]) && IsCollidingObj2Line(&pPhysObj1->m_stHit, pPhysObj1, &m_arLines[j]))
					{
						if(pPhysObj1->m_stHit.u2 < nearestValue)
						{
							nearestValue= pPhysObj1->m_stHit.u2;
							nearestIdx	= j;
						}
					}

					pPhysObj1->SetVelX(pPhysObj1->GetVelX()-pPhysObj1->GetAccelX());
					pPhysObj1->SetVelY(pPhysObj1->GetVelY()-pPhysObj1->GetAccelY());
					pPhysObj1->SetVelZ(pPhysObj1->GetVelZ()-pPhysObj1->GetAccelZ());
				}

				if(nearestIdx != -1)
				{
					pPhysObj1->SetVelX(pPhysObj1->GetVelX()+pPhysObj1->GetAccelX());
					pPhysObj1->SetVelY(pPhysObj1->GetVelY()+pPhysObj1->GetAccelY());
					pPhysObj1->SetVelZ(pPhysObj1->GetVelZ()+pPhysObj1->GetAccelZ());

					OnCollideObj2Line(&pPhysObj1->m_stHit, pPhysObj1);

					pPhysObj1->SetVelX(pPhysObj1->GetVelX()-pPhysObj1->GetAccelX());
					pPhysObj1->SetVelY(pPhysObj1->GetVelY()-pPhysObj1->GetAccelY());
					pPhysObj1->SetVelZ(pPhysObj1->GetVelZ()-pPhysObj1->GetAccelZ());
				}
			}while(nearestIdx != -1);
		}
	}

	//Camera 3D need to be managed first
	//Manage Obj
	for(i= 0; u32(i) < m_nbChild; ++i)
		if(m_arChilds[i])
			m_arChilds[i]->Manage(p_nTimeTick);

	//Sort the objects
	for(i= 0; u32(i) < m_nbChild; ++i){
		if(!m_arChilds[i])
			continue;
		if(!m_arChilds[i]->IsKindOf(CLSID_CPhysObj))
			continue;

		for(j= i+1; j < m_nbChild; ++j){
			if(!m_arChilds[i])
				continue;
			if(!m_arChilds[i]->IsKindOf(CLSID_CPhysObj))
				continue;

			CPhysObj *obj1= (CPhysObj *)m_arChilds[i];
			CPhysObj *obj2= (CPhysObj *)m_arChilds[j];
			if((obj2->GetPosZ() + obj2->GetParentPosZ()) > (obj1->GetPosZ() + obj1->GetParentPosZ())){
				m_arChilds[i]= obj2;
				m_arChilds[j]= obj1;
				i= -1;
				break;
			}
		}
	}

	//Move the mouse
	s32	posX;
	s32	posY;

	pMngInput->GetMousePos(posX, posY);

	CButton *pObj= (CButton*)FindAt(posX, posY, ~0);

	while(pObj && !pObj->m_isShown){
		CButton *pSecondObj= (CButton*)FindAt(posX, posY, pObj->GetIntPosZ());

		if(pSecondObj)
			pObj= pSecondObj;
		else
			break;
	}

	pObj= (CButton*)FindAtByRtti(posX, posY, ~0, CLSID_CButton);
	if(pObj && pObj->IsKindOf(CLSID_CButton) && m_pMouse){
		if((pObj->GetState() == CButton::stateIdle) || (pObj->GetState() == CButton::stateSelected)){
			pObj->m_nOldState= pObj->GetState();
			pObj->SetState(CButton::stateOver);
			pObj->m_nDelay= 10;
		}
	}

	if(m_pMouse){
		m_pMouse->SetPos(posX, posY, 0);

		if(pObj && pObj->IsKindOf(CLSID_CEntity)){
			if(pObj->OnMsgMap[CEntity::mapMouseOver])
				(this->*(((CEntity*)pObj)->OnMsgMap[CEntity::mapMouseOver]))(pObj);
		}
		else
			m_pMouse->SetState(0);
	}

	//Test the mouse for click
#ifdef _LINUX
	if(m_pMouse && pMngInput->IsMouseLogicKeyDown(0)){
		pObj= (CButton*)FindAtByRtti(posX, posY, -1, CLSID_CEntity);

		Click(pObj);
	}
#endif
}

u32		*g_pPixel;
bool	*g_arAlreadyDrawn;


void RecursiveDebugSceneEngine(s32 p_x, s32 p_y, CSceneEngine* p_pScnEng, CNode* p_pCur)
{
CMngGraphic	*mngGraphic	= GetMngGraphic();
long color= 0x0000FF00;
int i;

	for(i= 0; i < p_pScnEng->m_nCptRunningNodes; ++i){
		if(p_pScnEng->m_arRunningNodes[i] == p_pCur)
			color= 0x00FF0000;

	}

	mngGraphic->DrawCircle_locked(g_pPixel, p_x, p_y, 4, color);
	for(i= 0; i < p_pCur->m_nCptChilds; ++i){
		if(!g_arAlreadyDrawn[p_pCur->m_arChilds[i]]){
			s32 xDest= (p_x-(((p_pCur->m_nCptChilds-1)*10)/2))+i*10;
			s32 yDest= p_y+8;

			if(yDest > 480){
				yDest= 100;
				xDest+= 100;
			}

			g_arAlreadyDrawn[p_pCur->m_arChilds[i]]= true;
			mngGraphic->DrawLine_locked(g_pPixel, p_x, p_y, xDest, yDest, 0x0000FF00);
			RecursiveDebugSceneEngine(xDest, yDest, p_pScnEng, p_pScnEng->GetNode(p_pCur->m_arChilds[i]));
		}
	}
}

void
CContext::Update(void)
{
//	printf("CContext::Update->\n");
CMngGraphic	*mngGraphic	= GetMngGraphic();
CMng3D		*mng3D		= GetMng3D();
u32			i;


//	printf("mngGraphic->BeginScene\n");
	mngGraphic->BeginScene();
//	printf("mng3D->Clear\n");
	mng3D->Clear(0);

	for(i= 0; i < m_nbChild; ++i){
		m_arChilds[i]->Update();

		//When encounter the camera don't draw anything else
		if(m_arChilds[i]->IsKindOf(CLSID_CCamera2D))
			break;
	}

#ifdef _DEBUG
	u32		*pPixel;
	char	toto[128];
	/////////////
	//Debug Lines
	if(m_nDbgLevel & DBG_COL_SYS){/*TODO printing this is not working anymore
		pPixel= mngGraphic->LockSurface(mngGraphic->GetBB());
		u32	halfWidth=	mngGraphic->GetWidth(mngGraphic->GetBB())/2;
		u32	halfHeight=	mngGraphic->GetHeight(mngGraphic->GetBB())/2;

		for(i= 0; i < m_cptLines; ++i)
			mngGraphic->DrawLine_locked(pPixel, s32(m_arLines[i].m_start[0] + halfWidth), s32(m_arLines[i].m_start[1] + halfHeight), s32(m_arLines[i].m_end[0] + halfWidth), s32(m_arLines[i].m_end[1] + halfHeight),  0x00FF0000);

		mngGraphic->UnLockSurface(mngGraphic->GetBB(), pPixel);*/
	}

	///////////
	//Debug FPS
	if(m_nDbgLevel & DBG_FPS){
		#if defined(_WIN32DX7) || defined(_WIN32OPENGL)
			float fps= 1000.0f/((float)((g_TheApp->m_thisFrameTick - g_TheApp->m_lastFrameTick)*1000)/g_TheApp->m_clockFreq);
		#elif defined(_LINUX)
			float fps= 1000.0/((g_TheApp->m_thisFrameTick - g_TheApp->m_lastFrameTick)+1);
		#elif defined(_GP2X)
			float fps= 60;
		#endif
		if((m_delay++) == 60){
			m_delay= 0;
			sprintf(m_tickDiff, "|FF00FF00|%f", fps);
		}

		pPixel= mngGraphic->LockSurface(mngGraphic->GetBB());
		mngGraphic->DrawLine_locked(pPixel, 0, 11, s32(fps), 11, 0x000000FF);
		mngGraphic->DrawLine_locked(pPixel, 0, 12, s32(fps), 12, 0x000000FF);
		mngGraphic->DrawLine_locked(pPixel, 0, 13, s32(fps), 13, 0x000000FF);
		mngGraphic->UnLockSurface(mngGraphic->GetBB(), pPixel);
		CFont::m_pAllFontInstance[0]->PutS(0, 8, m_tickDiff);
	}

	/////////////
	//Debug Sound
	if(m_nDbgLevel & DBG_SOUND){
		#if defined(_WIN32DX7)
		CMngSound	*mngSound= (CMngSound*)GetMngSound();

		/////////////////////////
		//Debug Streaming Playing
		pPixel= mngGraphic->LockSurface(mngGraphic->GetBB());

		if(pPixel)
			mngGraphic->DrawLine_locked(pPixel, 0, 472, 640, 472, 1, 0x00999999);

		if(pPixel && mngSound->IsOn()){
			u32					posRead= 0;
			float				posWrite1= 640*(float(mngSound->m_lastWritingSB)/(2048.0f*128.0f));
			u32					posWrite2= 0;

	//		mngSound->m_pStreamBuffer->GetCurrentPosition(&posRead, &posWrite2);

			posRead		= u32(640.0f*(float(posRead)/(2048.0f*128.0f)));
			posWrite2	= u32(640.0f*(float(posWrite2)/(2048.0f*128.0f)));

			mngGraphic->PutPixel_locked(pPixel, float(posRead + 0), float(470 + 0), 0x000000FF);
			mngGraphic->PutPixel_locked(pPixel, float(posRead + 0), float(470 + 1), 0x000000FF);
			mngGraphic->PutPixel_locked(pPixel, float(posRead + 0), float(470 + 2), 0x000000FF);
			mngGraphic->PutPixel_locked(pPixel, float(posRead + 0), float(470 + 3), 0x000000FF);
			mngGraphic->PutPixel_locked(pPixel, float(posRead + 0), float(470 + 4), 0x000000FF);
			mngGraphic->PutPixel_locked(pPixel, float(posRead + 1), float(470 + 1), 0x000000FF);
			mngGraphic->PutPixel_locked(pPixel, float(posRead + 1), float(470 + 2), 0x000000FF);
			mngGraphic->PutPixel_locked(pPixel, float(posRead + 1), float(470 + 3), 0x000000FF);
			mngGraphic->PutPixel_locked(pPixel, float(posRead + 2), float(470 + 1), 0x000000FF);
			mngGraphic->PutPixel_locked(pPixel, float(posRead + 2), float(470 + 2), 0x000000FF);
			mngGraphic->PutPixel_locked(pPixel, float(posRead + 2), float(470 + 3), 0x000000FF);
			mngGraphic->PutPixel_locked(pPixel, float(posRead + 3), float(470 + 2), 0x000000FF);
			mngGraphic->PutPixel_locked(pPixel, float(posRead + 4), float(470 + 2), 0x000000FF);

			mngGraphic->PutPixel_locked(pPixel, float(posWrite1 + 0), float(470 + 2), 0x00FF0000);
			mngGraphic->PutPixel_locked(pPixel, float(posWrite1 + 1), float(470 + 1), 0x00FF0000);
			mngGraphic->PutPixel_locked(pPixel, float(posWrite1 + 1), float(470 + 2), 0x00FF0000);
			mngGraphic->PutPixel_locked(pPixel, float(posWrite1 + 1), float(470 + 3), 0x00FF0000);
			mngGraphic->PutPixel_locked(pPixel, float(posWrite1 + 2), float(470 + 0), 0x00FF0000);
			mngGraphic->PutPixel_locked(pPixel, float(posWrite1 + 2), float(470 + 1), 0x00FF0000);
			mngGraphic->PutPixel_locked(pPixel, float(posWrite1 + 2), float(470 + 2), 0x00FF0000);
			mngGraphic->PutPixel_locked(pPixel, float(posWrite1 + 2), float(470 + 3), 0x00FF0000);
			mngGraphic->PutPixel_locked(pPixel, float(posWrite1 + 2), float(470 + 4), 0x00FF0000);
			mngGraphic->PutPixel_locked(pPixel, float(posWrite1 + 3), float(470 + 1), 0x00FF0000);
			mngGraphic->PutPixel_locked(pPixel, float(posWrite1 + 3), float(470 + 2), 0x00FF0000);
			mngGraphic->PutPixel_locked(pPixel, float(posWrite1 + 3), float(470 + 3), 0x00FF0000);
			mngGraphic->PutPixel_locked(pPixel, float(posWrite1 + 4), float(470 + 2), 0x00FF0000);

			mngGraphic->PutPixel_locked(pPixel, float(posWrite2 + 0), float(470 + 2), 0x00AA0000);
			mngGraphic->PutPixel_locked(pPixel, float(posWrite2 + 1), float(470 + 1), 0x00AA0000);
			mngGraphic->PutPixel_locked(pPixel, float(posWrite2 + 1), float(470 + 2), 0x00AA0000);
			mngGraphic->PutPixel_locked(pPixel, float(posWrite2 + 1), float(470 + 3), 0x00AA0000);
			mngGraphic->PutPixel_locked(pPixel, float(posWrite2 + 2), float(470 + 0), 0x00AA0000);
			mngGraphic->PutPixel_locked(pPixel, float(posWrite2 + 2), float(470 + 1), 0x00AA0000);
			mngGraphic->PutPixel_locked(pPixel, float(posWrite2 + 2), float(470 + 2), 0x00AA0000);
			mngGraphic->PutPixel_locked(pPixel, float(posWrite2 + 2), float(470 + 3), 0x00AA0000);
			mngGraphic->PutPixel_locked(pPixel, float(posWrite2 + 2), float(470 + 4), 0x00AA0000);
			mngGraphic->PutPixel_locked(pPixel, float(posWrite2 + 3), float(470 + 1), 0x00AA0000);
			mngGraphic->PutPixel_locked(pPixel, float(posWrite2 + 3), float(470 + 2), 0x00AA0000);
			mngGraphic->PutPixel_locked(pPixel, float(posWrite2 + 3), float(470 + 3), 0x00AA0000);
			mngGraphic->PutPixel_locked(pPixel, float(posWrite2 + 4), float(470 + 2), 0x00AA0000);
		}
		mngGraphic->UnLockSurface(mngGraphic->GetBB(), pPixel);

		#endif //
	}

	/////////////////
	//Debug Mouse Pos
	if(m_nDbgLevel & DBG_MOUSE){
		s32 posX, posY;

		GetMngInput()->GetMousePos(posX, posY);

		sprintf(toto, "|FFFFFF00|Mouse %3d %3d", int(posX), int(posY));

		CFont::m_pAllFontInstance[0]->PutS(0, 16, toto);
	}

	///////////////
	//Debug Objects
	if(m_nDbgLevel & DBG_OBJ){
		for(i= 0; i < m_nbChild; ++i){
			sprintf(toto, "%s: %ld:%ld:%ld", m_arChilds[i]->GetTextID(), ((CPhysObj*)m_arChilds[i])->GetIntPosX(), ((CPhysObj*)m_arChilds[i])->GetIntPosY(), ((CPhysObj*)m_arChilds[i])->GetIntPosZ());
			CFont::m_pAllFontInstance[0]->PutS(0, (22+(i*6)), toto);
		}
	}

	///////////////////////
	//Debug Scenaric Engine
	if(m_nDbgLevel & DBG_SCENE){
		if(CSceneEngine::singleton){
			g_pPixel= mngGraphic->LockSurface(mngGraphic->GetBB());
			g_arAlreadyDrawn= new bool[CSceneEngine::singleton->m_nCptNodes];

			memset(g_arAlreadyDrawn, 0, sizeof(bool)*CSceneEngine::singleton->m_nCptNodes);
			RecursiveDebugSceneEngine(320, 6, CSceneEngine::singleton, CSceneEngine::singleton->GetNodeRoot());
			delete [] g_arAlreadyDrawn;
			mngGraphic->UnLockSurface(mngGraphic->GetBB(), pPixel);
		}
	}

#endif

	mng3D->Flip();
	mngGraphic->EndScene(0);
//	printf("<-CContext::Update\n");
}

u32
CContext::AddChild(CObj *p_pObject, bool p_bTransformSetRelativity)
{
	if(!p_pObject)
		return u32(-1);

	if(p_pObject->Rtti() == CLSID_CBackground){
		u32		lastRet= u32(-1);
		char	strID[32];
		char	temp[32];

		SAFE_DELETE(m_pCurBackgroundObject);

		m_pCurBackgroundObject= (CBackground*)p_pObject;

		u32 i;

		for(i= 0; i < m_pCurBackgroundObject->m_nbLayer; ++i){
			memset(strID, 0, 32);
			memset(temp, 0, 32);

			sprintf(strID, "%s_%d", p_pObject->GetTextID(), int(i));

			m_pCurBackgroundObject->m_pLayers[i]->SetTextID(strID);
		}

		for(i= 0; i < m_pCurBackgroundObject->m_nbLayer; ++i)
			lastRet= CObj::AddChild(m_pCurBackgroundObject->m_pLayers[i], false);

		return lastRet;
	}else{
		return CObj::AddChild(p_pObject, false);
	}
}

CObj*
CContext::FindAt(s32 p_nX, s32 p_nY, s32 p_nZ)
{
CObj	*pNearest	= 0;
float	nearestZ	= 1000000;

	if(!m_pRootFocus->m_nbChild)
		return 0;

	for(u32 i= 0; i < m_pRootFocus->m_nbChild; ++i){
		if(!m_pRootFocus->m_arChilds[i])
			continue;

		if(m_pRootFocus->m_arChilds[i]->IsKindOf(CLSID_CEntity)){
			CPhysObj *pPhysObj= (CEntity*)m_pRootFocus->m_arChilds[i];

			if(!helper_PointIsInRect((int)p_nX, (int)p_nY, (int)(pPhysObj->GetIntPosX()+pPhysObj->GetParentPosX()), (int)(pPhysObj->GetIntPosY() + pPhysObj->GetParentPosY()), ((CEntity*)pPhysObj)->GetWidth(), ((CEntity*)pPhysObj)->GetHeight()))
				continue;
			if(pPhysObj == m_pMouse)
				continue;
			if((pPhysObj->GetPosZ() + pPhysObj->GetParentPosZ()) <= p_nZ)
				continue;
			if(!pPhysObj->m_isShown)
				continue;

			if((pPhysObj->GetPosZ() + pPhysObj->GetParentPosZ()) < nearestZ){
				nearestZ= (pPhysObj->GetPosZ()+pPhysObj->GetParentPosZ());
				pNearest= pPhysObj;
			}
		}
	}

	return pNearest;
}

CObj*
CContext::FindAtByRtti(s32 p_nX, s32 p_nY, s32 p_nZ, DFC_CLASSID p_idClass)
{
CObj	*pNearest	= 0;
float	nearestZ	= 1000000;

	if(!m_pRootFocus->m_nbChild)
		return 0;

	for(u32 i= 0; i < m_pRootFocus->m_nbChild; ++i){
		if(!m_pRootFocus->m_arChilds[i])
			continue;

		if(m_pRootFocus->m_arChilds[i]->IsKindOf(CLSID_CEntity)){
			CEntity *pEntity= (CEntity*)m_pRootFocus->m_arChilds[i];

			if(!helper_PointIsInRect((int)p_nX, (int)p_nY, (int)(pEntity->GetIntPosX() - pEntity->GetHotSpotX()+pEntity->GetParentPosX()), (int)(pEntity->GetIntPosY()-pEntity->GetHotSpotY()+pEntity->GetParentPosY()), pEntity->GetWidth(), pEntity->GetHeight()))
				continue;
			if(pEntity == m_pMouse)
				continue;
			if((pEntity->GetPosZ()+pEntity->GetParentPosZ()) <= p_nZ)
				continue;
			if(!pEntity->m_isShown)
				continue;
			if(!pEntity->IsKindOf(p_idClass))
				continue;

			if((pEntity->GetPosZ()+pEntity->GetParentPosZ()) < nearestZ){
				nearestZ= (pEntity->GetPosZ()+pEntity->GetParentPosZ());
				pNearest= pEntity;
			}
		}
	}

	return pNearest;
}

CObj*
CContext::FindByIDUnder(CObj* p_root, const char* p_szID)
{
CObj* pFound= 0;

	if(p_root->GetTextID() && strcmp(p_root->GetTextID(), p_szID) == 0)
		return p_root;

	if(!p_root->m_nbChild)
		return 0;

	for(u32 i= 0; i < p_root->m_nbChild; ++i){
		pFound= FindByIDUnder(p_root->m_arChilds[i], p_szID);

		if(pFound)
			return pFound;
	}
	return 0;
}

CObj*
CContext::FindByID(const char * p_szID)
{
CObj *ret= FindByIDUnder(m_pRootFocus, p_szID);

	if(!ret && m_pRootFocus->m_pParent)
		ret= FindByIDUnder(m_pRootFocus->m_pParent, p_szID);

	return ret;
}

CObj*
CContext::FindByRtti(DFC_CLASSID p_nID)
{
	if(!m_nbChild)
		return 0;

	for(u32 i= 0; i < m_nbChild; ++i){
		if(!m_arChilds[i])
			continue;
		if(m_arChilds[i]->IsKindOf(p_nID))
			return m_arChilds[i];
	}

	return 0;
}

void
CContext::SetMouse(CEntity *p_pObjMouse)
{
	m_pMouse= p_pObjMouse;
}

void
CContext::AddMap(u32 p_type, const char * p_idControl, void (CObj::*p_fctMapped)(CEntity*))
{
CEntity* pButton= (CEntity*)FindByID(p_idControl);

	pButton->OnMsgMap[p_type]= p_fctMapped;
}

void
CContext::AddMap(u32 p_type, CEntity* p_pButton, void (CObj::*p_fctMapped)(CEntity*))
{
	p_pButton->OnMsgMap[p_type]= p_fctMapped;
}

void
CContext::Click(CEntity *p_pObj)
{
	if(!p_pObj)
		return;

	if(p_pObj->IsKindOf(CLSID_CButton)){
		p_pObj->SetState(CButton::stateClicked);
		((CButton*)p_pObj)->m_nDelay= 10;
	}

	if(p_pObj->OnMsgMap[CEntity::mapMouseClick])
		(this->*(p_pObj->OnMsgMap[CEntity::mapMouseClick]))(p_pObj);
}

void
CContext::Click(const char *p_idControl)
{
	Click((CButton*)FindByID(p_idControl));
}

int
CContext::AddTimer(int p_nDelay, void (CContext::*p_pFct)(void))
{
	m_tTimer[m_nbTimer].nDelayMax= p_nDelay;
	m_tTimer[m_nbTimer].nDelay	= p_nDelay;
	m_tTimer[m_nbTimer++].pFct	= p_pFct;


	ASSERT2(m_nbTimer < 4, "Only 4 Timer for now");

	return m_nbTimer-1;
}

void
CContext::ResetTimers(void)
{
	m_nbTimer= 0;
}
