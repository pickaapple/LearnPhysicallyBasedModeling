#include "../stdafx.h"
#include "Keyboard.h"

Keyboard::Keyboard() {}


void Keyboard::SetKeyState(KeyCode code, KEY_STATE state)
{
	//出现两次Down事件 表明为 Press事件
	//已经为Press事件时 忽略 Down事件
	if ((m_KeyState[code] == KEY_STATE_DOWN || m_KeyState[code] == KEY_STATE_PRESS)
		&& state == KEY_STATE_DOWN)
		state = KEY_STATE_PRESS;
	m_KeyState[code] = state;
	auto& listeners = m_Listeners[code][state];
	for (auto listener : listeners) 
		listener->KeyboardStateChange(code, state);
	//cout << "SetKeyState code:" << InterpretKeyCode(code) << " state: " << m_KeyState[code] << endl;
}

void Keyboard::RegisterKeyStateListener(KeyCode code, KEY_STATE state, CallKeyStateChange* object)
{
	m_Listeners[code][state].push_back(object);
}

void Keyboard::UnregisterKeyStateListener(KeyCode code, KEY_STATE state, CallKeyStateChange* object)
{
	auto &ids = m_Listeners[code][state];
	for (auto itr = ids.begin(); itr != ids.end(); ++itr) {
		if (*itr == object) {
			ids.erase(itr);
			break;
		}
	}
}

const char* InterpretKeyCode(KeyCode code)
{
	switch (code) {
	case KEY_BACK:return"back";
	case KEY_TAB:return"tab";
	case KEY_RETURN:return"return";
	case KEY_SHIFT:return"shift";
	case KEY_CONTROL:return"control";
	case KEY_ALT:return"alt";

	case KEY_0:return "0";
	case KEY_1:return "1";
	case KEY_2:return "2";
	case KEY_3:return "3";
	case KEY_4:return "4";
	case KEY_5:return "5";
	case KEY_6:return "6";
	case KEY_7:return "7";
	case KEY_8:return "8";
	case KEY_9:return"9";
	case KEY_A: return "A";
	case KEY_B: return "B";
	case KEY_C: return "C";
	case KEY_D: return "D";
	case KEY_E: return "E";
	case KEY_F: return "F";
	case KEY_G: return "G";
	case KEY_H: return "H";
	case KEY_I: return "I";
	case KEY_J: return "J";
	case KEY_K: return "K";
	case KEY_L: return "L";
	case KEY_M: return "M";
	case KEY_N: return "N";
	case KEY_O: return "O";
	case KEY_P: return "P";
	case KEY_Q: return "Q";
	case KEY_R: return "R";
	case KEY_S: return "S";
	case KEY_T: return "T";
	case KEY_U: return "U";
	case KEY_V: return "V";
	case KEY_W: return "W";
	case KEY_X: return "X";
	case KEY_Y: return "Y";
	case KEY_Z: return "Z";
	default:return "Unknown";
	}
}
