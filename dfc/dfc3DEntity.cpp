#include "lib.h"
#include "libMath.h"
#include "libMacros.h"
#include "libMngGraphic.h"
#include "libMng3D.h"
#include "dfc3DEntity.h"
#include "dfcCamera.h"

CEntity3D::CEntity3D():
m_arFrames(0),
m_arState(0)
{
	memset(&m_texture, 0, sizeof(TRuntimeFrame));
}

CEntity3D::~CEntity3D()
{
	SAFE_DELETE_ARRAY(m_arFrames);
	SAFE_DELETE_ARRAY(m_arState);
	delete m_pVb;
	delete m_pIb;
}

void
CEntity3D::LoadInstance(const char* p_dataSource)
{
//CMng3D_win32dx7	*p3DRenderer= (CMng3D_win32dx7*)GetMng3D();
//T_3DEntity		*pEntity= (T_3DEntity*)p_dataSource;
//u32				i;
//
//	m_nbFrame= pEntity->nbFrame;
//	m_arFrames= new T_Frame3D[m_nbFrame];
//
//	for(i= 0; i < m_nbFrame; ++i)
//		m_arFrames[i].pResVertex= pEntity->arFrames[i];
//
//	m_nbState= pEntity->nbState;
//	m_arState= new TFrame3D*[m_nbState+1];
//
//	for(i= 0; i < m_nbState; ++i)
//		m_arState[i]= &m_arFrames[(pEntity->arState[i] - pEntity->arFrames[0]) / pEntity->nbVertex];
//
//	m_arState[m_nbState]= &m_arFrames[m_nbFrame];
//
//	m_pCurAnim		= m_arState[m_curState= 0];
//	m_indexFrame	= 0.0f;
//
//	m_pVb= p3DRenderer->CreateVB(pEntity->nbVertex);
//	m_pIb= p3DRenderer->CreateIB(pEntity->nbIndex*3);
//	m_pTb= p3DRenderer->CreateTB(pEntity->nbIndex);
//
////	TVertex *pVert= m_pVb->Lock();
////	memcpy(pVert, pEntity->arFrames[0], sizeof(TVertex) * pEntity->nbVertex);
////	m_pVb->Unlock();
////
////	unsigned short *pIndex= m_pIb->Lock();
////	memcpy(pIndex, pEntity->arIndexes, sizeof(unsigned short) * pEntity->nbIndex*3);
////	m_pIb->Unlock();
////	pIndex= m_pIb->Lock();
////	m_pIb->Unlock();
////
////	TTexcoord *pTexcoord= m_pTb->Lock();
////	memcpy(pTexcoord, pEntity->arTexcoord, sizeof(TTexcoord) * pEntity->nbIndex);
////	m_pTb->Unlock();
}


void
CEntity3D::Manage(u32 p_nTimeTick)
{
//	m_indexFrame+= m_speed;
//	if(&m_pCurAnim[(int)m_indexFrame] >= m_arState[m_curState+1])
//		m_indexFrame= 0;

	CPhysObj::Manage(p_nTimeTick);
}

void
CEntity3D::Update(void)
{
CMng3D	*p3DRenderer= (CMng3D*)GetMng3D();

#ifdef _WIN32DX7
CMng3D *mng3D= (CMng3D *)GetMng3D();
float	mat[16], temp[16];

	CMngMath::MatrixIdentity(mat);
	CMngMath::MatrixIdentity(temp);

	CMngMath::MatrixRotateX(temp, m_angle[0]);
	CMngMath::MatrixConcat(mat, temp);
	CMngMath::MatrixRotateY(temp, m_angle[1]);
	CMngMath::MatrixConcat(mat, temp);
	CMngMath::MatrixRotateZ(temp, m_angle[2]);
	CMngMath::MatrixConcat(mat, temp);

	CMngMath::MatrixTranslate(temp, m_pos[0], m_pos[1], m_pos[2]);
	CMngMath::MatrixConcat(mat, temp);

	D3DMATRIX matDX;
	memcpy(&matDX, mat, sizeof(float)*16);
	mng3D->m_pD3DDevice->SetTransform(D3DTRANSFORMSTATE_WORLD, &matDX);
#endif

//TODO	p3DRenderer->DrawPrimitiveIB(m_pIb, m_pVb, m_texture.pData);
	CPhysObj::Update();
}

u32
CEntity3D::GetWidth(void)
{
	return 0;
}

u32
CEntity3D::GetHeight(void)
{
	return 0;
}

void
CEntity3D::SetFrame(u32 p_nFrame)
{
//	m_indexFrame= (float)p_nFrame;
//	if(&m_pCurAnim[(int)m_indexFrame] >= m_arState[m_curState+1])
//		m_indexFrame= 0;
}
