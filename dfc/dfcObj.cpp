#include "lib.h"
#include "libMacros.h"
#include "dfcObj.h"
#include "dfcApp.h"
#include "dfcPhysObj.h"
#include "dfcSceneEngine.h"
#include "dfcSound.h"
#include "dfcMusic.h"
#include "dfcBackground.h"
#include "dfcEntity.h"
#include "dfc3DEntity.h"
#include "dfc3DMaze.h"
#include "dfc3DSprite.h"
#include "dfcLabel.h"
#include "dfcButton.h"

CObj::CObj():
m_pRes(0),
m_stringID(0),
m_arChilds(0),
m_pParent(0),
m_nbChild(0),
m_nbMaxChild(0),
m_pXml(0)
{
}

CObj::CObj(const char * p_pRes):
m_pRes(p_pRes),
m_stringID(0),
m_arChilds(0),
m_pParent(0),
m_nbChild(0),
m_nbMaxChild(0),
m_pXml(0)
{
}

CObj::~CObj(void)
{
u32 i= 0;

	for(i= 0; i < m_nbChild; ++i){
		if(m_arChilds[i]){
			delete m_arChilds[i];
		}
	}

	SAFE_DELETE_ARRAY(m_stringID);
	SAFE_DELETE_ARRAY(m_arChilds);
}

void
CObj::LoadInstance(const char* p_dataSource)
{
CMngIO	*mngIO	= GetMngIO();
io 		fichier;
u32		size;
char 	*chaine;
char	strHeading[64];

	memset(strHeading, 0, sizeof(char)*64);
	switch(Rtti())
	{
	case CLSID_CObj:					strcpy(strHeading, "OBJECT");		break;
	case CLSID_CApp:					strcpy(strHeading, "APPLICATION");	break;
	case CLSID_CContext:				strcpy(strHeading, "CONTEXT");		break;
	case CLSID_CFont:					strcpy(strHeading, "FONT");			break;
	case CLSID_CDictionary:				strcpy(strHeading, "DICTIONARY");	break;
	case CLSID_CSceneEngine:			strcpy(strHeading, "SCENE");		break;
	case CLSID_CPhysObj:				strcpy(strHeading, "PHYS_OBJ");		break;
	case CLSID_CBackground:				strcpy(strHeading, "BACKGROUND");	break;
	case CLSID_CBackgroundLayer:		strcpy(strHeading, "CONTEXT");		break;
	case CLSID_CBackgroundLayerLooping:	strcpy(strHeading, "CONTEXT");		break;
	case CLSID_CCamera2D:				strcpy(strHeading, "CAMERA_2D");	break;
	case CLSID_CCamera3D:				strcpy(strHeading, "CAMERA_3D");	break;
	case CLSID_CEntity:					strcpy(strHeading, "ENTITY");		break;
	case CLSID_CEntity3D:				strcpy(strHeading, "3DENTITY");		break;
	case CLSID_CSprite3D:				strcpy(strHeading, "ENTITY");		break;
	case CLSID_CMaze3D:					strcpy(strHeading, "3DMAZE");		break;
	case CLSID_CButton:					strcpy(strHeading, "BUTTON");		break;
	case CLSID_CLabel:					strcpy(strHeading, "LABEL");		break;
	case CLSID_CLabelGradual:			strcpy(strHeading, "LABEL");		break;
	case CLSID_CSound:					strcpy(strHeading, "SOUND");		break;
	case CLSID_CMusic:					strcpy(strHeading, "SOUND");		break;
	default:	break;
	}

	helper_OpenScript(fichier, mngIO, p_dataSource);

	size	= mngIO->GetSize(fichier);
	chaine	= new char[size+1];
	chaine[size]= 0;

	mngIO->Read(fichier, chaine, size);
	mngIO->Close(fichier);

	/* Parse the xml*/
	m_pXml= new CXml(chaine);
	delete []chaine;

	m_pXml->FindElem(strHeading);
	m_pXml->IntoElem();{

		if(m_pXml->FindElem("NB_OBJECTS")){
			u32 nNbObject= atoi((m_pXml->GetData()).m_arBuffer);

			if(m_nbMaxChild < (nNbObject))
				SetNbChilds(nNbObject);

			for(u32 i= 0; i < nNbObject; ++i){
				CObj		*pObj= 0;
				CDString	strType;
				int 		nType;
				CDString	strID;
				CDString	strFilename;
				int 		pos[3]= {0, 0, 0};
				int 		initState=0;
				int 		initFrame=0;
				bool		initOn= true;

				if(m_pXml->FindElem("OBJECT")){
					m_pXml->IntoElem();{

						m_pXml->FindElem("TYPE");
						strType= m_pXml->GetData();
						nType= helper_StringToObjectType(strType.m_arBuffer);

						m_pXml->FindElem("ID");
						strID= m_pXml->GetData();

						m_pXml->FindElem("FILENAME");
						strFilename= m_pXml->GetData();

						if(m_pXml->FindElem("ON"))
							initOn= atoi((m_pXml->GetData()).m_arBuffer);

						if(m_pXml->FindElem("POS_X"))
							pos[0]= atoi((m_pXml->GetData()).m_arBuffer);

						if(m_pXml->FindElem("POS_Y"))
							pos[1]= (atoi((m_pXml->GetData()).m_arBuffer));

						if(m_pXml->FindElem("POS_Z"))
							pos[2]= (atoi((m_pXml->GetData()).m_arBuffer));

						if(m_pXml->FindElem("INIT_STATE"))
							initState= atoi((m_pXml->GetData()).m_arBuffer);

						if(m_pXml->FindElem("INIT_FRAME"))
							initFrame= atoi((m_pXml->GetData()).m_arBuffer);

						switch(nType){
						case CLSID_CObj:					pObj= new CObj;						break;
						case CLSID_CSceneEngine:			pObj= new CSceneEngine;				break;
						case CLSID_CFont:														break;
						case CLSID_CCamera2D:													break;
						case CLSID_CCamera3D:													break;
						case CLSID_CContext:													break;
						case CLSID_CPhysObj:				pObj= new CPhysObj;					break;
						case CLSID_CSound:					pObj= new CSound;					break;
						case CLSID_CMusic:					pObj= new CMusic;					break;
						case CLSID_CBackground:				pObj= new CBackground;				break;
						case CLSID_CBackgroundLayer:		pObj= new CBackgroundLayer;			break;
						case CLSID_CBackgroundLayerLooping:	pObj= new CBackgroundLayerLooping;	break;
						case CLSID_CEntity:					pObj= new CEntity;					break;
						case CLSID_CEntity3D:				pObj= new CEntity3D;				break;
						case CLSID_CSprite3D:				pObj= new CSprite3D;				break;
						case CLSID_CMaze3D:					pObj= new CMaze3D;					break;
						case CLSID_CLabel:					pObj= new CLabel;					break;
						case CLSID_CLabelGradual:			pObj= new CLabelGradual;			break;
						case CLSID_CButton:					pObj= new CButton;					break;
						default:							pObj= g_TheApp->GetCurContext()->UserInstall(strFilename.m_arBuffer, strType.m_arBuffer);	break;
						}

		#ifdef _DEBUG
						if(pObj){
                            if(pObj->IsKindOf(CLSID_CSceneEngine))
                                ASSERT2(i == (nNbObject - 1), "CLSID_CSceneEngine Obj Need to be last in context!! ");
						}else{
                            int toto=23;
                        }
		#endif
						if(pObj){
							pObj->LoadInstance(strFilename.m_arBuffer);
							pObj->SetTextID(strID.m_arBuffer);
							AddChild(pObj, false);

							if(pObj->IsKindOf(CLSID_CPhysObj)){
								((CPhysObj*)pObj)->SetPos(pos);
								((CPhysObj*)pObj)->Show(initOn);

								if(pObj->IsKindOf(CLSID_CEntity)){
									((CEntity*)pObj)->SetState(initState);
									((CEntity*)pObj)->SetFrame(initFrame);
								}
							}
						}
					}m_pXml->OutOfElem();
				}
			}
		}
	}
}


u32
CObj::AddChild(CObj* p_pObj, bool p_bTransformSetRelativity)
{
	if(m_nbMaxChild == m_nbChild)
		SetNbChilds(1+m_nbMaxChild+(m_nbMaxChild>>1));

	m_arChilds[m_nbChild++]= p_pObj;
	p_pObj->m_pParent= this;

	if(IsKindOf(CLSID_CPhysObj) && p_pObj->IsKindOf(CLSID_CPhysObj) && p_bTransformSetRelativity){
		CPhysObj *pCeci			= (CPhysObj *)this;
		CPhysObj *pFutureChild	= (CPhysObj *)p_pObj;

		pFutureChild->GetPosX()-= pCeci->GetPosX();
		pFutureChild->GetPosY()-= pCeci->GetPosY();
		pFutureChild->GetPosZ()-= pCeci->GetPosZ();
	}

	return m_nbChild-1;
}

CObj*
CObj::RemoveChild(CObj* p_pObj)
{
	for(u32 i= 0; i < m_nbChild; ++i){
		if(p_pObj == m_arChilds[i]){
			CObj *ret= p_pObj;

			if((m_nbChild-i) >1)
				memcpy(&m_arChilds[i], &m_arChilds[i+1], sizeof(CObj*)*(m_nbChild-i-1));

			--m_nbChild;

			if(IsKindOf(CLSID_CPhysObj) && p_pObj->IsKindOf(CLSID_CPhysObj)){
				CPhysObj *pCeci			= (CPhysObj *)this;
				CPhysObj *pOldChild		= (CPhysObj *)ret;

				pOldChild->GetPosX()+= pCeci->GetPosX();
				pOldChild->GetPosY()+= pCeci->GetPosY();
				pOldChild->GetPosZ()+= pCeci->GetPosZ();

				m_arChilds[i]= 0;
			}

			return ret;
		}
	}
	return 0;
}

void
CObj::SetNbChilds(u32 p_nNbElement)
{
	CObj **temp= new CObj*[p_nNbElement];
	memset(temp, 0, sizeof(CObj*)*p_nNbElement);

	memcpy(temp, m_arChilds, sizeof(CObj*)*m_nbMaxChild);
	SAFE_DELETE_ARRAY(m_arChilds);

	m_arChilds= temp;
	m_nbMaxChild= p_nNbElement;
}

void
CObj::Manage(unsigned long p_nTimeTick)
{
	for(u32 i= 0; i < m_nbChild; ++i)
		m_arChilds[i]->Manage(p_nTimeTick);

}

void
CObj::Update(void)
{
	for(u32 i= 0; i < m_nbChild; ++i)
		m_arChilds[i]->Update();

}

void
CObj::SetTextID(const char *p_text)
{
	m_stringID= new char [strlen(p_text)+1];
	memcpy(m_stringID, p_text, strlen(p_text)+1);
}
