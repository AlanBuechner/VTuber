#pragma once
#include "Platform/Windows/Win.h"
#include "Renderer/SwapChain.h"

namespace Engine
{
	class DirectX11SwapChain : public SwapChain
	{
	public:
		DirectX11SwapChain();
		virtual ~DirectX11SwapChain();

		virtual void Init(void* windowHandle, uint32_t width, uint32_t height) override;

		virtual void Resize(uint32_t width, uint32_t height) override;
		virtual void ClearColor(float r, float g, float b, float a = 1.0f) override;

		virtual void SetVSync(bool val) override { m_VSync = val; };

		virtual void SwapBuffers() override;

		virtual void Bind() override;

	private:

		wrl::ComPtr<IDXGISwapChain> pSwap; // pointer to the swap chain 
		wrl::ComPtr<ID3D11RenderTargetView> pTarget; // pointer to the render target
		wrl::ComPtr<ID3D11DepthStencilView> pDSV;

		bool m_VSync = true;

	};
}