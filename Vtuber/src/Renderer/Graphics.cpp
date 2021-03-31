#include "Graphics.h"

#pragma comment(lib, "d3d11.lib")

namespace Engine
{
	Graphics::Graphics(HWND hWnd)
	{
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
		sd.OutputWindow = hWnd; // window to output to
		sd.Windowed = TRUE;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		sd.Flags = 0;

		D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr, 0, nullptr, 0,
			D3D11_SDK_VERSION,
			&sd,
			&pSwap,
			&pDevice,
			nullptr,
			&pContext
		);

		ID3D11Resource* pBackBuffer = nullptr;
		pSwap->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&pBackBuffer));

		pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &pTarget);

		pBackBuffer->Release();
	}

	Graphics::~Graphics()
	{
		if (pDevice != nullptr)
			pDevice->Release();

		if (pSwap != nullptr)
			pSwap->Release();

		if (pContext != nullptr)
			pContext->Release();

		if (pTarget != nullptr)
			pTarget->Release();
	}

	void Graphics::SqapBuffers()
	{
		pSwap->Present(1u, 0u);
	}

	void Graphics::ClearBuffer(float r, float g, float b)
	{
		const float color[] = { r,g,b,1.0f };
		pContext->ClearRenderTargetView(pTarget, color);
	}

}