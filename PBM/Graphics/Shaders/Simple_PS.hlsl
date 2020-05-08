#include "Simple.hlsli"

float4 PSMain(VS_OUTPUT input) : SV_Target
{
	float3 toEye = normalize(g_EyePosW.xyz -  input.PosW);
	float3 normal = normalize(input.Normal);
	float4 ambient = float4(0.0f,0.0f,0.0f,0.0f), diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f), specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 a, d, s;
	ComputeDirectionalLight(
		g_Material, g_DirLight, normal, toEye,
		a, d, s);
	ambient += a;
	diffuse += d;
	specular += s;

	float4 litColor = input.Color * (ambient + diffuse) + specular;
	litColor.a = g_Material.Diffuse.a * input.Color.a;
	return litColor;
}
