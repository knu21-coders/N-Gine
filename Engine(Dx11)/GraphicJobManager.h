#pragma once
#include "framework.h"
#define GRAPHIC_THREAD_COUNT 4

using namespace DirectX;
//지연컨텍스트 사용시에 추후에 변경(현재는 걍 ImmediateContext에만 올릴것 -> CommandList까지 허용)
class GraphicJobManager
{
	ID3D11DeviceContext* m_pImmediateContext;
	ID3D11DeviceContext* m_pDefferedContexts[GRAPHIC_THREAD_COUNT];

};

