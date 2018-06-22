#include "lib.h"
#include "libMacros.h"
#include "libMath.h"
#include "dfcPhysObj.h"
#include "dfcApp.h"


CPhysObj::CPhysObj():
TILE_SIZE(g_TheApp->m_nPhysicSizeBlocMap),
m_isShown(true),
m_angleProjected(0),
m_scaleProjected(0.0f),
m_radius(0),
m_isCollidable(false),
m_moveIndirect(0),
m_bTurningKeepVel(false),
m_bTurningKeepVelAngle(false),
m_velMaxIndirect(0.0f),
m_accelIndirect(0.0f),
m_deccelIndirect(0.0f),
m_fSartToDeccel(0.0f),
m_forceApplied(0),
m_bCamRelative(false)
{
	m_stHit.objHit1			= 0;
	m_stHit.objHit2			= 0;
	m_stHit.lineHitten		= 0;
	m_stHit.boxHittenBack	= 0;
	m_stHit.angle			= 0;
	m_stHit.idHitten		= 0;

	memset(m_pos, 0, 3*sizeof(float));
	memset(m_angle, 0, 3*sizeof(float));
	memset(m_scale, 0, 3*sizeof(float));
	memset(m_posProjected, 0, 2*sizeof(float));
	memset(m_vel, 0, 3*sizeof(float));
	memset(m_accel, 0, 3*sizeof(float));
	memset(m_velAngle, 0, 3*sizeof(float));
	memset(m_accelAngle, 0, 3*sizeof(float));
	memset(m_posTo, 0, 3*sizeof(float));
	memset(m_angleTo, 0, 3*sizeof(float));
	memset(m_posMap, 0, 3*sizeof(int));
};

void
CPhysObj::LoadInstance(const char* p_dataSource)
{
	CObj::LoadInstance(p_dataSource);

	SAFE_DELETE(m_pXml);
}

void
CPhysObj::Manage(u32 p_nTimeTick)
{
	CObj::Manage(p_nTimeTick);

	//Manage MoveToPos
	if(m_moveIndirect & 0x3F){
		//Detect the vector
		float	dif[3]= {m_posTo[0] - m_pos[0], m_posTo[1] - m_pos[1], m_posTo[2] - m_pos[2]};
		//Scale the vector by (AccelFactor*velMax)
		float	len= CMngMath::VectorGetLenght(dif);
		float	lenVel= CMngMath::VectorGetLenght(m_vel);
		float	lenDesired= m_accelIndirect*m_velMaxIndirect;

		//If vel > velMax -> Scale to velMax
		//Travelling speed after accel & before decell
		if(len > m_fSartToDeccel){
			if(lenVel >= m_velMaxIndirect){
				CMngMath::VectorScale(m_vel, m_vel, m_velMaxIndirect/lenVel);
			//Accelerating
			}else{
				//If has reached last point is just turning... put back velMax else put accl
				if((lenVel == 0) && m_bTurningKeepVel){
					m_bTurningKeepVel= false;
					CMngMath::VectorScale(dif, dif, m_velMaxIndirect/len);
				}else{
					CMngMath::VectorScale(dif, dif, lenDesired/len);
				}
				//Add to velocity
				m_vel[0]+= dif[0];
				m_vel[1]+= dif[1];
				m_vel[2]+= dif[2];
			}
		//Had reach deceleration point
		}else{
			CMngMath::VectorScale(m_vel, m_vel, m_deccelIndirect);
		}
	}else{
		m_vel[0]+= m_accel[0];
		m_vel[1]+= m_accel[1];
		m_vel[2]+= m_accel[2];
	}

	m_pos[0]+= m_vel[0];
	m_pos[1]+= m_vel[1];
	m_pos[2]+= m_vel[2];
	m_posMap[0]= int((m_pos[0])/TILE_SIZE);
	m_posMap[1]= int((m_pos[1])/TILE_SIZE);
	m_posMap[2]= int((m_pos[2])/TILE_SIZE);


	if(m_moveIndirect & 0x3F){
		float	dif[3]= {m_posTo[0] - m_pos[0], m_posTo[1] - m_pos[1], m_posTo[2] - m_pos[2]};
		float	len= CMngMath::VectorGetLenght(dif);

		//If destination < Lenght(m_vel) pixel snap to it (whatever)
		if((len == 0) || len < CMngMath::VectorGetLenght(m_vel)){
			m_pos[0]= m_posTo[0];
			m_pos[1]= m_posTo[1];
			m_pos[2]= m_posTo[2];
			m_posMap[0]= int((m_pos[0])/TILE_SIZE);
			m_posMap[1]= int((m_pos[1])/TILE_SIZE);
			m_posMap[2]= int((m_pos[2])/TILE_SIZE);
			if((m_forceApplied & 0x3F) != (m_moveIndirect & 0x3F)){
				m_vel[0]= 0;
				m_vel[1]= 0;
				m_vel[2]= 0;
				if((m_forceApplied & 0x3F))
					m_bTurningKeepVel= true;
			}
			m_moveIndirect&= ~0x3F;

			switch(m_forceApplied & 0x3F){
			case 0x01:	MoveToMap(m_posMap[0]+1, m_posMap[1], m_posMap[2], m_accelIndirect, m_velMaxIndirect, 1, m_deccelIndirect);	break;
			case 0x02:	MoveToMap(m_posMap[0]-1, m_posMap[1], m_posMap[2], m_accelIndirect, m_velMaxIndirect, 1, m_deccelIndirect);	break;
			case 0x04:	MoveToMap(m_posMap[0], m_posMap[1]+1, m_posMap[2], m_accelIndirect, m_velMaxIndirect, 1, m_deccelIndirect);	break;
			case 0x08:	MoveToMap(m_posMap[0], m_posMap[1]-1, m_posMap[2], m_accelIndirect, m_velMaxIndirect, 1, m_deccelIndirect);	break;
			default:	break;
			}
		}
	}

	//Manage RotationMoveTo
	if(m_moveIndirect & 0x3F00){
		//Detect the vector
		float	difNormal[3]= {m_angleTo[0] - m_angle[0], m_angleTo[1] - m_angle[1], m_angleTo[2] - m_angle[2]};
		float	difPlus1Cycle[3]= {m_angleTo[0]+(2*PI) - m_angle[0], m_angleTo[1]+(2*PI) - m_angle[1], m_angleTo[2]+(2*PI) - m_angle[2]};
		float	difMinus1Cycle[3]= {m_angleTo[0]-(2*PI) - m_angle[0], m_angleTo[1]-(2*PI) - m_angle[1], m_angleTo[2]-(2*PI) - m_angle[2]};
		float	dif[3];

		dif[0]= (ABS(difNormal[0]) < ABS(difPlus1Cycle[0]))? (ABS(difNormal[0]) < ABS(difMinus1Cycle[0]))? difNormal[0]:difMinus1Cycle[0]:difPlus1Cycle[0];
		dif[1]= (ABS(difNormal[1]) < ABS(difPlus1Cycle[1]))? (ABS(difNormal[1]) < ABS(difMinus1Cycle[1]))? difNormal[1]:difMinus1Cycle[1]:difPlus1Cycle[1];
		dif[2]= (ABS(difNormal[2]) < ABS(difPlus1Cycle[2]))? (ABS(difNormal[2]) < ABS(difMinus1Cycle[2]))? difNormal[2]:difMinus1Cycle[2]:difPlus1Cycle[2];

		//Scale the vector by (AccelFactor*velMax)
		float	len= CMngMath::VectorGetLenght(dif);
		float	lenVel= CMngMath::VectorGetLenght(m_velAngle);
		float	lenDesired= m_accelIndirect*m_velMaxIndirect;

		//If has reached last point is just turning... put back velMax else put accl
		if((lenVel == 0) && m_bTurningKeepVelAngle){
			m_bTurningKeepVelAngle= false;
			CMngMath::VectorScale(dif, dif, float(m_velMaxIndirect/len));
		}else{
			CMngMath::VectorScale(dif, dif, float(lenDesired/len));
		}

		//Add to velocity
		m_velAngle[0]+= dif[0];
		m_velAngle[1]+= dif[1];
		m_velAngle[2]+= dif[2];

		//If vel > velMax -> Scale to velMax
		len= CMngMath::VectorGetLenght(m_velAngle);
		if(len > m_velMaxIndirect){
			CMngMath::VectorScale(m_velAngle, m_velAngle, float(m_velMaxIndirect/len));
		}

		//Calculate lenght require to decelerate
		//Calculate lenght left
		//If LenghtRequireToDecel < LenghtLeft
		//Scale vel by decelFactor
	}else{
		m_velAngle[0]+= m_accelAngle[0];
		m_velAngle[1]+= m_accelAngle[1];
		m_velAngle[2]+= m_accelAngle[2];
	}
	m_angle[0]+= m_velAngle[0];
	m_angle[1]+= m_velAngle[1];
	m_angle[2]+= m_velAngle[2];


	if(m_moveIndirect & 0x3F00){
		float	difNormal[3]= {m_angleTo[0] - m_angle[0], m_angleTo[1] - m_angle[1], m_angleTo[2] - m_angle[2]};
		float	difPlus1Cycle[3]= {m_angleTo[0]+(2*PI) - m_angle[0], m_angleTo[1]+(2*PI) - m_angle[1], m_angleTo[2]+(2*PI) - m_angle[2]};
		float	difMinus1Cycle[3]= {m_angleTo[0]-(2*PI) - m_angle[0], m_angleTo[1]-(2*PI) - m_angle[1], m_angleTo[2]-(2*PI) - m_angle[2]};
		float	dif[3];

		dif[0]= (ABS(difNormal[0]) < ABS(difPlus1Cycle[0]))? (ABS(difNormal[0]) < ABS(difMinus1Cycle[0]))? difNormal[0]:difMinus1Cycle[0]:difPlus1Cycle[0];
		dif[1]= (ABS(difNormal[1]) < ABS(difPlus1Cycle[1]))? (ABS(difNormal[1]) < ABS(difMinus1Cycle[1]))? difNormal[1]:difMinus1Cycle[1]:difPlus1Cycle[1];
		dif[2]= (ABS(difNormal[2]) < ABS(difPlus1Cycle[2]))? (ABS(difNormal[2]) < ABS(difMinus1Cycle[2]))? difNormal[2]:difMinus1Cycle[2]:difPlus1Cycle[2];

		float	len= CMngMath::VectorGetLenght(dif);

		//If destination < Lenght(m_vel) pixel snap to it (whatever)
		if((len == 0) || len < CMngMath::VectorGetLenght(m_velAngle)){
			m_angle[0]= m_angleTo[0];
			m_angle[1]= m_angleTo[1];
			m_angle[2]= m_angleTo[2];
			if((m_forceApplied & 0x3F00) != (m_moveIndirect & 0x3F00)){
				m_velAngle[0]= 0;
				m_velAngle[1]= 0;
				m_velAngle[2]= 0;
				if((m_forceApplied & 0x3F00))
					m_bTurningKeepVelAngle= true;
			}
			m_moveIndirect&= ~0x3F00;
		}
	}

	if(m_stHit.lineHitten)
		m_stHit.lineHitten= 0;
	m_forceApplied= 0;
}

void
CPhysObj::Update(void)
{
	if(!m_isShown)
		return;

	CObj::Update();

}

float
CPhysObj::GetParentPosX(void)
{
	if(!m_pParent)
		return 0;
	else if(m_pParent->IsKindOf(CLSID_CPhysObj))
		return ((CPhysObj*)m_pParent)->GetPosX() + ((CPhysObj*)m_pParent)->GetParentPosX();
	else
		return 0;
}

float
CPhysObj::GetParentPosY(void)
{
	if(!m_pParent)
		return 0;
	else if(m_pParent->IsKindOf(CLSID_CPhysObj))
		return ((CPhysObj*)m_pParent)->GetPosY() + ((CPhysObj*)m_pParent)->GetParentPosY();
	else
		return 0;
}

float
CPhysObj::GetParentPosZ(void)
{
	if(!m_pParent)
		return 0;
	else if(m_pParent->IsKindOf(CLSID_CPhysObj))
		return ((CPhysObj*)m_pParent)->GetPosZ() + ((CPhysObj*)m_pParent)->GetParentPosZ();
	else
		return 0;
}

u8
CPhysObj::GetParentAngleX(void)
{
	if(!m_pParent)
		return 0;
	else if(m_pParent->IsKindOf(CLSID_CPhysObj))
		return ((CPhysObj*)m_pParent)->GetAngleX() + ((CPhysObj*)m_pParent)->GetParentAngleX();
	else
		return 0;
}

u8
CPhysObj::GetParentAngleY(void)
{
	if(!m_pParent)
		return 0;
	else if(m_pParent->IsKindOf(CLSID_CPhysObj))
		return ((CPhysObj*)m_pParent)->GetAngleY() + ((CPhysObj*)m_pParent)->GetParentAngleY();
	else
		return 0;
}

u8
CPhysObj::GetParentAngleZ(void)
{
	if(!m_pParent)
		return 0;
	else if(m_pParent->IsKindOf(CLSID_CPhysObj))
		return ((CPhysObj*)m_pParent)->GetAngleZ() + ((CPhysObj*)m_pParent)->GetParentAngleZ();
	else
		return 0;
}

void
CPhysObj::MoveToMap(int p_nPosX, int p_nPosY, int p_nPosZ, float p_accel, float p_velMax, float p_fStarttoDeccel, float p_deccel)
{
	m_forceApplied= ((((p_nPosX-m_posMap[0])>0)?0x01:0x0)|
					(((p_nPosX-m_posMap[0])<0)?0x02:0x0)|
					(((p_nPosY-m_posMap[1])>0)?0x04:0x0)|
					(((p_nPosY-m_posMap[1])<0)?0x08:0x0)|
					(((p_nPosZ-m_posMap[2])>0)?0x10:0x0)|
					(((p_nPosZ-m_posMap[2])<0)?0x20:0x0));

	if((m_moveIndirect & 0x3F) && ((m_moveIndirect & 0x3F) != (m_forceApplied & 0x3F))){
		return;
	}

	MoveToFreely(p_nPosX*TILE_SIZE, p_nPosY*TILE_SIZE, p_nPosZ, p_accel, p_velMax, p_fStarttoDeccel, p_deccel);
}

void
CPhysObj::MoveToFreely(int p_nPosX, int p_nPosY, int p_nPosZ, float p_accel, float p_velMax, float p_fStarttoDeccel, float p_deccel)
{
	m_velMaxIndirect= p_velMax;
	m_accelIndirect= p_accel;
	m_deccelIndirect= p_deccel;
	m_moveIndirect= ((((p_nPosX-m_pos[0])>0)?0x0001:0x0)|
					(((p_nPosX-m_pos[0])<0)?0x0002:0x0)|
					(((p_nPosY-m_pos[1])>0)?0x0004:0x0)|
					(((p_nPosY-m_pos[1])<0)?0x0008:0x0)|
					(((p_nPosZ-m_pos[2])>0)?0x0010:0x0)|
					(((p_nPosZ-m_pos[2])<0)?0x0020:0x0));

	m_posTo[0]= p_nPosX;
	m_posTo[1]= p_nPosY;
	m_posTo[2]= p_nPosZ;

	float	dif[3]= {m_posTo[0] - m_pos[0], m_posTo[1] - m_pos[1], m_posTo[2] - m_pos[2]};
	//Scale the vector by (AccelFactor*velMax)
	float	len= CMngMath::VectorGetLenght(dif);
	m_fSartToDeccel= len * (float(1.0)-p_fStarttoDeccel);
}

void
CPhysObj::RotationMoveToFreely(float p_nAngleX, float p_nAngleY, float p_nAngleZ, float p_accel, float p_velMax, float p_deccel)
{
	m_velMaxIndirect= p_velMax;
	m_accelIndirect= p_accel;
	m_deccelIndirect= p_deccel;
	m_moveIndirect= ((((p_nAngleX-m_angle[0])>0)?0x0100:0x0)|
					(((p_nAngleX-m_angle[0])<0)?0x0200:0x0)|
					(((p_nAngleY-m_angle[1])>0)?0x0400:0x0)|
					(((p_nAngleY-m_angle[1])<0)?0x0800:0x0)|
					(((p_nAngleZ-m_angle[2])>0)?0x1000:0x0)|
					(((p_nAngleZ-m_angle[2])<0)?0x2000:0x0));

	m_angleTo[0]= p_nAngleX;
	m_angleTo[1]= p_nAngleY;
	m_angleTo[2]= p_nAngleZ;
}

void
CPhysObj::SetCamRelative(bool p_bCamRelative)
{
	m_bCamRelative= p_bCamRelative;

	for(u32 i= 0; i < m_nbChild; ++i)
		if(m_arChilds[i]->IsKindOf(CLSID_CPhysObj))
			((CPhysObj*)m_arChilds[i])->SetCamRelative(p_bCamRelative);

}
