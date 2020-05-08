#pragma once
class RenderSetting
{
public:
	static void InitSetting(ID3D11Device* device, ID3D11DeviceContext* context);
	static ID3D11RasterizerState* RSNoCull; // �ޱ����޳�
	static ID3D11RasterizerState* RSCullCW; // ˳ʱ���޳�
	static ID3D11RasterizerState* RSWireframe; // �߿�ģʽ

	static ID3D11SamplerState* SSLinearWrap; // ���Թ���

	static ID3D11BlendState* BSNoColorWrite;		                // ��д����ɫ

	static ID3D11DepthStencilState* DSSLess;		        // �������С�����ֵ������
	static ID3D11DepthStencilState* DSSLessEqual;	    // �������С�ڵ������ֵ������
	static ID3D11DepthStencilState* DSSWriteStencil;	    // д��ģ��ֵ
};

