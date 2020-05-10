#pragma once
class RenderSetting
{
public:
	static void InitSetting(ID3D11Device* device, ID3D11DeviceContext* context);
	static ID3D11RasterizerState* RSNoCull; // 无背面剔除
	static ID3D11RasterizerState* RSCullCW; // 顺时针剔除
	static ID3D11RasterizerState* RSWireframe; // 线框模式

	static ID3D11SamplerState* SSLinearWrap; // 线性过滤

	static ID3D11BlendState* BSNoColorWrite;		                // 不写入颜色

	static ID3D11DepthStencilState* DSSLess;		        // 允许绘制小于深度值的像素
	static ID3D11DepthStencilState* DSSLessEqual;	    // 允许绘制小于等于深度值的像素
	static ID3D11DepthStencilState* DSSWriteStencil;	    // 写入模板值
};

