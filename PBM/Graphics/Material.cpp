#include "Material.h"

Material::Material(Shader* shader) :
	m_Shader(shader),
	m_MaterialData(
		{
			{0.7,0.7,0.7,1},  //环境光
			{0.2,0.2,0.2,1},  //散射光
			{0.0,0.0,0.0,1}, //镜面反射
			{0,0,0,3}
		})
{
}