#pragma once

#include "Core/Win.h"
#include <d3d11.h>
#include <wrl.h>

namespace Engine
{
	namespace wrl = Microsoft::WRL;

	class Graphics
	{

	public:
		Graphics(HWND hWnd);
		Graphics(const Graphics&) = delete;
		Graphics& operator=(const Graphics&) = delete;
		~Graphics() = default;

		void SqapBuffers();

		void ClearBuffer(float r, float g, float b);

		void DrawTestTriangle();

	private:
		wrl::ComPtr<ID3D11Device> pDevice;
		wrl::ComPtr<IDXGISwapChain> pSwap;
		wrl::ComPtr<ID3D11DeviceContext> pContext;
		wrl::ComPtr<ID3D11RenderTargetView> pTarget;
	};
}