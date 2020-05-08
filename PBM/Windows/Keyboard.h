#pragma once
#define MOUSE_LEFT_BUTTON VK_LBUTTON
#define MOUSE_RIGHT_BUTTON VK_RBUTTON
#define MOUSE_MIDDLE_BUTTON VK_MBUTTON
#define KEY_BACK VK_BACK
#define KEY_TAB VK_TAB
#define KEY_RETURN VK_RETURN
#define KEY_SHIFT VK_SHIFT
#define KEY_CONTROL VK_CONTROL
#define KEY_ALT VK_MENU
#define KEY_CAPITAL VK_CAPITAL
#define KEY_ESCAPE VK_ESCAPE
#define KEY_SPACE VK_SPACE
#define KEY_LEFT VK_LEFT
#define KEY_UP VK_UP
#define KEY_RIGHT VK_RIGHT
#define KEY_DOWN VK_DOWN
#define KEY_0 0x30
#define KEY_1 0x31
#define KEY_2 0x32
#define KEY_3 0x33
#define KEY_4 0x34
#define KEY_5 0x35
#define KEY_6 0x36
#define KEY_7 0x37
#define KEY_8 0x38
#define KEY_9 0x39
#define KEY_A 0x41
#define KEY_B 0x42
#define KEY_C 0x43
#define KEY_D 0x44
#define KEY_E 0x45
#define KEY_F 0x46
#define KEY_G 0x47
#define KEY_H 0x48
#define KEY_I 0x49
#define KEY_J 0x4a
#define KEY_K 0x4b
#define KEY_L 0x4c
#define KEY_M 0x4d
#define KEY_N 0x4e
#define KEY_O 0x4f
#define KEY_P 0x50
#define KEY_Q 0x51
#define KEY_R 0x52
#define KEY_S 0x53
#define KEY_T 0x54
#define KEY_U 0x55
#define KEY_V 0x56
#define KEY_W 0x57
#define KEY_X 0x58
#define KEY_Y 0x59
#define KEY_Z 0x5a
#define KEY_NUMPAD1 VK_NUMPAD1
#define KEY_NUMPAD2 VK_NUMPAD2
#define KEY_NUMPAD3 VK_NUMPAD3
#define KEY_NUMPAD4 VK_NUMPAD4
#define KEY_NUMPAD5 VK_NUMPAD5
#define KEY_NUMPAD6 VK_NUMPAD6
#define KEY_NUMPAD7 VK_NUMPAD7
#define KEY_NUMPAD8VK_NUMPAD 8
#define KEY_NUMPAD9 VK_NUMPAD9
#define COUNT_KEY_CODE 0xFF
const char* InterpretKeyCode(KeyCode code);

class Keyboard
{
public:
	Keyboard();
	inline bool CheckKeyState(KeyCode code, KEY_STATE state) const;
	void RegisterKeyStateListener(KeyCode code, KEY_STATE state, CallKeyStateChange* object);
	void UnregisterKeyStateListener(KeyCode code, KEY_STATE state, CallKeyStateChange* object);
	void SetKeyState(KeyCode code, KEY_STATE state);
	inline bool IsPressing(KeyCode code) const;
	inline bool IsDown(KeyCode code) const;
protected:
	vector<CallKeyStateChange*> m_Listeners[COUNT_KEY_CODE][KEY_STATE_NUM];
	KEY_STATE m_KeyState[COUNT_KEY_CODE] = { KEY_STATE_NONE };
};

bool Keyboard::CheckKeyState(KeyCode code, KEY_STATE state) const
{
	return m_KeyState[code] == state;
}

bool Keyboard::IsPressing(KeyCode code)  const {
	return CheckKeyState(code, KEY_STATE_DOWN) || CheckKeyState(code, KEY_STATE_PRESS);
}

bool Keyboard::IsDown(KeyCode code) const
{
	return CheckKeyState(code, KEY_STATE_DOWN);
}

