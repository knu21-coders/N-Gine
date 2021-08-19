#include "DXFamily.h"
#include "Win32_.h"

/*
    Referenced Tutorial07.cpp
    - https://github.com/walbourn/directx-sdk-samples

    본 소스코드는 공부용도로 위 소스코드가 참고 됨

*/

HRESULT DXFamily::InitDevice(HWND _hwnd) {
    this->g_hwnd = _hwnd;

	HRESULT hr = S_OK;
	
	RECT rc;
	GetClientRect(g_hWnd, &rc);
	_WIDTH = rc.right - rc.left;
	_HEIGHT = rc.bottom - rc.top;

	UINT createDeviceFlags = 0;
	#ifdef _DEBUG
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	#endif

		
    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
    };
    UINT numDriverTypes = ARRAYSIZE( driverTypes );

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };
    UINT numFeatureLevels = ARRAYSIZE(featureLevels);
 //   g_driverType = driverTypes[];

    for (int deviceNum = 0; deviceNum < numDriverTypes; deviceNum++) {
        
        D3D_DRIVER_TYPE g_driverType = driverTypes[deviceNum];

        hr = D3D11CreateDevice( nullptr, g_driverType, nullptr, 
            createDeviceFlags, featureLevels, numFeatureLevels, 
            D3D11_SDK_VERSION, &m_pd3dDevice, &g_featureLevel, 
            &g_pImmediateContext );
             
        if (hr == E_INVALIDARG) {

            //DirectX 11
          
              hr = D3D11CreateDevice( nullptr, g_driverType, nullptr, createDeviceFlags, &featureLevels[1], numFeatureLevels - 1,
                                    D3D11_SDK_VERSION, &m_pd3dDevice, &g_featureLevel, &g_pImmediateContext );
    
        }
        
        //순차적 디바이스로 방식으로 로드후 성공시 break;
        if( SUCCEEDED( hr ) )
            break;

    }

    if( FAILED( hr ))
    return hr;


    
   

}

HRESULT DXFamily::CreateSwapChain(){
   
    HRESULT hr;
     // Obtain DXGI factory from device (since we used nullptr for pAdapter above)
    IDXGIFactory1* dxgiFactory = nullptr;
    {
        IDXGIDevice* dxgiDevice = nullptr;
        hr = m_pd3dDevice->QueryInterface( __uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice) );
        if (SUCCEEDED(hr))
        {
            IDXGIAdapter* adapter = nullptr;
            hr = dxgiDevice->GetAdapter(&adapter);
            if (SUCCEEDED(hr))
            {
                hr = adapter->GetParent( __uuidof(IDXGIFactory1), reinterpret_cast<void**>(&dxgiFactory) );
                adapter->Release();
            }
            dxgiDevice->Release();
        }
    }


    //DESC INIT
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 1;
    sd.BufferDesc.Width = _WIDTH;
    sd.BufferDesc.Height = _HEIGHT;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = g_hwnd;
    sd.SampleDesc.Count=1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed=TRUE;


    hr = dxgiFactory->CreateSwapChain( m_pd3dDevice, &sd, &g_pSwapChain );

    dxgiFactory->MakeWindowAssociation(g_hWnd, DXGI_MWA_NO_ALT_ENTER);
    dxgiFactory->Release();

    if (FAILED(hr))
        return hr;
    
    return setupRenderTarget();

}

HRESULT DXFamily::setupRenderTarget(){
    
    HRESULT hr;
    // Create a render target view
    ID3D11Texture2D* pBackBuffer = nullptr;
    hr = g_pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), reinterpret_cast<void**>( &pBackBuffer ) );
    if( FAILED( hr ) )
        return hr;

    hr = m_pd3dDevice->CreateRenderTargetView( pBackBuffer, nullptr, &g_pRenderTargetView );
    pBackBuffer->Release();
    if( FAILED( hr ) )
        return hr;

}