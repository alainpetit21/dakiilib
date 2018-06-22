#ifndef _INPUT_WIN32_H
#define _INPUT_WIN32_H

//#include <SDL/SDL.h>
#include <SDL.h>

#define IK_1			SDLK_1
#define IK_2			SDLK_2
#define IK_3			SDLK_3
#define IK_4			SDLK_4
#define IK_5			SDLK_5
#define IK_6			SDLK_6
#define IK_7			SDLK_7
#define IK_8			SDLK_8
#define IK_9			SDLK_9
#define IK_0			SDLK_0
#define IK_F1			SDLK_F1
#define IK_F2			SDLK_F2
#define IK_F3			SDLK_F3
#define IK_F4			SDLK_F4
#define IK_F5			SDLK_F5
#define IK_F6			SDLK_F6
#define IK_F7			SDLK_F7
#define IK_F8			SDLK_F8
#define IK_F9			SDLK_F9
#define IK_F10			SDLK_F10
#define IK_F11			SDLK_F11
#define IK_F12			SDLK_F12
#define IK_MINUS		SDLK_LESS
#define IK_EQUALS		SDLK_EQUALS
#define IK_Q			SDLK_q
#define IK_W			SDLK_w
#define IK_E			SDLK_e
#define IK_R			SDLK_r
#define IK_T			SDLK_t
#define IK_Y			SDLK_y
#define IK_U			SDLK_u
#define IK_I			SDLK_i
#define IK_O			SDLK_o
#define IK_P			SDLK_p
#define IK_BACK			SDLK_BACKSLASH
#define IK_LBRACKET		SDLK_LEFTBRACKET
#define IK_RBRACKET		SDLK_RIGHTBRACKET
#define IK_RETURN		SDLK_RETURN
#define IK_A			SDLK_a
#define IK_S			SDLK_s
#define IK_D			SDLK_d
#define IK_F			SDLK_f
#define IK_G			SDLK_g
#define IK_H			SDLK_h
#define IK_J			SDLK_j
#define IK_K			SDLK_k
#define IK_L			SDLK_l
#define IK_SEMICOLON	SDLK_SEMICOLON
#define IK_APOSTROPHE	SDLK_BACKQUOTE
#define IK_BACKSLASH	SDLK_BACKSLASH
#define IK_Z			SDLK_z
#define IK_X			SDLK_x
#define IK_C			SDLK_c
#define IK_V			SDLK_v
#define IK_B			SDLK_b
#define IK_N			SDLK_n
#define IK_M			SDLK_m
#define IK_COMMA		SDLK_COMMA
#define IK_PERIOD		SDLK_PERIOD
#define IK_SLASH		SDLK_HASH
#define IK_MULTIPLY		SDLK_ASTERISK
#define IK_SPACE		SDLK_SPACE
#define IK_NUMPAD7		SDLK_KP_7
#define IK_NUMPAD8		SDLK_KP_8
#define IK_NUMPAD9		SDLK_KP_9
#define IK_SUBTRACT		SDLK_KP_MINUS
#define IK_NUMPAD4		SDLK_KP_4
#define IK_NUMPAD5		SDLK_KP_5
#define IK_NUMPAD6		SDLK_KP_6
#define IK_ADD			SDLK_KP_PLUS
#define IK_NUMPAD1		SDLK_KP_1
#define IK_NUMPAD2		SDLK_KP_2
#define IK_NUMPAD3		SDLK_KP_3
#define IK_NUMPAD0		SDLK_KP_0
#define IK_DECIMAL		SDLK_KP_PERIOD
#define IK_ESCAPE		SDLK_ESCAPE
#define IK_ARROWUP		SDLK_UP
#define IK_ARROWDOWN	SDLK_DOWN
#define IK_ARROWLEFT	SDLK_LEFT
#define IK_ARROWRIGHT	SDLK_RIGHT

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

#define IK_RIGHT		SDLK_RIGHT
#define IK_RIGHTDOWN	SDLK_DOWN
#define IK_DOWN			SDLK_DOWN
#define IK_DOWNLEFT		SDLK_DOWN
#define IK_LEFT			SDLK_LEFT
#define IK_LEFTUP		SDLK_UP
#define IK_UP			SDLK_UP
#define IK_UPRIGHT		SDLK_UP
#define IK_BTN_X		SDLK_x
#define IK_BTN_B		SDLK_b
#define IK_BTN_A		SDLK_a
#define IK_BTN_Y		SDLK_y
#define IK_BTN_L		SDLK_l
#define IK_BTN_R		SDLK_r
#define IK_BTN_SELECT	SDLK_SPACE
#define IK_BTN_START	SDLK_RETURN
#define IK_BTN_PAD		SDLK_ESCAPE
#define IK_BTN_VOLDOWN	SDLK_KP_MINUS
#define IK_BTN_VOLUP	SDLK_KP_PLUS

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

//	u8		m_winKeyboard[SDLK_LAST];TODO
//	u8		m_lastWinKeyboard[SDLK_LAST];TODO
	u8		m_winKeyboard[512];
	u8		m_lastWinKeyboard[512];

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
