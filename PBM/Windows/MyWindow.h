#pragma once
#include "Keyboard.h"
#include "Mouse.h"
#define MAX_LOADSTRING 100

static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
static INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
struct WindowInfo {
	bool Active;
	bool Moving;
};
class MyWindow
{
protected:
	ATOM MyRegisterClass();
public:
	BOOL InitInstance(int nCmdShow);
	int Run();
	MyWindow(HINSTANCE hInstance);
	~MyWindow();
	void Active(bool active);
	inline bool IsActive() const;
	void Moving(const RECT& rect);
	void Moved(int screen_x, int screen_y);
public:
	HINSTANCE hInstance;  // 当前实例
	Keyboard m_Keyboard;
	Mouse m_Mouse;
private:
	WCHAR title[MAX_LOADSTRING], windowClass[MAX_LOADSTRING];  // 标题栏文本 主窗口类名
	double width, height;
	WindowInfo m_Info;
};

inline bool MyWindow::IsActive() const
{
	return m_Info.Active;
}

static MyWindow *applicationHandle;
