#include "Graphics.h"

#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

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

		wrl::ComPtr<ID3D11Resource> pBackBuffer;
		pSwap->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer);

		pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &pTarget);
	}

	void Graphics::SqapBuffers()
	{
		pSwap->Present(1u, 0u);
	}

	void Graphics::ClearBuffer(float r, float g, float b)
	{
		const float color[] = { r,g,b,1.0f };
		pContext->ClearRenderTargetView(pTarget.Get(), color);
	}

	void Graphics::DrawTestTriangle()
	{

		struct Vertex {
			float  x, y;
		};

		const Vertex vertices[] = {
			{ 0.0f,  0.5f},
			{ 0.5f, -0.5f},
			{-0.5f, -0.5f}
		};

		D3D11_BUFFER_DESC vbufferDesc = { 0 };
		vbufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vbufferDesc.CPUAccessFlags = 0u;
		vbufferDesc.MiscFlags = 0u;
		vbufferDesc.ByteWidth = sizeof(vertices);
		vbufferDesc.StructureByteStride = sizeof(Vertex);

		D3D11_SUBRESOURCE_DATA vbufferData = { 0 };
		vbufferData.pSysMem = vertices;

		wrl::ComPtr<ID3D11Buffer> pVertexBuffer;
		pDevice->CreateBuffer(&vbufferDesc, &vbufferData, &pVertexBuffer);

		const UINT stride = sizeof(Vertex);
		const UINT offset = 0u;
		pContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);


		wrl::ComPtr<ID3DBlob> pBlob;
		wrl::ComPtr<ID3D11PixelShader> pPixelShader;
		D3DReadFileToBlob(L"PixelShader.cso", &pBlob);
		pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);

		pContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);

		wrl::ComPtr<ID3D11VertexShader> pVertexShader;
		D3DReadFileToBlob(L"VertexShader.cso", &pBlob);
		pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader);

		wrl::ComPtr<ID3D11InputLayout> pInputLayout;
		const D3D11_INPUT_ELEMENT_DESC ied[] = {
			{ "Position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		pDevice->CreateInputLayout(ied, 1u, pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &pInputLayout);

		pContext->IASetInputLayout(pInputLayout.Get());

		pContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);

		pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), nullptr);

		pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		D3D11_VIEWPORT vp = { 0 };
		vp.Width = 1080;
		vp.Height = 720;
		vp.MinDepth = 0;
		vp.MaxDepth = 1;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		pContext->RSSetViewports(1u, &vp);

		pContext->Draw(3u, 0u);
	}

}