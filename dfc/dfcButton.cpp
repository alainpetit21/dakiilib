#include "lib.h"
#include "libMacros.h"
#include "libMngGraphic.h"
#include "dfcButton.h"
#include "dfcContext.h"

CButton::CButton():
m_nDelay(0),
m_nOldState(0)
{
};

void
CButton::LoadInstance(const char* p_dataSource)
{
	CObj::LoadInstance(p_dataSource);

	m_pXml->FindElem("NB_FRAMES");
	m_nbFrame= atoi((m_pXml->GetData()).m_arBuffer);

	u32		i;

	m_arFrames= new TRuntimeFrame[m_nbFrame];

	for(i= 0; i < m_nbFrame; ++i){
		m_pXml->FindElem("FRAME");
		m_pXml->IntoElem();{

			m_pXml->FindElem("FILENAME");
			LoadTGA((m_pXml->GetData()).m_arBuffer, i);

			m_pXml->FindElem("POS_X");
			m_arFrames[i].nHotSpotX= atoi((m_pXml->GetData()).m_arBuffer);

			m_pXml->FindElem("POS_Y");
			m_arFrames[i].nHotSpotY= atoi((m_pXml->GetData()).m_arBuffer);

		}m_pXml->OutOfElem();
	}

	m_nbState= 5;

	m_arState= new TRuntimeFrame*[m_nbState+1];
	m_arSpeed= new u32[m_nbState+1];

	//State 0
	CDString chaineTemp= "STATE_IDLE";

	m_pXml->FindElem(chaineTemp.m_arBuffer);
	m_pXml->IntoElem();{

		m_pXml->FindElem("FLAG");
		m_arState[0]= &m_arFrames[atoi((m_pXml->GetData()).m_arBuffer)];

		m_pXml->FindElem("SPEED");
		m_arSpeed[0]= atoi((m_pXml->GetData()).m_arBuffer);
	}m_pXml->OutOfElem();

	//State 1
	chaineTemp= "STATE_OVER";

	m_pXml->FindElem(chaineTemp.m_arBuffer);
	m_pXml->IntoElem();{

		m_pXml->FindElem("FLAG");
		m_arState[1]= &m_arFrames[atoi((m_pXml->GetData()).m_arBuffer)];

		m_pXml->FindElem("SPEED");
		m_arSpeed[1]= atoi((m_pXml->GetData()).m_arBuffer);
	}m_pXml->OutOfElem();

	//State 2
	chaineTemp= "STATE_SELECTED";

	m_pXml->FindElem(chaineTemp.m_arBuffer);
	m_pXml->IntoElem();{

		m_pXml->FindElem("FLAG");
		m_arState[2]= &m_arFrames[atoi((m_pXml->GetData()).m_arBuffer)];

		m_pXml->FindElem("SPEED");
		m_arSpeed[2]= atoi((m_pXml->GetData()).m_arBuffer);
	}m_pXml->OutOfElem();

	//State 3
	chaineTemp= "STATE_FOCUSED";

	m_pXml->FindElem(chaineTemp.m_arBuffer);
	m_pXml->IntoElem();{

		m_pXml->FindElem("FLAG");
		m_arState[3]= &m_arFrames[atoi((m_pXml->GetData()).m_arBuffer)];

		m_pXml->FindElem("SPEED");
		m_arSpeed[3]= atoi((m_pXml->GetData()).m_arBuffer);
	}m_pXml->OutOfElem();


	//State 4
	chaineTemp= "STATE_CLICKED";

	m_pXml->FindElem(chaineTemp.m_arBuffer);
	m_pXml->IntoElem();{

		m_pXml->FindElem("FLAG");
		m_arState[4]= &m_arFrames[atoi((m_pXml->GetData()).m_arBuffer)];

		m_pXml->FindElem("SPEED");
		m_arSpeed[4]= atoi((m_pXml->GetData()).m_arBuffer);
	}m_pXml->OutOfElem();


	m_arState[m_nbState]= &m_arFrames[m_nbFrame];

	m_pCurAnim	= m_arState[m_curState= 0];
	m_indexFrame= 0.0f;
	m_speed		= float(int(m_arSpeed[0]));

	SetAnimationSpeed(1.0);

	if(m_arState[1] == m_arState[2])
		m_bNoOverState= true;
	else
		m_bNoOverState= false;

	SAFE_DELETE(m_pXml);
}

void
CButton::Manage(u32 p_nTimeTick)
{
	CEntity::Manage(p_nTimeTick);

	if(m_nDelay)
		if(!(--m_nDelay))
			SetState(m_nOldState);
}

void
CButton::Update(void)
{
	if(m_bNoOverState && (GetState() == CButton::stateOver)){
		SetState(m_nOldState);
		CEntity::Update();
		SetState(CButton::stateOver);
	}else
		CEntity::Update();
}
