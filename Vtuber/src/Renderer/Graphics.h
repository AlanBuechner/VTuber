#pragma once

#include "Platform/Windows/Win.h"
#include "Core/Core.h"

namespace Engine
{

	class Graphics
	{

	public:
		Graphics();
		Graphics(const Graphics&) = delete;
		Graphics& operator=(const Graphics&) = delete;
		~Graphics() = default;

		void CreateSwapChainAndRenderTarget(HWND hWnd, wrl::ComPtr<IDXGISwapChain>& swap, wrl::ComPtr<ID3D11RenderTargetView>& target);
		void ResizeSwapChainAndRenderTarget(wrl::ComPtr<IDXGISwapChain>& swap, wrl::ComPtr<ID3D11RenderTargetView>& target, uint32_t width, uint32_t height);

		void DrawTestTriangle();

		inline wrl::ComPtr<ID3D11Device> GetDivice() { return pDevice; }
		inline wrl::ComPtr<ID3D11DeviceContext> GetContext() { return pContext; }

	private:
		wrl::ComPtr<ID3D11Device> pDevice;
		wrl::ComPtr<ID3D11DeviceContext> pContext;
		wrl::ComPtr<IDXGIFactory> pDXGIFactory;
	};
}