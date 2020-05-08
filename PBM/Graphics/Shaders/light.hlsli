
struct DirectionalLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float4 Direction;
};

struct Material
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float4 Reflect;
};

void ComputeDirectionalLight(
	Material mat, DirectionalLight l,
	float3 normal, float3 toEye,
	out float4 ambient,
	out float4 diffuse,
	out float4 specular) 
{
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	float3 lightVec = l.Direction.xyz;

	ambient = l.Ambient * mat.Ambient;
	float diffuseFactor = -dot(lightVec, normal);
	[flatten]
	if (diffuseFactor > 0.0f) 
	{
		float3 v = reflect(lightVec, -normal);
		float specFactor = pow(max(dot(v, toEye), 0.0f), mat.Specular.w);
		diffuse = diffuseFactor * mat.Diffuse * l.Diffuse;
		specular = specFactor * mat.Specular * l.Specular;
	}
}
