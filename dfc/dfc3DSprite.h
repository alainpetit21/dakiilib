#ifndef _3D_SPRITE_H_
#define _3D_SPRITE_H_

#include "dfc3DEntity.h"

class CSprite3D : public CEntity3D
{
public :
	CSprite3D();
	~CSprite3D();
	IMPLEMENT_RTTI(Sprite3D, Entity3D);

	virtual void LoadInstance(const char* p_dataSource);
	virtual void Manage(u32 p_timeTick);
	virtual void Update(void);
	virtual void SetFrame(u32 p_frame){};
	virtual void SetState(u32 p_state){};

	void Build3D();


};

#endif // _3D_SPRITE_H_
