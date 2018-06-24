#ifndef _INPUT_WIN32_H
#define _INPUT_WIN32_H

#include <SDL.h>

#define IK_1			SDL_SCANCODE_1
#define IK_2			SDL_SCANCODE_2
#define IK_3			SDL_SCANCODE_3
#define IK_4			SDL_SCANCODE_4
#define IK_5			SDL_SCANCODE_5
#define IK_6			SDL_SCANCODE_6
#define IK_7			SDL_SCANCODE_7
#define IK_8			SDL_SCANCODE_8
#define IK_9			SDL_SCANCODE_9
#define IK_0			SDL_SCANCODE_0
#define IK_F1			SDL_SCANCODE_F1
#define IK_F2			SDL_SCANCODE_F2
#define IK_F3			SDL_SCANCODE_F3
#define IK_F4			SDL_SCANCODE_F4
#define IK_F5			SDL_SCANCODE_F5
#define IK_F6			SDL_SCANCODE_F6
#define IK_F7			SDL_SCANCODE_F7
#define IK_F8			SDL_SCANCODE_F8
#define IK_F9			SDL_SCANCODE_F9
#define IK_F10			SDL_SCANCODE_F10
#define IK_F11			SDL_SCANCODE_F11
#define IK_F12			SDL_SCANCODE_F12
#define IK_MINUS		SDL_SCANCODE_MINUS
#define IK_EQUALS		SDL_SCANCODE_EQUALS
#define IK_Q			SDL_SCANCODE_Q
#define IK_W			SDL_SCANCODE_W
#define IK_E			SDL_SCANCODE_E
#define IK_R			SDL_SCANCODE_R
#define IK_T			SDL_SCANCODE_T
#define IK_Y			SDL_SCANCODE_Y
#define IK_U			SDL_SCANCODE_U
#define IK_I			SDL_SCANCODE_I
#define IK_O			SDL_SCANCODE_O
#define IK_P			SDL_SCANCODE_P
#define IK_BACK			SDL_SCANCODE_BACKSLASH
#define IK_LBRACKET		SDL_SCANCODE_LEFTBRACKET
#define IK_RBRACKET		SDL_SCANCODE_RIGHTBRACKET
#define IK_RETURN		SDL_SCANCODE_RETURN
#define IK_A			SDL_SCANCODE_A
#define IK_S			SDL_SCANCODE_S
#define IK_D			SDL_SCANCODE_D
#define IK_F			SDL_SCANCODE_F
#define IK_G			SDL_SCANCODE_G
#define IK_H			SDL_SCANCODE_H
#define IK_J			SDL_SCANCODE_J
#define IK_K			SDL_SCANCODE_K
#define IK_L			SDL_SCANCODE_L
#define IK_SEMICOLON	SDL_SCANCODE_SEMICOLON
#define IK_APOSTROPHE	SDL_SCANCODE_APOSTROPHE
#define IK_BACKSLASH	SDL_SCANCODE_BACKSLASH
#define IK_Z			SDL_SCANCODE_Z
#define IK_X			SDL_SCANCODE_X
#define IK_C			SDL_SCANCODE_C
#define IK_V			SDL_SCANCODE_V
#define IK_B			SDL_SCANCODE_B
#define IK_N			SDL_SCANCODE_N
#define IK_M			SDL_SCANCODE_M
#define IK_COMMA		SDL_SCANCODE_COMMA
#define IK_PERIOD		SDL_SCANCODE_PERIOD
#define IK_SLASH		SDL_SCANCODE_SLASH
#define IK_MULTIPLY		SDL_SCANCODE_KP_MULTIPLY
#define IK_SPACE		SDL_SCANCODE_SPACE
#define IK_NUMPAD7		SDL_SCANCODE_KP_7
#define IK_NUMPAD8		SDL_SCANCODE_KP_8
#define IK_NUMPAD9		SDL_SCANCODE_KP_9
#define IK_SUBTRACT		SDL_SCANCODE_KP_MINUS
#define IK_NUMPAD4		SDL_SCANCODE_KP_4
#define IK_NUMPAD5		SDL_SCANCODE_KP_5
#define IK_NUMPAD6		SDL_SCANCODE_KP_6
#define IK_ADD			SDL_SCANCODE_KP_PLUS
#define IK_NUMPAD1		SDL_SCANCODE_KP_1
#define IK_NUMPAD2		SDL_SCANCODE_KP_2
#define IK_NUMPAD3		SDL_SCANCODE_KP_3
#define IK_NUMPAD0		SDL_SCANCODE_KP_0
#define IK_DECIMAL		SDL_SCANCODE_KP_PERIOD
#define IK_ESCAPE		SDL_SCANCODE_ESCAPE
#define IK_ARROWUP		SDL_SCANCODE_UP
#define IK_ARROWDOWN	SDL_SCANCODE_DOWN
#define IK_ARROWLEFT	SDL_SCANCODE_LEFT
#define IK_ARROWRIGHT	SDL_SCANCODE_RIGHT

#define IJ_UP		0x00000001
#define IJ_DOWN		0x00000002
#define IJ_LEFT		0x00000004
#define IJ_RIGHT	0x00000008
#define IJ_BTN0		0x00000010
#define IJ_BTN1		0x00000020
#define IJ_BTN2		0x00000040
#define IJ_BTN3		0x00000080
#define IJ_BTN4		0x00000100
#define IJ_BTN5		0x00000200
#define IJ_BTN6		0x00000400
#define IJ_BTN7		0x00000800
#define IJ_BTN8		0x00001000
#define IJ_BTN9		0x00002000
#define IJ_BTNA		0x00004000
#define IJ_BTNB		0x00008000
#define IJ_BTNC		0x00010000
#define IJ_BTND		0x00020000
#define IJ_BTNE		0x00040000
#define IJ_BTNF		0x00080000
#define IJ_BTNN(n)	(1<<(n+4))

#define IK_RIGHT		SDL_SCANCODE_RIGHT
#define IK_RIGHTDOWN	SDL_SCANCODE_DOWN
#define IK_DOWN			SDL_SCANCODE_DOWN
#define IK_DOWNLEFT		SDL_SCANCODE_DOWN
#define IK_LEFT			SDL_SCANCODE_LEFT
#define IK_LEFTUP		SDL_SCANCODE_UP
#define IK_UP			SDL_SCANCODE_UP
#define IK_UPRIGHT		SDL_SCANCODE_UP
#define IK_BTN_X		SDL_SCANCODE_X
#define IK_BTN_B		SDL_SCANCODE_B
#define IK_BTN_A		SDL_SCANCODE_A
#define IK_BTN_Y		SDL_SCANCODE_Y
#define IK_BTN_L		SDL_SCANCODE_L
#define IK_BTN_R		SDL_SCANCODE_R
#define IK_BTN_SELECT	SDL_SCANCODE_SPACE
#define IK_BTN_START	SDL_SCANCODE_RETURN
#define IK_BTN_PAD		SDL_SCANCODE_ESCAPE
#define IK_BTN_VOLDOWN	SDL_SCANCODE_KP_MINUS
#define IK_BTN_VOLUP	SDL_SCANCODE_KP_PLUS

#define IJ_AXE_X	0x00
#define IJ_AXE_Y	0x01

class CMngInput
{
public:

	CMngInput();
	virtual ~CMngInput();

	virtual void	Init			(bool p_bExclusive);
	virtual void	Exit			(void);

	virtual	void	Manage			(unsigned long p_nTimeTick);

	virtual	bool	IsKeyDown		(u32 p_key);
	virtual	bool	IsLogicKeyDown	(u32 p_key);
	virtual	char	GetAsciiCharDown(void);
	virtual int		AsciiToID		(char *p_strKey);

	virtual	bool	IsMouseKeyDown		(u32 p_key);
	virtual	bool	IsMouseLogicKeyDown	(u32 p_key);
	virtual	void	GetMousePos			(s32 &p_posX, s32 &p_posY);

	virtual void	SetDelayKb				(u32 p_nDelay)					{m_delaytoHold= p_nDelay;};
	virtual	bool	IsJoystickKeyDown		(u32 p_idxJoystick, u32 p_key);
	virtual	bool	IsJoystickLogicKeyDown	(u32 p_idxJoystick, u32 p_key);

	virtual void	Reset(void);

	void	SetMouseBouton(s32 p_idxBouton, int p_isOn);

	u8		m_winKeyboard[SDL_NUM_SCANCODES];
	u8		m_lastWinKeyboard[SDL_NUM_SCANCODES];

	int		m_mouseStateBB[3];
	int		m_mouseState[3];
	int		m_lastMouseState[3];
	s32		m_posMouseX;
	s32		m_posMouseY;

	u32		m_winJoystick[5];
	u32		m_lastWinJoystick[5];

	u32		m_delaytoHold;

	static CMngInput	*singletonInstance;
};

inline CMngInput*
GetMngInput(void)
{
	return CMngInput::singletonInstance? CMngInput::singletonInstance : new CMngInput;
}

#endif // _RENDER_WIN32_H
