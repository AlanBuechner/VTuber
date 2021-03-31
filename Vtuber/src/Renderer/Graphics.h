#pragma once

#include "Core/Win.h"
#include <d3d11.h>

namespace Engine
{
	class Graphics
	{
	public:
		Graphics(HWND hWnd);
		Graphics(const Graphics&) = delete;
		Graphics& operator=(const Graphics&) = delete;
		~Graphics();

		void SqapBuffers();

		void ClearBuffer(float r, float g, float b);

	private:
		ID3D11Device* pDevice = nullptr;
		IDXGISwapChain* pSwap = nullptr;
		ID3D11DeviceContext* pContext = nullptr;
		ID3D11RenderTargetView* pTarget = nullptr;
	};
}