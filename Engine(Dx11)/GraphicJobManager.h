#pragma once
#include "framework.h"
#define GRAPHIC_THREAD_COUNT 4

using namespace DirectX;
//�������ؽ�Ʈ ���ÿ� ���Ŀ� ����(����� �� ImmediateContext���� �ø��� -> CommandList���� ���)
class GraphicJobManager
{
	ID3D11DeviceContext* m_pImmediateContext;
	ID3D11DeviceContext* m_pDefferedContexts[GRAPHIC_THREAD_COUNT];

};

