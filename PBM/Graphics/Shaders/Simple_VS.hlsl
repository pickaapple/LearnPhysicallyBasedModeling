#include "Simple.hlsli"

struct VS_IN
{
	float3 Pos : POSITION;
	float4 Color : COLOR;
	float3 Normal : NORMAL;
};

VS_OUTPUT VSMain(VS_IN vIn)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	float4 posW = mul(float4(vIn.Pos, 1.0), g_World);

	matrix viewProj = mul(g_View, g_Projection);
	output.Pos = mul(posW, viewProj);

	output.Color = vIn.Color;
	output.PosW = posW.xyz;
	output.Normal = mul(vIn.Normal, (float3x3)g_WorldInvTranspose);
	return output;
}