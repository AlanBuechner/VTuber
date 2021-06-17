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

	void DirectX11SwapChain::Init(void* windowHandle, uint32_t width, uint32_t height)
	{
		DirectX11RendererAPI& api = *((DirectX11RendererAPI*)RendererAPI::Get());

		DXGI_SWAP_CHAIN_DESC sd = { 0 };
		sd.BufferDesc.Width = width;
		sd.BufferDesc.Height = height; 
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

		CreateDepthBuffer(width, height);

	}

	void DirectX11SwapChain::Resize(uint32_t width, uint32_t height)
	{
		if (pSwap && pTarget && pDSV) 
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

			// resize the target and swapchain buffers
			pSwap->ResizeTarget(&md);
			pSwap->ResizeBuffers(0, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

			// recreate the render target
			wrl::ComPtr<ID3D11Resource> pBackBuffer;
			pSwap->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer);

			api.GetDivice()->CreateRenderTargetView(pBackBuffer.Get(), nullptr, pTarget.GetAddressOf());

			// recreate the depth buffer
			CreateDepthBuffer(width, height);
		}
	}

	void DirectX11SwapChain::ClearColor(float r, float g, float b, float a)
	{
		DirectX11RendererAPI& api = *(DirectX11RendererAPI*)RendererAPI::Get();
		const float color[] = { r,g,b,a };
		api.GetContext()->ClearRenderTargetView(pTarget.Get(), color);
		api.GetContext()->ClearDepthStencilView(pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
	}

	void DirectX11SwapChain::SwapBuffers()
	{
		pSwap->Present((m_VSync ? 1u:0u), 0u);
	}

	void DirectX11SwapChain::Bind()
	{
		DirectX11RendererAPI& api = *((DirectX11RendererAPI*)RendererAPI::Get());

		api.GetContext()->OMSetRenderTargets(1u, pTarget.GetAddressOf(), pDSV.Get());
	}

	void DirectX11SwapChain::CreateDepthBuffer(uint32_t width, uint32_t height)
	{
		DirectX11RendererAPI& api = *((DirectX11RendererAPI*)RendererAPI::Get());

		D3D11_DEPTH_STENCIL_DESC dsDesc = {};
		dsDesc.DepthEnable = TRUE;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

		wrl::ComPtr<ID3D11DepthStencilState> pDSState;
		HRESULT hr = api.GetDivice()->CreateDepthStencilState(&dsDesc, &pDSState);
		if (FAILED(hr)) {
			DBOUT("failed to create depth stencil state" << std::endl);
		}

		api.GetContext()->OMSetDepthStencilState(pDSState.Get(), 1u);

		wrl::ComPtr<ID3D11Texture2D> pDepthStencil;
		D3D11_TEXTURE2D_DESC descDepth = {};
		descDepth.Width = width;
		descDepth.Height = height;
		descDepth.MipLevels = 1u;
		descDepth.ArraySize = 1u;
		descDepth.Format = DXGI_FORMAT_D32_FLOAT;
		descDepth.SampleDesc.Count = 1u;
		descDepth.SampleDesc.Quality = 0u;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;

		api.GetDivice()->CreateTexture2D(&descDepth, nullptr, &pDepthStencil);

		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
		descDSV.Format = DXGI_FORMAT_D32_FLOAT;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0u;

		api.GetDivice()->CreateDepthStencilView(pDepthStencil.Get(), &descDSV, &pDSV);
	}

}