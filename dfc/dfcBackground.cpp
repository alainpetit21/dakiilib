#include "lib.h"
#include "libMacros.h"
#include "dfcBackground.h"
#include "dfcCamera.h"
#include "dfcApp.h"
#include "libMngResIO.h"

CBackgroundLayerTiled::CBackgroundLayerTiled():
m_pMap(0)
{
}

CBackgroundLayerTiled::~CBackgroundLayerTiled()
{
	SAFE_DELETE_ARRAY(m_pMap);
}

void
CBackgroundLayerTiled::LoadMap(const char *p_filename)
{
CMngIO		*mngIO		= GetMngIO();
io 			fichier;

	helper_OpenRes(fichier, mngIO, p_filename);


	m_pMap= new u16[(m_sizeTiled[0]/g_TheApp->m_nPhysicSizeBlocMap)*(m_sizeTiled[1]/g_TheApp->m_nPhysicSizeBlocMap)];
	mngIO->Read(fichier, m_pMap, sizeof(u16)*(m_sizeTiled[0]/g_TheApp->m_nPhysicSizeBlocMap)*(m_sizeTiled[1]/g_TheApp->m_nPhysicSizeBlocMap));
}

void
CBackgroundLayerTiled::LoadInstance(CXml* p_pXml)
{
CMngImageIO	*mngImageIO	= GetMngImageIO();

	p_pXml->FindElem("NB_TILES");
	m_nbTiles= atoi((p_pXml->GetData()).m_arBuffer);

	p_pXml->FindElem("SIZE_X");
	m_sizeTiled[0]= atoi((p_pXml->GetData()).m_arBuffer);

	p_pXml->FindElem("SIZE_Y");
	m_sizeTiled[1]= atoi((p_pXml->GetData()).m_arBuffer);

	p_pXml->FindElem("TILES_FILE");
	mngImageIO->Open((p_pXml->GetData()).m_arBuffer, &m_bitmap, &m_size[0], &m_size[1], 0);

	p_pXml->FindElem("MAP_FILE");
	LoadMap((p_pXml->GetData()).m_arBuffer);

	p_pXml->FindElem("ON");
	m_isShown= (bool(atoi((p_pXml->GetData()).m_arBuffer)));

	if(p_pXml->FindElem("POS_X"))
		SetPosX(atoi((p_pXml->GetData()).m_arBuffer));
	else
		SetPosX(m_sizeTiled[0]>>1);

	if(p_pXml->FindElem("POS_Y"))
		SetPosY(atoi((p_pXml->GetData()).m_arBuffer));
	else
		SetPosY(m_sizeTiled[1]>>1);

	p_pXml->FindElem("POS_Z");
	SetPosZ(atoi((p_pXml->GetData()).m_arBuffer));

	if(p_pXml->FindElem("VEL_X"))
		SetVelX(float(atof((p_pXml->GetData()).m_arBuffer)));

	if(p_pXml->FindElem("VEL_Y"))
		SetVelY(float(atof((p_pXml->GetData()).m_arBuffer)));

	if(p_pXml->FindElem("VEL_Z"))
		SetVelZ(float(atof((p_pXml->GetData()).m_arBuffer)));
}


void
CBackgroundLayerTiled::Update(void)
{
CMngGraphic	*pRenderer			= GetMngGraphic();
void		*pDestBitmap		= pRenderer->GetBB();
u16			*pMap				= m_pMap;
int			nSizeBloc			= g_TheApp->m_nPhysicSizeBlocMap;
int			nBitSizeBloc		= helper_FindHigher16(nSizeBloc);
int			sizeInTileX			= m_sizeTiled[0]/nSizeBloc;
int			sizeInTileY			= m_sizeTiled[1]/nSizeBloc;
int			widthTilePow2		= helper_PadPower2Up(m_size[0]/nSizeBloc);
int			maskWidthTilePow2	= widthTilePow2 - 1;
int			nWidthDst			= pRenderer->GetWidth(pDestBitmap);
int			nHeightDst			= pRenderer->GetHeight(pDestBitmap);
int			nPosDstStartX		= s32(-GetIntPosX()) + (pRenderer->GetWidth(pDestBitmap)/2);
int			nPosDstStartY		= s32(-GetIntPosY()) + (pRenderer->GetHeight(pDestBitmap)/2);
int			i, j;

	CPhysObj::Update();

	if(!m_isShown)
		return;

	int nPosDstY= nPosDstStartY;
	for(j= 0; j < sizeInTileY; ++j, nPosDstY+= nSizeBloc){
		int nPosDstX= nPosDstStartX;

		if((nPosDstY < 0) || (nPosDstY >= nHeightDst)){
			pMap+= sizeInTileX;
			continue;
		}

		for(i= 0; i < sizeInTileX; ++i, nPosDstX+= nSizeBloc){
			int nTileOffset= *pMap++;
			int nTileOffsetX= nTileOffset & maskWidthTilePow2;
			int nTileOffsetY= nTileOffset / widthTilePow2;

			if((nPosDstX < 0) || (nPosDstX >= nWidthDst))
				continue;

			pRenderer->DrawPartialBitmapFast(pDestBitmap, nPosDstX, nPosDstY, nSizeBloc, nSizeBloc,
											 nTileOffsetX<<nBitSizeBloc, nTileOffsetY<<nBitSizeBloc, nSizeBloc, nSizeBloc, 255, m_bitmap);
		}
	}
}


void
CBackgroundLayerLooping::Update(void)
{/*
CMngGraphic	*pRenderer	= GetMngGraphic();

	CPhysObj::Update();

	if(!m_isShown)
		return;

	pRenderer->DrawBitmapLooping(0, 0, 0, s32(pRenderer->GetWidth(pRenderer->GetBB())), s32(pRenderer->GetHeight(pRenderer->GetBB())),
								 s32(-(CCamera2D::GetCurrent()->GetPosX().m_nDec + GetPosX().m_nDec)), s32(-(CCamera2D::GetCurrent()->GetPosY().m_nDec + GetPosY().m_nDec)), 0, m_bitmap);
*/}

CBackgroundLayer::~CBackgroundLayer()
{
CMngImageIO	*mngImageIO	= GetMngImageIO();

	mngImageIO->Close(&m_bitmap);
}

void
CBackgroundLayer::Update(void)
{
//printf("CBackgroundLayer::Update->\n");
CMngGraphic	*pRenderer	= GetMngGraphic();
CCamera2D	*pCurCam	= CCamera2D::GetCurrent();

	CPhysObj::Update();

	if(!m_isShown)
		return;

	if(!m_bCamRelative)
		pRenderer->DrawBitmap(0,
			s32(-(pCurCam->GetIntPosX() + GetIntPosX()) + (pRenderer->GetWidth(pRenderer->GetBB()) / 2)),
			s32(-(pCurCam->GetIntPosY() + GetIntPosY()) + (pRenderer->GetHeight(pRenderer->GetBB()) / 2)),
			s32(m_size[0]), s32(m_size[1]), 0, 1.0f, 0, m_bitmap);
	else
		pRenderer->DrawBitmap(0,
			s32(-GetIntPosX() + (pRenderer->GetWidth(pRenderer->GetBB()) / 2)),
			s32(-GetIntPosY() + (pRenderer->GetHeight(pRenderer->GetBB()) / 2)),
			s32(m_size[0]), s32(m_size[1]), 0, 1.0f, 0, m_bitmap);

//	printf("<-CBackgroundLayer::Update\n");
}

void
CBackgroundLayer::LoadInstance(const char* p_strFilename)
{
CMngImageIO	*mngImageIO	= GetMngImageIO();

	mngImageIO->Open(p_strFilename, &m_bitmap, &m_size[0], &m_size[1], false);
}

void
CBackgroundLayer::LoadInstance(CXml* p_pXml)
{
CMngImageIO	*mngImageIO	= GetMngImageIO();

	p_pXml->FindElem("FILENAME");
	mngImageIO->Open((p_pXml->GetData()).m_arBuffer, &m_bitmap, &m_size[0], &m_size[1], false);

	p_pXml->FindElem("ON");
	m_isShown= (bool(atoi((p_pXml->GetData()).m_arBuffer)));

	if(p_pXml->FindElem("POS_X"))
		SetPosX(atoi((p_pXml->GetData()).m_arBuffer));
	else
		SetPosX(m_size[0]>>1);

	if(p_pXml->FindElem("POS_Y"))
		SetPosY(atoi((p_pXml->GetData()).m_arBuffer));
	else
		SetPosY(m_size[1]>>1);

	p_pXml->FindElem("POS_Z");
	SetPosZ(atoi((p_pXml->GetData()).m_arBuffer));

	if(p_pXml->FindElem("VEL_X"))
		SetVelX(float(atof((p_pXml->GetData()).m_arBuffer)));

	if(p_pXml->FindElem("VEL_Y"))
		SetVelY(float(atof((p_pXml->GetData()).m_arBuffer)));

	if(p_pXml->FindElem("VEL_Z"))
		SetVelZ(float(atof((p_pXml->GetData()).m_arBuffer)));
}

CBackground::~CBackground()
{
	delete []m_pLayers;
}

void
CBackground::LoadCollision(char* p_dataSource)
{
CMngIO	*mngIO	= GetMngIO();
io 		fichier;
u32		size;
char 	*chaine;

	helper_OpenScript(fichier, mngIO, p_dataSource);

	size	= mngIO->GetSize(fichier);
	chaine	= new char[size];

	mngIO->Read(fichier, chaine, size);

	/* Parse the xml*/
	CXml xml(chaine);

	xml.FindElem("COLLISION");
	xml.IntoElem();{


		xml.FindElem("NB_LINES");
		int nbLines= atoi((xml.GetData()).m_arBuffer);

		g_TheApp->GetCurContext()->m_cptLines	= 0;
		g_TheApp->GetCurContext()->m_maxLines	= nbLines;
		g_TheApp->GetCurContext()->m_arLines	= new TLine[g_TheApp->GetCurContext()->m_maxLines];


		for(int i= 0; i < nbLines; ++i){
			char chaineTemp[32];

			sprintf(chaineTemp, "LINE");
			xml.FindElem(chaineTemp);
			xml.IntoElem();{

				xml.FindElem("START_X");
				int sX= atoi((xml.GetData()).m_arBuffer);

				xml.FindElem("START_Y");
				int sY= atoi((xml.GetData()).m_arBuffer);

				xml.FindElem("END_X");
				int eX= atoi((xml.GetData()).m_arBuffer);

				xml.FindElem("END_Y");
				int eY= atoi((xml.GetData()).m_arBuffer);

				xml.FindElem("GROUP");
				u32 group= (u32)atoi((xml.GetData()).m_arBuffer);

				g_TheApp->GetCurContext()->AddLine(sX, sY, eX, eY, group);

			}xml.OutOfElem();
		}
	}xml.OutOfElem();

	mngIO->Close(fichier);
	delete []chaine;
}

void
CBackground::LoadInstance(const char* p_dataSource)
{
	CObj::LoadInstance(p_dataSource);

	m_pXml->FindElem("NB_LAYERS");
	m_nbLayer= atoi((m_pXml->GetData()).m_arBuffer);
	m_pLayers	= new CBackgroundLayer*[m_nbLayer];

	for(u32 i= 0; i < m_nbLayer; ++i){
		int nType= -1;
		char chaineTemp[32];

		sprintf(chaineTemp, "LAYER");
		m_pXml->FindElem(chaineTemp);
		m_pXml->IntoElem();{

			CDString chaineTemp;
			if(m_pXml->FindElem("TYPE"))
				nType= helper_StringToObjectType(m_pXml->GetData().m_arBuffer);

			switch(nType){
			case CLSID_CBackgroundLayerLooping:	m_pLayers[i]= new CBackgroundLayerLooping;	break;
			case CLSID_CBackgroundLayerTiled:	m_pLayers[i]= new CBackgroundLayerTiled;	break;
			default:							m_pLayers[i]= new CBackgroundLayer;			break;
			}

			m_pLayers[i]->LoadInstance(m_pXml);

		}m_pXml->OutOfElem();
	}

	m_pXml->FindElem("FILENAME_COLLISION");
	CDString temp= m_pXml->GetData();

	if(!temp.IsEmpty())
		LoadCollision(temp.m_arBuffer);


	SAFE_DELETE(m_pXml);
}
