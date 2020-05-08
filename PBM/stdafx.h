// stdafx.h: 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 项目特定的包含文件
//

#pragma once
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "Mincore.lib")

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
// Windows 头文件
#include <windows.h>
#include <windowsx.h>

// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// 在此处引用程序需要的其他标头
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <utility>
#include <exception>
#include <numeric>
#include <tuple>
#include <algorithm>

using namespace std;

#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
using namespace DirectX;

#include "./Common/utility.h"
#define PI 3.1415926
#define ToRadian(degree) ((degree) * (PI / 180.0))
#define ToDegree(radian) ((radian) * (180.0 / PI))
#define Sign(number) ((number) > 0?1:-1)

#include "./Math/Vector.h"
#include "./Math/Matrix.h"
#include "./Common/Debug.h"

struct RenderState
{
	Matrix& pParentModelMatrix;
};

inline POINT PointSubstract(const POINT& a, const POINT& b)
{
	return POINT{ a.x - b.x, a.y - b.y };
}

enum KEY_STATE {
	KEY_STATE_NONE = 0,
	KEY_STATE_DOWN,  //按下
	KEY_STATE_PRESS,  //长按
	KEY_STATE_UP,         //释放
	KEY_STATE_NUM
};

typedef unsigned char KeyCode;

//回调接口
class CallKeyStateChange
{
public:
	virtual void KeyboardStateChange(KeyCode code, KEY_STATE state) = 0;
};

class CallUserController {
public:
	virtual void Turn(int sign, double speed) = 0;
	virtual void Move(double speed) = 0;
};

template<UINT TNameLength>
inline void D3D11SetDebugObjectName(_In_ ID3D11DeviceChild* resource, _In_ const char(&name)[TNameLength])
{
#if defined(DEBUG) || defined(_DEBUG)
	resource->SetPrivateData(WKPDID_D3DDebugObjectName, TNameLength - 1, name);
#else
	UNREFERENCED_PARAMETER(resource);
	UNREFERENCED_PARAMETER(name);
#endif
}

template<UINT TNameLength>
inline void DXGISetDebugObjectName(_In_ IDXGIObject* resource, _In_ const char(&name)[TNameLength])
{
#if defined(DEBUG) || defined(_DEBUG)
	resource->SetPrivateData(WKPDID_D3DDebugObjectName, TNameLength - 1, name);
#else
	UNREFERENCED_PARAMETER(resource);
	UNREFERENCED_PARAMETER(name);
#endif
}
