#include "DirectX11RendererAPI.h"

#include "Renderer/Mesh.h"
#include "Renderer/Buffer.h"
#include "Renderer/Shader.h"


namespace Engine
{
	void DirectX11RendererAPI::Init()
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

		pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	}

	void DirectX11RendererAPI::SetViewPort(int width, int height, int x, int y)
	{
		D3D11_VIEWPORT vp = { 0 };
		vp.Width = (float)width;
		vp.Height = (float)height;
		vp.MinDepth = 0;
		vp.MaxDepth = 1;
		vp.TopLeftX = (float)x;
		vp.TopLeftY = (float)y;
		pContext->RSSetViewports(1u, &vp);
	}

	void DirectX11RendererAPI::DrawIndexed(uint32_t count)
	{
		pContext->DrawIndexed(count, 0u, 0u);
	}

	void DirectX11RendererAPI::DrawTestTriangle()
	{

		struct Vertex {
			float  x, y, z, w;
		};

		const Vertex vertices[] = {
			{ 0.0f,  0.5f, 0.0f, 1.0f},
			{ 0.5f, -0.5f, 0.0f, 1.0f},
			{-0.5f, -0.5f, 0.0f, 1.0f}
		};

		const uint32_t indices[] = {
			0, 1, 2
		};

		BufferLayout layout = {
			{ShaderDataType::Float4, "Position"}
		};

		Ref<VertexBuffer> vb = VertexBuffer::Create(vertices, sizeof(vertices));
		vb->SetLayout(layout);
		vb->Bind();

		Ref<IndexBuffer> ib = IndexBuffer::Create(indices, 3);
		ib->Bind();

		Engine::ShaderSource src;
		src.VetexShader = L"VertexShader.vertex.cso";
		src.PixelShader = L"PixelShader.pixel.cso";

		Ref<Shader> shader = Engine::Shader::Create(src);

		shader->Bind();

		DrawIndexed(ib->GetCount());
	}

}