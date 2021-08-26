#include "DXFamily.h"
#include "Win32_.h"

/*
    Referenced Tutorial07.cpp
    - https://github.com/walbourn/directx-sdk-samples

    본 소스코드는 공부용도로 위 소스코드가 참고 됨

*/

inline HRESULT DXFamily::InitDevice() {
   

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
            &m_pImmediateContext );
             
        if (hr == E_INVALIDARG) {

            //DirectX 11
          
              hr = D3D11CreateDevice( nullptr, g_driverType, nullptr, createDeviceFlags, &featureLevels[1], numFeatureLevels - 1,
                                    D3D11_SDK_VERSION, &m_pd3dDevice, &g_featureLevel, &m_pImmediateContext );
    
        }
        
        //순차적 디바이스로 방식으로 로드후 성공시 break;
        if( SUCCEEDED( hr ) )
            break;

    }

    return hr;

}

inline HRESULT DXFamily::CreateSwapChain(){
   
    HRESULT hr = S_OK;
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
    sd.OutputWindow = m_hwnd;
    sd.SampleDesc.Count=1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed=TRUE;


    hr = dxgiFactory->CreateSwapChain( m_pd3dDevice, &sd, &m_pSwapChain );

    dxgiFactory->MakeWindowAssociation(g_hWnd, DXGI_MWA_NO_ALT_ENTER);
    dxgiFactory->Release();

    return hr;

}
  
inline HRESULT DXFamily::CreateDepthStencilView()
{
    HRESULT hr = S_OK;
    // Create depth stencil texture
    D3D11_TEXTURE2D_DESC descDepth = {};
    descDepth.Width = _WIDTH;
    descDepth.Height = _HEIGHT;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDepth.SampleDesc.Count = 1;
    descDepth.SampleDesc.Quality = 0;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    descDepth.CPUAccessFlags = 0;
    descDepth.MiscFlags = 0;
    hr = m_pd3dDevice->CreateTexture2D(&descDepth, nullptr, &m_pDepthStencil);
    if (FAILED(hr))
        return hr;

    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
    descDSV.Format = descDepth.Format;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice = 0;
    hr = m_pd3dDevice->CreateDepthStencilView(m_pDepthStencil, &descDSV, &m_pDepthStencilView);
    if (FAILED(hr))
        return hr;

    m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);
    return hr;
}

inline HRESULT DXFamily::InitViewport()
{
    HRESULT hr = S_OK;
    D3D11_VIEWPORT vp;
    vp.Width = (FLOAT)_WIDTH;
    vp.Height = (FLOAT)_HEIGHT;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    m_pImmediateContext->RSSetViewports(1, &vp);
    return hr;
}

inline HRESULT DXFamily::CompileShader(const WCHAR * szFileName, const D3D_SHADER_MACRO* defines, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob * *ppBlobOut)
{
        HRESULT hr = S_OK;

        DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG

        dwShaderFlags |= D3DCOMPILE_DEBUG;

        // Disable optimizations to further improve shader debugging
        dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

        ID3DBlob* pErrorBlob = nullptr;
        hr = D3DCompileFromFile(szFileName, nullptr, nullptr, szEntryPoint, szShaderModel,
            dwShaderFlags, 0, ppBlobOut, &pErrorBlob);
        if (FAILED(hr))
        {
            if (pErrorBlob)
            {
                OutputDebugStringA(reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()));
                pErrorBlob->Release();
            }
            return hr;
        }
        if (pErrorBlob) pErrorBlob->Release();

        return S_OK;
}

inline HRESULT DXFamily::CreateVertexShader()
{

    HRESULT hr;
    // Compile the vertex shader
    ID3DBlob* pVSBlob = nullptr;
    hr = CompileShader(L"VertexShader.hlsl", nullptr, "VS", "vs_4_0", &pVSBlob);
    if (FAILED(hr))
    {
        MessageBox(nullptr,
            L"The HLSL file cannot be compiled.  Please run this executable from the directory that contains the HLSL file.", L"Error", MB_OK);
        return hr;
    }


    // Create the vertex shader
    hr = m_pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &m_pVertexShader);
    if (FAILED(hr))
    {
        pVSBlob->Release();
        return hr;
    }
    // Define the input layout
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    UINT numElements = ARRAYSIZE(layout);

    // Create the input layout
    hr = m_pd3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
        pVSBlob->GetBufferSize(), &m_pVertexLayout);
    pVSBlob->Release();
    if (FAILED(hr))
        return hr;

    // Set the input layout
    m_pImmediateContext->IASetInputLayout(m_pVertexLayout);

    
    return S_OK;
}

inline HRESULT DXFamily::CreatePixelShader()
{
    HRESULT hr;
    // Compile the pixel shader
    ID3DBlob* pPSBlob = nullptr;
    hr = CompileShader(L"PixelShader.hlsl", nullptr, "PS", "ps_4_0", &pPSBlob);
    if (FAILED(hr))
    {
        MessageBox(nullptr,
            L"The HLSL file(PixelShader) cannot be compiled.  Please run this executable from the directory that contains the HLSL file.", L"Error", MB_OK);
        return hr;
    }

    // Create the pixel shader
    hr = m_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &m_pPixelShaderSolid);
    pPSBlob->Release();
    if (FAILED(hr))
        return hr;
}

HRESULT DXFamily::Init(const HWND& _hwnd)
{
    m_hwnd = _hwnd;
    HRESULT hr = S_OK;
    if (!(hr = InitDevice())) return hr;
    if (!(hr = CreateSwapChain())) return hr;
    if (!(hr = CreateDepthStencilView())) return hr;
    if (!(hr = InitViewport())) return hr;
    if (!(hr = CreateVertexShader())) return hr;
    if (!(hr = CreatePixelShader())) return hr;
    return S_OK;
}

void DXFamily::Render()
{
}

void DXFamily::End()
{

        if (m_pImmediateContext) m_pImmediateContext->ClearState();

        if (m_pConstantBuffer) m_pConstantBuffer->Release();
        if (m_pVertexBuffer) m_pVertexBuffer->Release();
        if (m_pIndexBuffer) m_pIndexBuffer->Release();

        if (m_pVertexLayout) m_pVertexLayout->Release();
        if (m_pVertexShader) m_pVertexShader->Release();
        if (m_pPixelShaderSolid) m_pPixelShaderSolid->Release();
        if (m_pPixelShader) m_pPixelShader->Release();

        if (m_pDepthStencil) m_pDepthStencil->Release();
        if (m_pDepthStencilView) m_pDepthStencilView->Release();
        if (m_pRenderTargetView) m_pRenderTargetView->Release();

        if (m_pSwapChain1) m_pSwapChain1->Release();
        if (m_pSwapChain) m_pSwapChain->Release();

        if (m_pImmediateContext1) m_pImmediateContext1->Release();
        if (m_pImmediateContext) m_pImmediateContext->Release();

        if (m_pd3dDevice1) m_pd3dDevice1->Release();
        if (m_pd3dDevice) m_pd3dDevice->Release();
    
}


inline HRESULT DXFamily::setupRenderTarget(){
    
    HRESULT hr = S_OK;
    // Create a render target view
    ID3D11Texture2D* pBackBuffer = nullptr;
    hr = m_pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), reinterpret_cast<void**>( &pBackBuffer ) );
    if( FAILED( hr ) )
        return hr;

    hr = m_pd3dDevice->CreateRenderTargetView( pBackBuffer, nullptr, &m_pRenderTargetView );
    pBackBuffer->Release();
    
    return hr;

}