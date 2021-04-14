#include "Core/Core.h"
#include "Graphics.h"
#include "Buffer.h"
#include "Shader.h"
#include "RendererCommand.h"

#include <d3dcompiler.h>
#include <dxgi.h>

#pragma comment(lib, "DXGI.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

namespace Engine
{
	Graphics::Graphics()
	{
		D3D11CreateDevice(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr, 0, nullptr, 0,
			D3D11_SDK_VERSION,
			&pDevice,
			nullptr,
			&pContext
		);

		IDXGIDevice* dxgiDevice = 0;
		pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);

		IDXGIAdapter* dxgiAdapter = 0;
		dxgiDevice->GetAdapter(&dxgiAdapter);

		dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&pDXGIFactory);
		
	}

	void Graphics::CreateSwapChainAndRenderTarget(HWND hWnd, wrl::ComPtr<IDXGISwapChain>& swap, wrl::ComPtr<ID3D11RenderTargetView>& target)
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

		pDXGIFactory->CreateSwapChain((IUnknown*)pDevice.Get(), &sd, &swap);

		wrl::ComPtr<ID3D11Resource> pBackBuffer;
		swap->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer);

		pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &target);

	}

	void Graphics::ResizeSwapChainAndRenderTarget(wrl::ComPtr<IDXGISwapChain>& swap, wrl::ComPtr<ID3D11RenderTargetView>& target, uint32_t width, uint32_t height)
	{
		DXGI_MODE_DESC md = { 0 };
		md.Width = width;
		md.Height = height;
		md.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // format to store the data
		md.RefreshRate.Numerator = 0;
		md.RefreshRate.Denominator = 0;
		md.Scaling = DXGI_MODE_SCALING_UNSPECIFIED; // unspecified no scalling requierd 
		md.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; // unspecified 

		// unbind the render target
		pContext->OMSetRenderTargets(0, 0, 0);
		// release refrense to the render target
		target->Release();

		// flush the context
		pContext->Flush();

		// resize the target and swapchain buffers buffres
		swap->ResizeTarget(&md);
		swap->ResizeBuffers(0, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

		// recreate the render target
		wrl::ComPtr<ID3D11Resource> pBackBuffer;
		swap->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer);

		pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, target.GetAddressOf());
	}

	void Graphics::DrawTestTriangle()
	{

		struct Vertex {
			float  x, y, z;
		};

		const Vertex vertices[] = {
			{ 0.0f,  0.5f, 0.0f},
			{ 0.5f, -0.5f, 0.0f},
			{-0.5f, -0.5f, 0.0f}
		};

		const uint32_t indices[] = {
			0, 1, 2
		};

		BufferLayout layout = {
			{ShaderDataType::Float3, "Position"}
		};

		VertexBuffer vb(vertices, sizeof(vertices));
		vb.SetLayout(layout);
		vb.Bind();

		//D3D11_BUFFER_DESC vbufferDesc = { 0 };
		//vbufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		//vbufferDesc.Usage = D3D11_USAGE_DEFAULT;
		//vbufferDesc.CPUAccessFlags = 0u;
		//vbufferDesc.MiscFlags = 0u;
		//vbufferDesc.ByteWidth = sizeof(vertices);
		//vbufferDesc.StructureByteStride = sizeof(Vertex);

		//D3D11_SUBRESOURCE_DATA vbufferData = { 0 };
		//vbufferData.pSysMem = vertices;

		//wrl::ComPtr<ID3D11Buffer> pVertexBuffer;
		//pDevice->CreateBuffer(&vbufferDesc, &vbufferData, &pVertexBuffer);

		//const UINT stride = sizeof(Vertex);
		//const UINT offset = 0u;
		//pContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);

		IndexBuffer ib(indices, 3);
		ib.Bind();

		//D3D11_BUFFER_DESC ibufferDesc = { 0 };
		//ibufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		//ibufferDesc.Usage = D3D11_USAGE_DEFAULT;
		//ibufferDesc.CPUAccessFlags = 0u;
		//ibufferDesc.MiscFlags = 0u;
		//ibufferDesc.ByteWidth = sizeof(indices);
		//ibufferDesc.StructureByteStride = sizeof(uint32_t);

		//D3D11_SUBRESOURCE_DATA ibufferData = { 0 };
		//ibufferData.pSysMem = indices;

		//wrl::ComPtr<ID3D11Buffer> pIndexBuffer;
		//pDevice->CreateBuffer(&ibufferDesc, &ibufferData, &pIndexBuffer);
		//pContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

		Shader shader = Shader();

		shader.LoadVertexShader(L"VertexShader.cso");
		shader.LoadPixleShader(L"PixelShader.cso");
		shader.SetInputLayout(layout);
		//shader.GenInputLayoutFromReflection();

		shader.Bind();


		/*wrl::ComPtr<ID3DBlob> pBlob;
		wrl::ComPtr<ID3D11PixelShader> pPixelShader;
		D3DReadFileToBlob(L"ShaderBin/PixelShader.cso", &pBlob);
		pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);

		pContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);

		wrl::ComPtr<ID3D11VertexShader> pVertexShader;
		D3DReadFileToBlob(L"ShaderBin/VertexShader.cso", &pBlob);
		pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader);

		wrl::ComPtr<ID3D11InputLayout> pInputLayout;
		const D3D11_INPUT_ELEMENT_DESC ied[] = {
			{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		pDevice->CreateInputLayout(ied, 1u, pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &pInputLayout);

		pContext->IASetInputLayout(pInputLayout.Get());

		pContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);*/

		/*D3D11_VIEWPORT vp = { 0 };
		vp.Width = 1080;
		vp.Height = 720;
		vp.MinDepth = 0;
		vp.MaxDepth = 1;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		pContext->RSSetViewports(1u, &vp);*/

		pContext->DrawIndexed(3u, 0u, 0u);
	}

}