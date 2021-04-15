#include "DirectX11SwapChain.h"
#include "DirectX11RendererAPI.h"

namespace Engine
{
	DirectX11SwapChain::DirectX11SwapChain()
	{
		
	}

	DirectX11SwapChain::~DirectX11SwapChain()
	{

	}

	void DirectX11SwapChain::Init(void* windowHandle)
	{
		DirectX11RendererAPI& api = *((DirectX11RendererAPI*)RendererAPI::Get());

		DXGI_SWAP_CHAIN_DESC sd = { 0 };
		sd.BufferDesc.Width = 0; // get the width from the window
		sd.BufferDesc.Height = 0; // get the height from the window
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // format to store the data
		sd.BufferDesc.RefreshRate.Numerator = 0;
		sd.BufferDesc.RefreshRate.Denominator = 0;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED; // unspecified no scalling requierd 
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; // unspecified 
		sd.SampleDesc.Count = 1; // no anti aliasing
		sd.SampleDesc.Quality = 0; // no anti aliasing
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1; // double buffering
		sd.OutputWindow = (HWND)windowHandle; // window to output to
		sd.Windowed = TRUE;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		sd.Flags = 0;

		api.GetFactory()->CreateSwapChain((IUnknown*)api.GetDivice().Get(), &sd, &pSwap);

		wrl::ComPtr<ID3D11Resource> pBackBuffer;
		pSwap->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer);

		api.GetDivice()->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &pTarget);
	}

	void DirectX11SwapChain::Resize(uint32_t width, uint32_t height)
	{
		if (pSwap && pTarget) 
		{
			DirectX11RendererAPI& api = *((DirectX11RendererAPI*)RendererAPI::Get());

			DXGI_MODE_DESC md = { 0 };
			md.Width = width;
			md.Height = height;
			md.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // format to store the data
			md.RefreshRate.Numerator = 0;
			md.RefreshRate.Denominator = 0;
			md.Scaling = DXGI_MODE_SCALING_UNSPECIFIED; // unspecified no scalling requierd 
			md.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; // unspecified 

			// unbind the render target
			api.GetContext()->OMSetRenderTargets(0, 0, 0);
			// release refrense to the render target
			pTarget->Release();

			// flush the context
			api.GetContext()->Flush();

			// resize the target and swapchain buffers buffres
			pSwap->ResizeTarget(&md);
			pSwap->ResizeBuffers(0, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

			// recreate the render target
			wrl::ComPtr<ID3D11Resource> pBackBuffer;
			pSwap->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer);

			api.GetDivice()->CreateRenderTargetView(pBackBuffer.Get(), nullptr, pTarget.GetAddressOf());
		}
	}

	void DirectX11SwapChain::ClearColor(float r, float g, float b, float a)
	{
		DirectX11RendererAPI& graphics = *(DirectX11RendererAPI*)RendererAPI::Get();
		const float color[] = { r,g,b,a };
		graphics.GetContext()->ClearRenderTargetView(pTarget.Get(), color);
	}

	void DirectX11SwapChain::SwapBuffers()
	{
		pSwap->Present(1u, 0u);
	}

	void DirectX11SwapChain::Bind()
	{
		((DirectX11RendererAPI*)RendererAPI::Get())->GetContext()->OMSetRenderTargets(1u, pTarget.GetAddressOf(), nullptr);
	}

}