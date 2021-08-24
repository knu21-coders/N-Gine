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

    // 전역 string 초기화
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_ENGINEDX11, szWindowClass, MAX_LOADSTRING);
    
    // OS 종속 초기화
    if (!InitWindow(hInstance, nCmdShow))
    {
        return FALSE;
    }
    DXFamily _dxFamily;
    g_pDXFamily = &_dxFamily; 
    if (FAILED(g_pDXFamily->Init(g_hWnd)))
    {
        g_pDXFamily->End();
        return FALSE;
    }

    MSG msg = { 0 };

    //메인 루프
    while (WM_QUIT != msg.message)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            g_pDXFamily->Render();
        }
    }

    return (int) msg.wParam;
}
