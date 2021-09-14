#pragma once
#include "framework.h"
#define GRAPHIC_THREAD_COUNT 4

class DXFamily
{
private:
	D3D_DRIVER_TYPE                     g_driverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL                   g_featureLevel = D3D_FEATURE_LEVEL_11_0;
	ID3D11Device* m_pd3dDevice = nullptr;
	ID3D11Device1* m_pd3dDevice1 = nullptr;
	ID3D11DeviceContext* m_pImmediateContext = nullptr;
	ID3D11DeviceContext* m_pDefferedContexts[GRAPHIC_THREAD_COUNT] = { nullptr }; //�ϴ� ImmediateContext�� �ٷ� �ø���, ����ȭ Ÿ�̹��� �ɻ�ġ ����
	ID3D11DeviceContext1* m_pImmediateContext1 = nullptr;

	IDXGISwapChain* m_pSwapChain = nullptr;
	IDXGISwapChain1* m_pSwapChain1 = nullptr;

	ID3D11RenderTargetView* m_pRenderTargetView = nullptr;
	ID3D11Texture2D* m_pDepthStencil = nullptr;
	ID3D11DepthStencilView* m_pDepthStencilView = nullptr;

	ID3D11VertexShader* m_pVertexShader = nullptr;
	ID3D11InputLayout* m_pVertexLayout = nullptr;
	ID3D11PixelShader* m_pPixelShader = nullptr;
	ID3D11PixelShader* m_pPixelShaderSolid = nullptr;
	
	//�������: Shader������ Map�� ����� -> ���� ��¦ ���������� �ڵ尡 ����������, �� ���� ���ɻ��� ������ ����

	ID3D11Buffer* m_pVertexBuffer = nullptr;
	ID3D11Buffer* m_pIndexBuffer = nullptr;
	ID3D11Buffer* m_pConstantBuffer = nullptr;
	//�������: UploadBuffer Ŭ������ ������

	//Client Specs
	HWND m_hwnd = NULL;
	UINT _WIDTH = 1920;
	UINT _HEIGHT = 1080;

	//Methods
	HRESULT setupRenderTarget();
	inline HRESULT InitDevice();
	inline HRESULT CreateSwapChain();
	inline HRESULT CreateDepthStencilView();
	inline HRESULT InitViewport();
	inline HRESULT CreateVertexShader();
	inline HRESULT CreatePixelShader();
public:

	DXFamily() {

	}
	~DXFamily() {

	}
	HRESULT Init(const HWND& _hwnd);
	void Render();
	void End();

};
extern DXFamily *g_pDXFamily = nullptr;

