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
	ID3D11DeviceContext* m_pDefferedContexts[GRAPHIC_THREAD_COUNT] = { nullptr }; //일단 ImmediateContext에 바로 올릴것, 동기화 타이밍이 심상치 않음
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
	
	//토론의제: Shader를위한 Map을 쓸까말까 -> 쓰면 살짝 느려지지만 코드가 용이해지고, 안 쓰면 성능상의 이점이 있음

	ID3D11Buffer* m_pVertexBuffer = nullptr;
	ID3D11Buffer* m_pIndexBuffer = nullptr;
	ID3D11Buffer* m_pConstantBuffer = nullptr;
	//토론의제: UploadBuffer 클래스를 만들까말까

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

