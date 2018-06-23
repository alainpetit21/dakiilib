#include <unistd.h>
#include <fcntl.h>

#include <linux/joystick.h>

#include "lib.h"
#include "libTypes.h"
#include "libMacros.h"
#include "libMngInput.h"
#include "libMngGraphic.h"

const char *js_device[5] = {"/dev/js0", "/dev/js1", "/dev/js2", "/dev/js3", "/dev/js4"};
int js_fd[5] = {-1, -1, -1, -1, -1};

CMngInput* CMngInput::singletonInstance= 0;

CMngInput::CMngInput()
{
	ASSERT2(!singletonInstance, "Another instance of CMngInput is already instanced");

	singletonInstance= this;
}

CMngInput::~CMngInput()
{
	singletonInstance= 0;
};

void
CMngInput::Init(bool p_bExclusive)
{
int	version;

	m_posMouseX= 0;
	m_posMouseY= 0;

	if((js_fd[0]= open(js_device[0], O_RDONLY | O_NONBLOCK)) < 0)
		return;

	if(ioctl(js_fd[0], JSIOCGVERSION, &version))
	{
		printf("joystick: You need at least driver version 1.0 for joystick support.\n");
		close(js_fd[0]);
		return;
	}

	js_fd[1]= open(js_device[1], O_RDONLY | O_NONBLOCK);
	js_fd[2]= open(js_device[2], O_RDONLY | O_NONBLOCK);
	js_fd[3]= open(js_device[3], O_RDONLY | O_NONBLOCK);
}

void
CMngInput::Exit(void)
{
}

void
CMngInput::Manage(unsigned long p_nTimeTick)
{
struct js_event js_ev;
int 			i;

	if(!System_IsActive())
		return;

	memcpy(&m_lastMouseState, &m_mouseState, sizeof(m_mouseState));
	memcpy(&m_mouseState, &m_mouseStateBB, sizeof(m_mouseStateBB));

	memcpy(m_lastWinKeyboard, m_winKeyboard, sizeof(m_winKeyboard));
//TODO	memcpy(m_winKeyboard, SDL_GetKeyState(0), sizeof(m_winKeyboard));

	for(i= 0; i < 5; ++i){
		memcpy(&m_lastWinJoystick[i], &m_winJoystick[i], sizeof(m_winJoystick[i]));

		while(read(js_fd[i], &js_ev, sizeof(struct js_event)) == sizeof (struct js_event)){
			switch(js_ev.type & ~JS_EVENT_INIT){
			case JS_EVENT_AXIS:

				if(js_ev.number == 0){
					if(js_ev.value < -16384){
						m_winJoystick[i]|= IJ_LEFT;
						m_winJoystick[i]&= ~IJ_RIGHT;
					}else if(js_ev.value > 16384){
						m_winJoystick[i]&= ~IJ_LEFT;
						m_winJoystick[i]|= IJ_RIGHT;
					}else{
						m_winJoystick[i]&= ~IJ_LEFT;
						m_winJoystick[i]&= ~IJ_RIGHT;
					}
				}

				if(js_ev.number == 1){
					if(js_ev.value < -16384){
						m_winJoystick[i]|= IJ_UP;
						m_winJoystick[i]&= ~IJ_DOWN;
					}else if(js_ev.value > 16384){
						m_winJoystick[i]&= ~IJ_UP;
						m_winJoystick[i]|= IJ_DOWN;
					}else{
						m_winJoystick[i]&= ~IJ_UP;
						m_winJoystick[i]&= ~IJ_DOWN;
					}
				}
			break;
			case JS_EVENT_BUTTON:

				if(js_ev.number > 15)
					break;
				if(js_ev.value)
					m_winJoystick[i]|= IJ_BTNN(js_ev.number);
				else
					m_winJoystick[i]&= ~IJ_BTNN(js_ev.number);
			break;
			}
		}
	}
}

bool
CMngInput::IsKeyDown(u32 p_nKey)
{/*TODO
	return m_winKeyboard[p_nKey];*/
    return false;
}

bool
CMngInput::IsLogicKeyDown(u32 p_nKey)
{
	return (m_winKeyboard[p_nKey] && !m_lastWinKeyboard[p_nKey]);
}

bool
CMngInput::IsJoystickKeyDown(u32 p_idxJoystick, u32 p_nKey)
{
	return (m_winJoystick[p_idxJoystick] & p_nKey);
}

bool
CMngInput::IsJoystickLogicKeyDown(u32 p_idxJoystick, u32 p_nKey)
{
	return ((m_winJoystick[p_idxJoystick] & p_nKey) && !(m_lastWinJoystick[p_nKey] & p_nKey));
}

bool
CMngInput::IsMouseKeyDown(u32 p_nKey)
{
	if(p_nKey==1)		p_nKey= 2;
	else if(p_nKey==2)	p_nKey= 1;

	return m_mouseState[p_nKey];
}

bool
CMngInput::IsMouseLogicKeyDown(u32 p_nKey)
{
	if(p_nKey==1)		p_nKey= 2;
	else if(p_nKey==2)	p_nKey= 1;

	return (m_mouseState[p_nKey] && !m_lastMouseState[p_nKey]);
}
void
CMngInput::GetMousePos(s32 &p_nPosX, s32 &p_nPosY)
{/*TODO
CMngGraphic	*mngGraphic	= GetMngGraphic();
int x, y;

	SDL_PumpEvents();
	SDL_GetMouseState(&x, &y);

	p_nPosX= m_posMouseX= x - mngGraphic->GetWidth(mngGraphic->GetBB())/2;
	p_nPosY= m_posMouseY= y - mngGraphic->GetHeight(mngGraphic->GetBB())/2;*/
}

void
CMngInput::SetMouseBouton(s32 p_idxBouton, int p_isOn)
{
	m_mouseStateBB[p_idxBouton]= p_isOn;
}


char
CMngInput::GetAsciiCharDown(void)
{
	return 0;
}

int
CMngInput::AsciiToID(char *p_strKey)
{
	if(strcmp(p_strKey, "IK_F1") == 0)				return IK_F1;
	else if(strcmp(p_strKey, "IK_F2") == 0)			return IK_F2;
	else if(strcmp(p_strKey, "IK_F3") == 0)			return IK_F3;
	else if(strcmp(p_strKey, "IK_F4") == 0)			return IK_F4;
	else if(strcmp(p_strKey, "IK_F5") == 0)			return IK_F5;
	else if(strcmp(p_strKey, "IK_F6") == 0)			return IK_F6;
	else if(strcmp(p_strKey, "IK_F7") == 0)			return IK_F7;
	else if(strcmp(p_strKey, "IK_F8") == 0)			return IK_F8;
	else if(strcmp(p_strKey, "IK_F9") == 0)			return IK_F9;
	else if(strcmp(p_strKey, "IK_F10") == 0)		return IK_F10;
	else if(strcmp(p_strKey, "IK_F11") == 0)		return IK_F11;
	else if(strcmp(p_strKey, "IK_F12") == 0)		return IK_F12;
	else if(strcmp(p_strKey, "IK_1") == 0)			return IK_1;
	else if(strcmp(p_strKey, "IK_2") == 0)			return IK_2;
	else if(strcmp(p_strKey, "IK_3") == 0)			return IK_3;
	else if(strcmp(p_strKey, "IK_4") == 0)			return IK_4;
	else if(strcmp(p_strKey, "IK_5") == 0)			return IK_5;
	else if(strcmp(p_strKey, "IK_6") == 0)			return IK_6;
	else if(strcmp(p_strKey, "IK_7") == 0)			return IK_7;
	else if(strcmp(p_strKey, "IK_8") == 0)			return IK_8;
	else if(strcmp(p_strKey, "IK_9") == 0)			return IK_9;
	else if(strcmp(p_strKey, "IK_0") == 0)			return IK_0;
	else if(strcmp(p_strKey, "IK_MINUS") == 0)		return IK_MINUS;
	else if(strcmp(p_strKey, "IK_EQUALS") == 0)		return IK_EQUALS;
	else if(strcmp(p_strKey, "IK_Q") == 0)			return IK_Q;
	else if(strcmp(p_strKey, "IK_W") == 0)			return IK_W;
	else if(strcmp(p_strKey, "IK_E") == 0)			return IK_E;
	else if(strcmp(p_strKey, "IK_R") == 0)			return IK_R;
	else if(strcmp(p_strKey, "IK_T") == 0)			return IK_T;
	else if(strcmp(p_strKey, "IK_Y") == 0)			return IK_Y;
	else if(strcmp(p_strKey, "IK_U") == 0)			return IK_U;
	else if(strcmp(p_strKey, "IK_I") == 0)			return IK_I;
	else if(strcmp(p_strKey, "IK_O") == 0)			return IK_O;
	else if(strcmp(p_strKey, "IK_P") == 0)			return IK_P;
	else if(strcmp(p_strKey, "IK_BACK") == 0)		return IK_BACK;
	else if(strcmp(p_strKey, "IK_LBRACKET") == 0)	return IK_LBRACKET;
	else if(strcmp(p_strKey, "IK_RBRACKET") == 0)	return IK_RBRACKET;
	else if(strcmp(p_strKey, "IK_RETURN") == 0)		return IK_RETURN;
	else if(strcmp(p_strKey, "IK_A") == 0)			return IK_A;
	else if(strcmp(p_strKey, "IK_S") == 0)			return IK_S;
	else if(strcmp(p_strKey, "IK_D") == 0)			return IK_D;
	else if(strcmp(p_strKey, "IK_F") == 0)			return IK_F;
	else if(strcmp(p_strKey, "IK_G") == 0)			return IK_G;
	else if(strcmp(p_strKey, "IK_H") == 0)			return IK_H;
	else if(strcmp(p_strKey, "IK_J") == 0)			return IK_J;
	else if(strcmp(p_strKey, "IK_K") == 0)			return IK_K;
	else if(strcmp(p_strKey, "IK_L") == 0)			return IK_L;
	else if(strcmp(p_strKey, "IK_SEMICOLON") == 0)	return IK_SEMICOLON;
	else if(strcmp(p_strKey, "IK_APOSTROPHE") == 0)	return IK_APOSTROPHE;
	else if(strcmp(p_strKey, "IK_BACKSLASH") == 0)	return IK_BACKSLASH;
	else if(strcmp(p_strKey, "IK_Z") == 0)			return IK_Z;
	else if(strcmp(p_strKey, "IK_X") == 0)			return IK_X;
	else if(strcmp(p_strKey, "IK_C") == 0)			return IK_C;
	else if(strcmp(p_strKey, "IK_V") == 0)			return IK_V;
	else if(strcmp(p_strKey, "IK_B") == 0)			return IK_B;
	else if(strcmp(p_strKey, "IK_N") == 0)			return IK_N;
	else if(strcmp(p_strKey, "IK_M") == 0)			return IK_M;
	else if(strcmp(p_strKey, "IK_COMMA") == 0)		return IK_COMMA;
	else if(strcmp(p_strKey, "IK_PERIOD") == 0)		return IK_PERIOD;
	else if(strcmp(p_strKey, "IK_SLASH") == 0)		return IK_SLASH;
	else if(strcmp(p_strKey, "IK_MULTIPLY") == 0)	return IK_MULTIPLY;
	else if(strcmp(p_strKey, "IK_SPACE") == 0)		return IK_SPACE;
	else if(strcmp(p_strKey, "IK_NUMPAD7") == 0)	return IK_NUMPAD7;
	else if(strcmp(p_strKey, "IK_NUMPAD8") == 0)	return IK_NUMPAD8;
	else if(strcmp(p_strKey, "IK_NUMPAD9") == 0)	return IK_NUMPAD9;
	else if(strcmp(p_strKey, "IK_SUBTRACT") == 0)	return IK_SUBTRACT;
	else if(strcmp(p_strKey, "IK_NUMPAD4") == 0)	return IK_NUMPAD4;
	else if(strcmp(p_strKey, "IK_NUMPAD5") == 0)	return IK_NUMPAD5;
	else if(strcmp(p_strKey, "IK_NUMPAD6") == 0)	return IK_NUMPAD6;
	else if(strcmp(p_strKey, "IK_ADD") == 0)		return IK_ADD;
	else if(strcmp(p_strKey, "IK_NUMPAD1") == 0)	return IK_NUMPAD1;
	else if(strcmp(p_strKey, "IK_NUMPAD2") == 0)	return IK_NUMPAD2;
	else if(strcmp(p_strKey, "IK_NUMPAD3") == 0)	return IK_NUMPAD3;
	else if(strcmp(p_strKey, "IK_NUMPAD0") == 0)	return IK_NUMPAD0;
	else if(strcmp(p_strKey, "IK_DECIMAL") == 0)	return IK_DECIMAL;
	else if(strcmp(p_strKey, "IK_ESCAPE") == 0)		return IK_ESCAPE;
	else if(strcmp(p_strKey, "IK_ARROWUP") == 0)	return IK_ARROWUP;
	else if(strcmp(p_strKey, "IK_ARROWDOWN") == 0)	return IK_ARROWDOWN;
	else if(strcmp(p_strKey, "IK_ARROWLEFT") == 0)	return IK_ARROWLEFT;
	else if(strcmp(p_strKey, "IK_ARROWRIGHT") == 0)	return IK_ARROWRIGHT;
	else if(strcmp(p_strKey, "IJ_UP") == 0)			return IJ_UP;
	else if(strcmp(p_strKey, "IJ_DOWN") == 0)		return IJ_DOWN;
	else if(strcmp(p_strKey, "IJ_LEFT") == 0)		return IJ_LEFT;
	else if(strcmp(p_strKey, "IJ_RIGHT") == 0)		return IJ_RIGHT;
	else if(strcmp(p_strKey, "IJ_BTN0") == 0)		return IJ_BTN0;
	else if(strcmp(p_strKey, "IJ_BTN1") == 0)		return IJ_BTN1;
	else if(strcmp(p_strKey, "IJ_BTN2") == 0)		return IJ_BTN2;
	else if(strcmp(p_strKey, "IJ_BTN3") == 0)		return IJ_BTN3;
	else if(strcmp(p_strKey, "IJ_BTN4") == 0)		return IJ_BTN4;
	else if(strcmp(p_strKey, "IJ_BTN5") == 0)		return IJ_BTN5;
	else if(strcmp(p_strKey, "IJ_BTN6") == 0)		return IJ_BTN6;
	else if(strcmp(p_strKey, "IJ_BTN7") == 0)		return IJ_BTN7;
	else if(strcmp(p_strKey, "IJ_BTN8") == 0)		return IJ_BTN8;
	else if(strcmp(p_strKey, "IJ_BTN9") == 0)		return IJ_BTN9;
	else if(strcmp(p_strKey, "IJ_BTNA") == 0)		return IJ_BTNA;
	else if(strcmp(p_strKey, "IJ_BTNB") == 0)		return IJ_BTNB;
	else if(strcmp(p_strKey, "IJ_BTNC") == 0)		return IJ_BTNC;
	else if(strcmp(p_strKey, "IJ_BTND") == 0)		return IJ_BTND;
	else if(strcmp(p_strKey, "IJ_BTNE") == 0)		return IJ_BTNE;
	else if(strcmp(p_strKey, "IJ_BTNF") == 0)		return IJ_BTNF;

	return 0;
}


void
CMngInput::Reset(void)
{
	Manage(1);
}
