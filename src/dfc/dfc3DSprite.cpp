#include "lib.h"
#include "libMacros.h"
#include "libMath.h"
#include "libMngGraphic.h"
#include "libMng3D.h"
#include "libMngResIO.h"
#include "dfc3DSprite.h"
#include "dfcCamera.h"
#include "dfcFont.h"


CSprite3D::CSprite3D():
CEntity3D()
{
}

CSprite3D::~CSprite3D()
{
}

void
CSprite3D::LoadInstance(const char* p_dataSource)
{
	CEntity::LoadInstance(p_dataSource);
	Build3D();
}

void
CSprite3D::Build3D(void)
{
CMng3D	*pMng3D	= (CMng3D*)GetMng3D();

	m_pVb		= pMng3D->CreateVB(4);
	m_pIb		= pMng3D->CreateIB(6);

	TVertex *retvb= m_pVb->Lock();
	u16		*retib= m_pIb->Lock();
	memset(retvb, 0, sizeof(TVertex)*4);
	memset(retib, 0, sizeof(u16)*6);

	//Create West Wal
	retvb[0].x= retvb[0].tu= 0;
	retvb[0].y= retvb[0].tv= 0;
	retvb[0].z= 0;
	retvb[1].x= retvb[1].tu= 1;
	retvb[1].y= retvb[1].tv= 0;
	retvb[1].z= 0;
	retvb[2].x= retvb[2].tu= 0;
	retvb[2].y= retvb[2].tv= 1;
	retvb[2].z= 0;
	retvb[3].x= retvb[3].tu= 1;
	retvb[3].y= retvb[3].tv= 1;
	retvb[3].z= 0;

	retib[0]= 0;
	retib[1]= 1;
	retib[2]= 2;
	retib[3]= 1;
	retib[4]= 2;
	retib[5]= 3;

	m_pVb->Unlock();
	m_pIb->Unlock();
}

void
CSprite3D::Manage(u32 p_nTimeTick)
{
//CCamera3D	*pCam= CCamera3D::m_pCurCamera;

	CEntity::Manage(p_nTimeTick);
	CEntity3D::Manage(p_nTimeTick);

	m_texture.nHeight	= ((CEntity*)this)->m_pCurAnim->nHeight;
	m_texture.nHotSpotX	= ((CEntity*)this)->m_pCurAnim->nHotSpotX;
	m_texture.nHotSpotY	= ((CEntity*)this)->m_pCurAnim->nHotSpotY;
	m_texture.nWidth	= ((CEntity*)this)->m_pCurAnim->nWidth;
	m_texture.pData		= ((CEntity*)this)->m_pCurAnim->pData;
}

void
CSprite3D::Update(void)
{
	CEntity3D::Update();
}
