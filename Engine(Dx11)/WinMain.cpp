// Engine(Dx11).cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Engine(Dx11).h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // ���� string �ʱ�ȭ
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_ENGINEDX11, szWindowClass, MAX_LOADSTRING);
    
    // OS ���� �ʱ�ȭ
    if (!InitWindow(hInstance, nCmdShow))
    {
        return FALSE;
    }
    g_pDXFamily = new DXFamily();
    if (FAILED(g_pDXFamily->InitDevice(g_hWnd)))
    {
        //CleanupDevice();
        return FALSE;
    }
    
    //Create SwapChain;
    if (FAILED(g_pDXFamily->CreateSwapChain()))
    {
       
        return FALSE;
    }

    MSG msg = { 0 };

    //���� ����
    while (WM_QUIT != msg.message)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            //Render();
        }
    }
    delete g_pDXFamily;
    return (int) msg.wParam;
}
