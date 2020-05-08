#include "light.hlsli"

cbuffer VSConstantBuffer : register(b0)
{
	matrix g_World;
	matrix g_View;
	matrix g_Projection;
	matrix g_WorldInvTranspose; // 用于计算法向量
}

cbuffer PSConstantBuffer : register(b1)
{
	DirectionalLight g_DirLight;
	Material g_Material;
	float4 g_EyePosW;
}

struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float3 PosW : POSITION;
	float4 Color : COLOR;
	float3 Normal : NORMAL;
};
