#ifndef _DFC_H_
#define _DFC_H_

typedef enum tagDFC_CLASSID
{								
	CLSID_CObj						= 0,	
	CLSID_CApp						= 1,	
	CLSID_CContext					= 2,	
	CLSID_CFont						= 3,	
	CLSID_CDictionary				= 4,	
	CLSID_CSceneEngine				= 5,	
	CLSID_CPhysObj					= 6,	
	CLSID_CBackground				= 7,	
	CLSID_CBackgroundLayer			= 8,	
	CLSID_CBackgroundLayerLooping	= 9,	
	CLSID_CBackgroundLayerTiled		= 10,	
	CLSID_CCamera2D					= 11,	
	CLSID_CCamera3D					= 12,	
	CLSID_CEntity					= 13,	
	CLSID_CEntity3D					= 14,	
	CLSID_CSprite3D					= 15,	
	CLSID_CMaze3D					= 16,	
	CLSID_CButton					= 17,	
	CLSID_CLabel					= 18,	
	CLSID_CLabelGradual				= 19,	
	CLSID_CSound					= 20,	
	CLSID_CMusic					= 21,	
	CLSID_LIBMAX					= 22,	

}DFC_CLASSID;

#endif // _DFC_H_
