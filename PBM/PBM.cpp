// PBM.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "PBM.h"
#include "./Windows/MyWindow.h"

static void CallConcoleOutput(void)
{
	AllocConsole();
	FILE* stream;
	freopen_s(&stream, "CON", "r", stdin);//重定向输入流
	freopen_s(&stream, "CON", "w", stdout);//重定向输入流
	cout << "Init Console Output" << endl;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	CallConcoleOutput();
	MyWindow window(hInstance);
	int ret = -1;
	if (window.InitInstance(nCmdShow)) {
		//运行
		ret = window.Run();
	}
	else {
		cout << "windows 初始化失败" << endl;
		system("pause");
	}
	FreeConsole();
	return ret;
}