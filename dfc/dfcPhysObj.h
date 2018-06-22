#ifndef _PHYSOBJECT_MNG_H_
#define _PHYSOBJECT_MNG_H_

#include "dfcObj.h"
#include "libMath.h"

class CPhysObj;

typedef struct tagTHit
{
	CPhysObj	*objHit1;
	CPhysObj	*objHit2;
	void		*lineHitten;
	void		*boxHittenBack;
	float		ptHit[3];
	float		norm[3];
	float		u1;
	float		u2;
	float		timeRemaining;
	float		angle;
	u8			idHitten;
}THit;


class CPhysObj : public CObj
{
public :
	CPhysObj();
	virtual ~CPhysObj(){};
	IMPLEMENT_RTTI(PhysObj, Obj);

	//From CObject
	virtual void LoadInstance(const char* p_dataSource);
	virtual void Manage(u32 p_nTimeTick);
	virtual void Update(void);

	virtual void Hide(void){m_isShown= false;}
	virtual void Show(bool p_show= true){m_isShown= p_show;}

	virtual u32 GetWidth(void)	{return 0;};
	virtual u32 GetHeight(void)	{return 0;};

	//From CPhysObject
	void SetPosX(s32 p_fPos)	{m_pos[0]= p_fPos;};
	void SetPosY(s32 p_fPos)	{m_pos[1]= p_fPos;};
	void SetPosZ(s32 p_fPos)	{m_pos[2]= p_fPos;};
	void SetPosX(u32 p_fPos)	{m_pos[0]= p_fPos;};
	void SetPosY(u32 p_fPos)	{m_pos[1]= p_fPos;};
	void SetPosZ(u32 p_fPos)	{m_pos[2]= p_fPos;};
	void SetPosX(int p_fPos)	{m_pos[0]= p_fPos;};
	void SetPosY(int p_fPos)	{m_pos[1]= p_fPos;};
	void SetPosZ(int p_fPos)	{m_pos[2]= p_fPos;};
	void SetPos(int p_nPosX, int p_nPosY, int p_nPosZ)
	{
		m_pos[0]= p_nPosX;
		m_pos[1]= p_nPosY;
		m_pos[2]= p_nPosZ;
	};
	void SetPosX(float p_fPos)	{m_pos[0]= p_fPos;};
	void SetPosY(float p_fPos)	{m_pos[1]= p_fPos;};
	void SetPosZ(float p_fPos)	{m_pos[2]= p_fPos;};
	void SetPosX(float& p_fPos)	{m_pos[0]= p_fPos;};
	void SetPosY(float& p_fPos)	{m_pos[1]= p_fPos;};
	void SetPosZ(float& p_fPos)	{m_pos[2]= p_fPos;};
	void SetPos(float& p_nPosX, float& p_nPosY, float& p_nPosZ)
	{
		m_pos[0]= p_nPosX;
		m_pos[1]= p_nPosY;
		m_pos[2]= p_nPosZ;
	};
	void SetPos(float *p_fPos)
	{
		m_pos[0]= p_fPos[0];
		m_pos[1]= p_fPos[1];
		m_pos[2]= p_fPos[2];
	};

	void SetPos(int *p_nPos)
	{
		m_pos[0]= p_nPos[0];
		m_pos[1]= p_nPos[1];
		m_pos[2]= p_nPos[2];
	};

	float* GetPos(void)	{return m_pos;};
	float& GetPosX(void)	{return m_pos[0];};
	float& GetPosY(void)	{return m_pos[1];};
	float& GetPosZ(void)	{return m_pos[2];};
	s32 GetIntPosX(void)	{return s32(m_pos[0]);};
	s32 GetIntPosY(void)	{return s32(m_pos[1]);};
	s32 GetIntPosZ(void)	{return s32(m_pos[2]);};

	int* GetMapPos(void)	{return m_posMap;};
	int& GetMapPosX(void)	{return m_posMap[0];};
	int& GetMapPosY(void)	{return m_posMap[1];};
	int& GetMapPosZ(void)	{return m_posMap[2];};

	float GetParentPosX(void);
	float GetParentPosY(void);
	float GetParentPosZ(void);

	void SetAngleX(float p_fAngle)	{m_angle[0]= p_fAngle;};
	void SetAngleY(float p_fAngle)	{m_angle[1]= p_fAngle;};
	void SetAngleZ(float p_fAngle)	{m_angle[2]= p_fAngle;};
	void SetAngle(float p_nAngleX, float p_nAngleY, float p_nAngleZ)
	{
		m_angle[0]= p_nAngleX;
		m_angle[1]= p_nAngleY;
		m_angle[2]= p_nAngleZ;
	};
	void SetAngle(float *p_fAngle)
	{
		m_angle[0]= p_fAngle[0];
		m_angle[1]= p_fAngle[1];
		m_angle[2]= p_fAngle[2];
	};

	float* 	GetAngle(void)	{return m_angle;};
	float& 	GetAngleX(void)	{return m_angle[0];};
	float& 	GetAngleY(void)	{return m_angle[1];};
	float& 	GetAngleZ(void)	{return m_angle[2];};

	u8 GetParentAngleX(void);
	u8 GetParentAngleY(void);
	u8 GetParentAngleZ(void);

	void SetScaleX(float p_fScale)	{m_scale[0]= p_fScale;};
	void SetScaleY(float p_fScale)	{m_scale[1]= p_fScale;};
	void SetScaleZ(float p_fScale)	{m_scale[2]= p_fScale;};
	void SetScale(float p_nScaleX, float p_nScaleY, float p_nScaleZ)
	{
		m_scale[0]= p_nScaleX;
		m_scale[1]= p_nScaleY;
		m_scale[2]= p_nScaleZ;
	};
	void SetScale(float *p_fScale)
	{
		m_scale[0]= p_fScale[0];
		m_scale[1]= p_fScale[1];
		m_scale[2]= p_fScale[2];
	};

	float* GetScale(void)	{return m_scale;};
	float& GetScaleX(void)	{return m_scale[0];};
	float& GetScaleY(void)	{return m_scale[1];};
	float& GetScaleZ(void)	{return m_scale[2];};

	void SetVelX(float p_vel)	{m_vel[0]= p_vel;};
	void SetVelY(float p_vel)	{m_vel[1]= p_vel;};
	void SetVelZ(float p_vel)	{m_vel[2]= p_vel;};
	void SetVel(float p_velX, float p_velY,float p_velZ)
	{
		m_vel[0]= p_velX;
		m_vel[1]= p_velY;
		m_vel[2]= p_velZ;
	};
	void SetVel(float *p_vecVel)
	{
		m_vel[0]= p_vecVel[0];
		m_vel[1]= p_vecVel[1];
		m_vel[2]= p_vecVel[2];
	};

	float* GetVel(void)	{return m_vel;};
	float& GetVelX(void)	{return m_vel[0];};
	float& GetVelY(void)	{return m_vel[1];};
	float& GetVelZ(void)	{return m_vel[2];};

	void SetAccelX(float p_accel)	{m_accel[0]= p_accel;};
	void SetAccelY(float p_accel)	{m_accel[1]= p_accel;};
	void SetAccelZ(float p_accel)	{m_accel[2]= p_accel;};
	void SetAccel(float p_accelX, float p_accelY, float p_accelZ)
	{
		m_accel[0]= p_accelX;
		m_accel[1]= p_accelY;
		m_accel[2]= p_accelZ;
	};
	void SetAccel(float *p_vecAccel)
	{
		m_accel[0]= p_vecAccel[0];
		m_accel[1]= p_vecAccel[1];
		m_accel[2]= p_vecAccel[2];
	};

	float* GetAccel(void)	{return m_accel;};
	float& GetAccelX(void)	{return m_accel[0];};
	float& GetAccelY(void)	{return m_accel[1];};
	float& GetAccelZ(void)	{return m_accel[2];};

	void MoveToMap(int p_nPosX, int p_nPosY, int p_nPosZ, float p_accel, float p_velMax, float p_fStarttoDeccel, float p_deccel);
	void MoveToFreely(int p_nPosX, int p_nPosY, int p_nPosZ, float p_accel, float p_velMax, float p_fStarttoDeccel, float p_deccel);
	void RotationMoveToFreely(float p_nAngleX, float p_nAngleY, float p_nAngleZ, float p_accel, float p_velMax, float p_deccel);

	bool IsMoving(void)	{return (m_moveIndirect&0x3F) || (m_vel[0] != 0) || (m_vel[1] != 0) || (m_vel[2] != 0);};
	bool IsRotating(void){return (m_moveIndirect&0x3F00) || (m_velAngle[0] != 0) || (m_velAngle[1] != 0) || (m_velAngle[2] != 0);};

	void	SetCamRelative(bool p_bCamRelative);
	const int TILE_SIZE;
	bool		m_isShown;

	float	m_pos[3];
	float	m_angle[3];
	float	m_scale[3];

	float	m_posProjected[2];
	u32 	m_angleProjected;
	float 	m_scaleProjected;

	float	m_vel[3];
	float	m_accel[3];
	float 	m_velAngle[3];
	float	m_accelAngle[3];
	u32		m_radius;

	bool	m_isCollidable;

	THit	m_stHit;


	//MoveTo Stuff
	u32		m_moveIndirect;
	bool	m_bTurningKeepVel;
	bool	m_bTurningKeepVelAngle;
	float	m_posTo[3];
	float	m_angleTo[3];
	float	m_velMaxIndirect;
	float	m_accelIndirect;
	float	m_deccelIndirect;
	float	m_fSartToDeccel;
	u32		m_forceApplied;


	//Map Mode
	int		m_posMap[3];
	bool	m_bCamRelative;
};

#endif // _PHYSOBJECT_MNG_H_
