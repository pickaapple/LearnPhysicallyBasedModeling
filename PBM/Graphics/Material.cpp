#include "Material.h"

Material::Material(Shader* shader) :
	m_Shader(shader),
	m_MaterialData(
		{
			{0.7,0.7,0.7,1},  //������
			{0.2,0.2,0.2,1},  //ɢ���
			{0.0,0.0,0.0,1}, //���淴��
			{0,0,0,3}
		})
{
}