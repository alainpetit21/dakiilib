#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "dfcEntity.h"

class CButton : public CEntity
{
public :
	enum eState
	{
		stateIdle		= 0,
		stateOver		= 1,
		stateSelected	= 2,
		stateFocused	= 3,
		stateClicked	= 4,
	};

	CButton();
	virtual ~CButton(){};
	IMPLEMENT_RTTI(Button, Entity);

	virtual void LoadInstance(const char* p_dataSource);
	virtual void Manage(u32 p_timeTick);
	virtual void Update(void);

	bool	m_bNoOverState;
	u32		m_nDelay;
	u32		m_nOldState;
};

#endif // _BUTTON_H_
