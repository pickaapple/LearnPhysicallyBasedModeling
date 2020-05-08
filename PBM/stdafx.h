// stdafx.h: ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// ��Ŀ�ض��İ����ļ�
//

#pragma once
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "Mincore.lib")

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ�ļ����ų�����ʹ�õ�����
// Windows ͷ�ļ�
#include <windows.h>
#include <windowsx.h>

// C ����ʱͷ�ļ�
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// �ڴ˴����ó�����Ҫ��������ͷ
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
	KEY_STATE_DOWN,  //����
	KEY_STATE_PRESS,  //����
	KEY_STATE_UP,         //�ͷ�
	KEY_STATE_NUM
};

typedef unsigned char KeyCode;

//�ص��ӿ�
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
