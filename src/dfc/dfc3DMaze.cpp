#include "lib.h"
#include "libMacros.h"
#include "libMath.h"
#include "libMngGraphic.h"
#include "libMng3D.h"
#include "libMngResIO.h"
#include "dfc3DMaze.h"
#include "dfcCamera.h"
#include "dfcFont.h"


CMaze3D::CMaze3D():
CEntity3D(),
m_nWidth(0),
m_nHeight(0),
m_arMap(0)
#ifdef _DEBUG
, m_bShowDebug(true)
#endif
{
	memset(m_strName, 0, sizeof(char)*128);
//	memset(m_startingPos, 0, sizeof(float)*3);
}

CMaze3D::~CMaze3D()
{
	SAFE_DELETE_ARRAY(m_arMap);
}

void
CMaze3D::LoadInstance(const char* p_dataSource)
{
CMngImageIO	*mngImageIO	= GetMngImageIO();
int			nbBlocs;
int			i;

	CObj::LoadInstance(p_dataSource);

	m_pXml->FindElem("NAME");
	memcpy(m_strName, m_pXml->GetData().m_arBuffer, strlen(m_pXml->GetData().m_arBuffer));

	m_pXml->FindElem("TEXTURE");
	mngImageIO->Open(m_pXml->GetData().m_arBuffer, &m_texture.pData, &m_texture.nWidth, &m_texture.nHeight, true);

	m_pXml->FindElem("START_POS_X");
	m_startingPos[0]= float(atof(m_pXml->GetData().m_arBuffer)) + 1;

	m_pXml->FindElem("START_POS_Y");
	m_startingPos[1]= float(atof(m_pXml->GetData().m_arBuffer)) + 1;

	m_pXml->FindElem("START_POS_Z");
	m_startingPos[2]= (m_nCurZ= int(atof(m_pXml->GetData().m_arBuffer))) - 0.4f;

	m_pXml->FindElem("START_ANGLE");
	m_startingAngleY= u32(atof(m_pXml->GetData().m_arBuffer)*256/6.2832);

	m_pXml->FindElem("SIZE_X");
	m_nWidth= (atoi(m_pXml->GetData().m_arBuffer)+2)|1;
	m_pXml->FindElem("SIZE_Y");
	m_nHeight= (atoi(m_pXml->GetData().m_arBuffer)+2)|1;
	m_pXml->FindElem("SIZE_Z");
	m_nDepth= atoi(m_pXml->GetData().m_arBuffer);

	m_pXml->FindElem("NB_BLOCS");
	nbBlocs= atoi(m_pXml->GetData().m_arBuffer);

	m_startingPos[0]-= int(m_nWidth / 2.0f);
	m_startingPos[1]-= int(m_nHeight / 2.0f);
	m_startingPos[1]= -m_startingPos[1];
	m_startingPos[2]= (-m_nCurZ)-0.4f;

	//Convet to a arMap2D
	m_arMap= new u8[m_nWidth*m_nHeight*m_nDepth];
	memset(m_arMap, 0x1, m_nWidth*m_nHeight*m_nDepth);

	//Build the maze
	for(i= 0; i < nbBlocs; ++i){
		m_pXml->FindElem("BLOC");
		m_pXml->IntoElem();{

			m_pXml->FindElem("POS_X");
			int posX= atoi(m_pXml->GetData().m_arBuffer)+1;

			m_pXml->FindElem("POS_Y");
			int posY= atoi(m_pXml->GetData().m_arBuffer)+1;

			m_pXml->FindElem("POS_Z");
			int posZ= atoi(m_pXml->GetData().m_arBuffer);

			m_pXml->FindElem("TYPE");
			u8 type	= atoi(m_pXml->GetData().m_arBuffer);

			m_arMap[(posZ*(m_nWidth*m_nHeight))+(posY*m_nWidth)+posX]= type;

		}m_pXml->OutOfElem();
	}

	SAFE_DELETE(m_pXml);

	Build3D();
}

void
CMaze3D::Build3D(void)
{
//Open the .3dMaze
CMng3D	*pMng3D	= (CMng3D*)GetMng3D();
int i, j, z;
int cptWall= 0;
int cptFloor= 0;


	//Analyse nbWallRequired
	for(z= 0; z < m_nDepth; ++z){
		for(j= 0; j < m_nHeight; ++j){
			for(i= 0; i < m_nWidth-1; ++i){
				if(!TileIsFree(i, j, z) && TileIsFree(i+1, j, z)){
					++cptWall;
				}
			}
		}

		for(j= 0; j < m_nHeight; ++j){
			for(i= m_nWidth-1; i > 0; --i){
				if(!TileIsFree(i, j, z) && TileIsFree(i-1, j, z)){
					++cptWall;
				}
			}
		}


		for(i= 0; i < m_nWidth; ++i){
			for(j= 0; j < m_nHeight-1; ++j){
				if(!TileIsFree(i, j, z) && TileIsFree(i, j+1, z)){
					++cptWall;
				}
			}
		}

		for(i= 0; i < m_nWidth; ++i){
			for(j= m_nHeight-1; j > 0; --j){
				if(!TileIsFree(i, j, z) && TileIsFree(i, j-1, z)){
					++cptWall;
				}
			}
		}

		//Analyse nbFloorRequired
		for(j= 0; j < m_nHeight; ++j)
			for(i= 0; i < m_nWidth; ++i)
				if(TileIsFree(i, j, z))
					++cptFloor;
	}


	m_pVb		= pMng3D->CreateVB((cptWall*4) + (cptFloor*4) + (cptFloor*4));
	m_pIb		= pMng3D->CreateIB((cptWall*6) + (cptFloor*6) + (cptFloor*6));

	TVertex *retvb= m_pVb->Lock();
	u16		*retib= m_pIb->Lock();
	memset(retvb, 0, sizeof(TVertex)*((cptWall*4) + (cptFloor*4) + (cptFloor*4)));
	memset(retib, 0, sizeof(u16)*((cptWall*6) + (cptFloor*6) + (cptFloor*6)));

	//Create West Wal
	int cptV= 0;
	int cptI= 0;

	for(z= 0; z < m_nDepth; ++z){
		for(j= 0; j < m_nHeight; ++j){
			for(i= 0; i < m_nWidth-1; ++i){
				if(!TileIsFree(i, j, z) && TileIsFree(i+1, j, z)){
					retvb[cptV].x	= i+1-(float(m_nWidth)/2.0f);
					retvb[cptV].y	= j+1-(float(m_nHeight)/2.0f);
					retvb[cptV].z	= 0.0f+z;
					retvb[cptV].tu	= 0;
					retvb[cptV++].tv= 0;
					retvb[cptV].x	= i+1-(float(m_nWidth)/2.0f);
					retvb[cptV].y	= j-(float(m_nHeight)/2.0f);
					retvb[cptV].z	= 0.0f+z;
					retvb[cptV].tu	= 0.5f;
					retvb[cptV++].tv= 0;
					retvb[cptV].x	= i+1-(float(m_nWidth)/2.0f);
					retvb[cptV].y	= j+1-(float(m_nHeight)/2.0f);
					retvb[cptV].z	= 1.0f+z;
					retvb[cptV].tu	= 0;
					retvb[cptV++].tv= 0.5f;
					retvb[cptV].x	= i+1-(float(m_nWidth)/2.0f);
					retvb[cptV].y	= j-(float(m_nHeight)/2.0f);
					retvb[cptV].z	= 1.0f+z;
					retvb[cptV].tu	= 0.5f;
					retvb[cptV++].tv= 0.5f;

					retib[cptI++]= cptV - 1;
					retib[cptI++]= cptV - 3;
					retib[cptI++]= cptV - 2;
					retib[cptI++]= cptV - 2;
					retib[cptI++]= cptV - 3;
					retib[cptI++]= cptV - 4;
				}
				//Change TU & TV for a door
				if((m_arMap[(z*(m_nWidth*m_nHeight))+(j*m_nWidth)+i] == 2) && TileIsFree(i+1, j, z)){
					retvb[cptV-4].tu= 0;
					retvb[cptV-4].tv= 1;
					retvb[cptV-3].tu= 0.5f;
					retvb[cptV-3].tv= 1;
					retvb[cptV-2].tu= 0;
					retvb[cptV-2].tv= 0.5f;
					retvb[cptV-1].tu= 0.5f;
					retvb[cptV-1].tv= 0.5f;
				}
			}
		}

		//Create East Wall
		for(j= 0; j < m_nHeight; ++j){
			for(i= m_nWidth-1; i > 0; --i){
				if(!TileIsFree(i, j, z) && TileIsFree(i-1, j, z)){
					retvb[cptV].x	= i-(float(m_nWidth)/2.0f);
					retvb[cptV].y	= j-(float(m_nHeight)/2.0f);
					retvb[cptV].z	= 0.0f+z;
					retvb[cptV].tu	= 0;
					retvb[cptV++].tv= 0;
					retvb[cptV].x	= i-(float(m_nWidth)/2.0f);
					retvb[cptV].y	= j+1-(float(m_nHeight)/2.0f);
					retvb[cptV].z	= 0.0f+z;
					retvb[cptV].tu	= 0.5f;
					retvb[cptV++].tv= 0;
					retvb[cptV].x	= i-(float(m_nWidth)/2.0f);
					retvb[cptV].y	= j-(float(m_nHeight)/2.0f);
					retvb[cptV].z	= 1.0f+z;
					retvb[cptV].tu	= 0;
					retvb[cptV++].tv= 0.5f;
					retvb[cptV].x	= i-(float(m_nWidth)/2.0f);
					retvb[cptV].y	= j+1-(float(m_nHeight)/2.0f);
					retvb[cptV].z	= 1.0f+z;
					retvb[cptV].tu	= 0.5f;
					retvb[cptV++].tv= 0.5f;

					retib[cptI++]= cptV - 1;
					retib[cptI++]= cptV - 3;
					retib[cptI++]= cptV - 2;
					retib[cptI++]= cptV - 2;
					retib[cptI++]= cptV - 3;
					retib[cptI++]= cptV - 4;
				}
				//Change TU & TV for a door
				if((m_arMap[(z*(m_nWidth*m_nHeight))+(j*m_nWidth)+i]==2) && TileIsFree(i-1, j, z)){
					retvb[cptV-4].tu= 0;
					retvb[cptV-4].tv= 1;
					retvb[cptV-3].tu= 0.5f;
					retvb[cptV-3].tv= 1;
					retvb[cptV-2].tu= 0;
					retvb[cptV-2].tv= 0.5f;
					retvb[cptV-1].tu= 0.5f;
					retvb[cptV-1].tv= 0.5f;
				}
			}
		}


		//Create North Wall
		for(i= 0; i < m_nWidth; ++i){
			for(j= 0; j < m_nHeight-1; ++j){
				if(!TileIsFree(i, j, z) && TileIsFree(i, j+1, z)){
					retvb[cptV].x	= i-(float(m_nWidth)/2.0f);
					retvb[cptV].y	= j+1-(float(m_nHeight)/2.0f);
					retvb[cptV].z	= 0.0f+z;
					retvb[cptV].tu	= 0;
					retvb[cptV++].tv= 0;
					retvb[cptV].x	= i+1-(float(m_nWidth)/2.0f);
					retvb[cptV].y	= j+1-(float(m_nHeight)/2.0f);
					retvb[cptV].z	= 0.0f+z;
					retvb[cptV].tu	= 0.5f;
					retvb[cptV++].tv= 0;
					retvb[cptV].x	= i-(float(m_nWidth)/2.0f);
					retvb[cptV].y	= j+1-(float(m_nHeight)/2.0f);
					retvb[cptV].z	= 1.0f+z;
					retvb[cptV].tu	= 0;
					retvb[cptV++].tv= 0.5f;
					retvb[cptV].x	= i+1-(float(m_nWidth)/2.0f);
					retvb[cptV].y	= j+1-(float(m_nHeight)/2.0f);
					retvb[cptV].z	= 1.0f+z;
					retvb[cptV].tu	= 0.5f;
					retvb[cptV++].tv= 0.5f;

					retib[cptI++]= cptV - 1;
					retib[cptI++]= cptV - 3;
					retib[cptI++]= cptV - 2;
					retib[cptI++]= cptV - 2;
					retib[cptI++]= cptV - 3;
					retib[cptI++]= cptV - 4;
				}
				if((m_arMap[(z*(m_nWidth*m_nHeight))+(j*m_nWidth)+i]==2) && TileIsFree(i, j+1, z)){
					retvb[cptV-4].tu= 0;
					retvb[cptV-4].tv= 1;
					retvb[cptV-3].tu= 0.5f;
					retvb[cptV-3].tv= 1;
					retvb[cptV-2].tu= 0;
					retvb[cptV-2].tv= 0.5f;
					retvb[cptV-1].tu= 0.5f;
					retvb[cptV-1].tv= 0.5f;
				}
			}
		}

		//Create South Wall
		for(i= 0; i < m_nWidth; ++i){
			for(j= m_nHeight-1; j > 0; --j){
				if(!TileIsFree(i, j, z) && TileIsFree(i, j-1, z)){
					retvb[cptV].x	= i+1-(float(m_nWidth)/2.0f);
					retvb[cptV].y	= j-(float(m_nHeight)/2.0f);
					retvb[cptV].z	= 0.0f+z;
					retvb[cptV].tu	= 0;
					retvb[cptV++].tv= 0;
					retvb[cptV].x	= i-(float(m_nWidth)/2.0f);
					retvb[cptV].y	= j-(float(m_nHeight)/2.0f);
					retvb[cptV].z	= 0.0f+z;
					retvb[cptV].tu	= 0.5f;
					retvb[cptV++].tv= 0;
					retvb[cptV].x	= i+1-(float(m_nWidth)/2.0f);
					retvb[cptV].y	= j-(float(m_nHeight)/2.0f);
					retvb[cptV].z	= 1.0f+z;
					retvb[cptV].tu	= 0;
					retvb[cptV++].tv= 0.5f;
					retvb[cptV].x	= i-(float(m_nWidth)/2.0f);
					retvb[cptV].y	= j-(float(m_nHeight)/2.0f);
					retvb[cptV].z	= 1.0f+z;
					retvb[cptV].tu	= 0.5f;
					retvb[cptV++].tv= 0.5f;

					retib[cptI++]= cptV - 1;
					retib[cptI++]= cptV - 3;
					retib[cptI++]= cptV - 2;
					retib[cptI++]= cptV - 2;
					retib[cptI++]= cptV - 3;
					retib[cptI++]= cptV - 4;
				}
				if((m_arMap[(z*(m_nWidth*m_nHeight))+(j*m_nWidth)+i]==2) && TileIsFree(i, j-1, z)){
					retvb[cptV-4].tu= 0;
					retvb[cptV-4].tv= 1;
					retvb[cptV-3].tu= 0.5f;
					retvb[cptV-3].tv= 1;
					retvb[cptV-2].tu= 0;
					retvb[cptV-2].tv= 0.5f;
					retvb[cptV-1].tu= 0.5f;
					retvb[cptV-1].tv= 0.5f;
				}
			}
		}

		//Create Ceiling
		for(j= 0; j < m_nHeight; ++j){
			for(i= 0; i < m_nWidth; ++i){
				if((m_arMap[(z*(m_nWidth*m_nHeight))+(j*m_nWidth)+i] == 0) || (m_arMap[(z*(m_nWidth*m_nHeight))+(j*m_nWidth)+i] == 4)){
					retvb[cptV].x	= i-(float(m_nWidth)/2.0f);
					retvb[cptV].y	= j-(float(m_nHeight)/2.0f);
					retvb[cptV].z	= 1.0f+z;
					retvb[cptV].tu	= 0.5f;
					retvb[cptV++].tv= 0;
					retvb[cptV].x	= i+1-(float(m_nWidth)/2.0f);
					retvb[cptV].y	= j-(float(m_nHeight)/2.0f);
					retvb[cptV].z	= 1.0f+z;
					retvb[cptV].tu	= 1;
					retvb[cptV++].tv= 0;
					retvb[cptV].x	= i-(float(m_nWidth)/2.0f);
					retvb[cptV].y	= j+1-(float(m_nHeight)/2.0f);
					retvb[cptV].z	= 1.0f+z;
					retvb[cptV].tu	= 0.5f;
					retvb[cptV++].tv= 0.5f;
					retvb[cptV].x	= i+1-(float(m_nWidth)/2.0f);
					retvb[cptV].y	= j+1-(float(m_nHeight)/2.0f);
					retvb[cptV].z	= 1.0f+z;
					retvb[cptV].tu	= 1;
					retvb[cptV++].tv= 0.5f;

					retib[cptI++]= cptV - 1;
					retib[cptI++]= cptV - 3;
					retib[cptI++]= cptV - 2;
					retib[cptI++]= cptV - 2;
					retib[cptI++]= cptV - 3;
					retib[cptI++]= cptV - 4;
				}
			}
		}

		//Create Floor
		for(j= 0; j < m_nHeight; ++j){
			for(i= 0; i < m_nWidth; ++i){
				if((m_arMap[(z*(m_nWidth*m_nHeight))+(j*m_nWidth)+i] == 0) || (m_arMap[(z*(m_nWidth*m_nHeight))+(j*m_nWidth)+i] == 3)){
					retvb[cptV].x	= i+1-(float(m_nWidth)/2.0f);
					retvb[cptV].y	= j-(float(m_nHeight)/2.0f);
					retvb[cptV].z	= 0.0f+z;
					retvb[cptV].tu	= 0.5f;
					retvb[cptV++].tv= 0;
					retvb[cptV].x	= i-(float(m_nWidth)/2.0f);
					retvb[cptV].y	= j-(float(m_nHeight)/2.0f);
					retvb[cptV].z	= 0.0f+z;
					retvb[cptV].tu	= 1;
					retvb[cptV++].tv= 0;
					retvb[cptV].x	= i+1-(float(m_nWidth)/2.0f);
					retvb[cptV].y	= j+1-(float(m_nHeight)/2.0f);
					retvb[cptV].z	= 0.0f+z;
					retvb[cptV].tu	= 0.5f;
					retvb[cptV++].tv= 0.5f;
					retvb[cptV].x	= i-(float(m_nWidth)/2.0f);
					retvb[cptV].y	= j+1-(float(m_nHeight)/2.0f);
					retvb[cptV].z	= 0.0f+z;
					retvb[cptV].tu	= 1;
					retvb[cptV++].tv= 0.5f;

					retib[cptI++]= cptV - 1;
					retib[cptI++]= cptV - 3;
					retib[cptI++]= cptV - 2;
					retib[cptI++]= cptV - 2;
					retib[cptI++]= cptV - 3;
					retib[cptI++]= cptV - 4;
				}
			}
		}
	}

	m_pVb->Unlock();
	m_pIb->Unlock();
}

void
CMaze3D::Manage(u32 p_nTimeTick)
{
CCamera3D	*pCam= CCamera3D::m_pCurCamera;

	m_nCurZ= -(pCam->GetIntPosZ());
	CEntity3D::Manage(p_nTimeTick);
}

void
CMaze3D::Update(void)
{
	CEntity3D::Update();

#ifdef _DEBUG
	if(m_bShowDebug){
		CCamera3D	*pCam= CCamera3D::m_pCurCamera;
		CFont		*pFont= CFont::m_pAllFontInstance[0];
		char		temp[4];

		for(int i= 0; i < m_nWidth; ++i){
			for(int j= 0; j < m_nHeight; ++j){
				if(m_arMap[int((m_nCurZ*(m_nWidth*m_nHeight))+(j*m_nWidth)+i)] == 1){
					sprintf(temp, ".");
				}else if(m_arMap[int((m_nCurZ*(m_nWidth*m_nHeight))+(j*m_nWidth)+i)] == 2){
					sprintf(temp, "O");
				}else{
					sprintf(temp, " ");
				}
				if(((-(pCam->GetIntPosX())+(m_nWidth/2)) == i) && ((-(pCam->GetIntPosY())+(m_nHeight/2) == j)))
					sprintf(temp, ".");

				pFont->PutS((350+(i*5)), (40+(j*5)), temp);
			}
		}
	}
#endif
}

u8
CMaze3D::GetCollision(int p_nX, int p_nY)
{
	p_nX= int(-p_nX+float(m_nWidth)/2.0);
	p_nY= int(-p_nY+float(m_nHeight)/2.0);
	return m_arMap[int((m_nCurZ*(m_nWidth*m_nHeight))+(p_nY*m_nWidth)+p_nX)];
}
