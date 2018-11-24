#ifndef _OBJECT_MNG_H_
#define _OBJECT_MNG_H_

#include "lib.h"
#include "libTypes.h"
#include "dfc.h"

#include "libMngIO.h"

class CXml;
class CObj
{
public :
	CObj();
	CObj(const char* p_pRes);
	virtual ~CObj(void);

	virtual void LoadInstance(const char* p_dataSource);
	virtual void Manage(u32 p_timeTick);
	virtual void Update(void);

	virtual DFC_CLASSID Rtti(void)			{return CLSID_CObj;};
	virtual bool IsKindOf(DFC_CLASSID p_id)	{return (p_id == CLSID_CObj);};

	void SetTextID(const char *p_text);
	char* GetTextID(void){return m_stringID;};

	virtual void	SetNbChilds(u32 p_nNbElement);
	virtual u32		GetNbChilds()	{return m_nbChild;};
	virtual u32		AddChild(CObj* p_pObj, bool p_bTransformSetRelativity);
	virtual CObj*	RemoveChild(CObj* p_pObj);
	virtual CObj*	GetChild(int p_idxChild)	{return m_arChilds[p_idxChild];};

	const char	*m_pRes;
	char		*m_stringID;
	CObj		**m_arChilds;
	CObj		*m_pParent;
	u32			m_nbChild;
	u32			m_nbMaxChild;

	CXml		*m_pXml;	//Instanciated during LoadInstane only by CObj::LoadInstance ... user need to close it !!
};

#define IMPLEMENT_RTTI(className, derivateFrom)		\
	virtual DFC_CLASSID Rtti(void)					\
	{												\
		return (DFC_CLASSID)CLSID_C##className;		\
	};												\
	virtual bool IsKindOf(DFC_CLASSID p_id)			\
	{												\
		bool ret;									\
		if(p_id == (DFC_CLASSID)CLSID_C##className)	\
			return true;							\
		ret= C##derivateFrom::IsKindOf(p_id);		\
		return ret;									\
	}


#endif // _OBJECT_MNG_H_
