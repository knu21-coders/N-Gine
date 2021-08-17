#pragma once
#include "framework.h"
class DXFamily
{
private:
	D3D_DRIVER_TYPE                     g_driverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL                   g_featureLevel = D3D_FEATURE_LEVEL_11_0;
	ID3D11Device* m_pd3dDevice = nullptr;
	ID3D11Device1* g_pd3dDevice1 = nullptr;
	ID3D11DeviceContext* g_pImmediateContext = nullptr;
	ID3D11DeviceContext1* g_pImmediateContext1 = nullptr;
	IDXGISwapChain* g_pSwapChain = nullptr;
	IDXGISwapChain1* g_pSwapChain1 = nullptr;
	ID3D11RenderTargetView* g_pRenderTargetView = nullptr;


	//Client Specs
	HWND g_hwnd;
	UINT _WIDTH;
	UINT _HEIGHT;

	//Methods
	HRESULT DXFamily::setupRenderTarget();

public:
	HRESULT InitDevice(HWND g_hwnd);
	HRESULT CreateSwapChain();
	DXFamily() {

	}
	~DXFamily() {

	}

};
extern DXFamily *g_pDXFamily = nullptr;

