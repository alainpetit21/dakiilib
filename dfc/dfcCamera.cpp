#include "lib.h"
#include "libTypes.h"
#include "libMath.h"
#include "libMng3D.h"
#include "libMngGraphic.h"
#include "dfcCamera.h"
#include "dfcCamera.h"

CCamera2D* CCamera2D::m_pCurCamera= 0;
CCamera3D* CCamera3D::m_pCurCamera= 0;

u32
CCamera2D::AddChild(CObj* p_pObj, bool p_bTransformSetRelativity)
{
	if(p_pObj->IsKindOf(CLSID_CPhysObj))
		((CPhysObj*)p_pObj)->SetCamRelative(true);

	return CObj::AddChild(p_pObj, p_bTransformSetRelativity);
}

CObj*
CCamera2D::RemoveChild(CObj* p_pObj)
{
	if(p_pObj->IsKindOf(CLSID_CPhysObj))
		((CPhysObj*)p_pObj)->SetCamRelative(false);

	return CObj::RemoveChild(p_pObj);
}

void
CCamera2D::Manage(unsigned long p_nTimeTick)
{
	CPhysObj::Manage(p_nTimeTick);
}

void
CCamera2D::Update(void)
{
	CPhysObj::Update();
}

void
CCamera2D::FocusOn(CPhysObj *p_focused)
{
}

void
CCamera2D::SetCurrent(void)
{
	m_pCurCamera= this;
}

void
CCamera2D::Project(CPhysObj *p_Physobj)
{
CMngGraphic	*pRenderer	= GetMngGraphic();

	p_Physobj->m_posProjected[0]= (CCamera2D::GetCurrent()->GetPosX() + p_Physobj->GetPosX() + p_Physobj->GetParentPosX() + (long)(pRenderer->GetWidth(pRenderer->GetBB()) / 2));
	p_Physobj->m_posProjected[1]= (CCamera2D::GetCurrent()->GetPosY() + p_Physobj->GetPosY() + p_Physobj->GetParentPosY() + (long)(pRenderer->GetHeight(pRenderer->GetBB()) / 2));

	m_angleProjected= CCamera2D::GetCurrent()->GetAngleZ() + p_Physobj->GetParentAngleZ() + p_Physobj->GetAngleZ();
	m_scaleProjected= GetScaleZ();
}

void
CCamera3D::Manage(unsigned long p_nTimeTick)
{
	CPhysObj::Manage(p_nTimeTick);
}

void
CCamera3D::Update(void)
{
#ifdef _WIN32DX7
CMng3D *mng3D= (CMng3D *)GetMng3D();
float	mat[16], temp[16];

	if(mng3D->m_bIsInit){
		CMngMath::MatrixIdentity(mat);
		CMngMath::MatrixIdentity(temp);

		CMngMath::MatrixTranslate(temp, m_pos[0], m_pos[1], m_pos[2]);
		CMngMath::MatrixConcat(mat, temp);

		CMngMath::MatrixRotateX(temp, m_angle[0]);
		CMngMath::MatrixConcat(mat, temp);
		CMngMath::MatrixRotateY(temp, m_angle[1]);
		CMngMath::MatrixConcat(mat, temp);
		CMngMath::MatrixRotateZ(temp, m_angle[2]);
		CMngMath::MatrixConcat(mat, temp);

		D3DMATRIX matDX;
		memcpy(&matDX, mat, sizeof(float)*16);

		mng3D->m_pD3DDevice->SetTransform(D3DTRANSFORMSTATE_VIEW, &matDX);
	}
#endif

	CPhysObj::Update();
}

void
CCamera3D::FocusOn(CPhysObj *p_focused)
{
}

void
CCamera3D::SetCurrent(void)
{
	m_pCurCamera= this;
}
