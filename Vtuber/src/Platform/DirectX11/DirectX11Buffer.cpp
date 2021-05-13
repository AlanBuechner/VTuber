#include "DirectX11Buffer.h"
#include "Renderer/RendererAPI.h"
#include "DirectX11RendererAPI.h"

namespace Engine
{

	DXGI_FORMAT ShaderDataTypeToDirectXFormat(ShaderDataType type)
	{
		switch (type)
		{
		case Engine::ShaderDataType::None:		return DXGI_FORMAT_UNKNOWN;
		case Engine::ShaderDataType::Float:		return DXGI_FORMAT_R32_FLOAT;
		case Engine::ShaderDataType::Float2:	return DXGI_FORMAT_R32G32_FLOAT;
		case Engine::ShaderDataType::Float3:	return DXGI_FORMAT_R32G32B32_FLOAT;
		case Engine::ShaderDataType::Float4:	return DXGI_FORMAT_R32G32B32A32_FLOAT;
		case Engine::ShaderDataType::Int:		return DXGI_FORMAT_R32_SINT;
		case Engine::ShaderDataType::Int2:		return DXGI_FORMAT_R32G32_SINT;
		case Engine::ShaderDataType::Int3:		return DXGI_FORMAT_R32G32B32_SINT;
		case Engine::ShaderDataType::Int4:		return DXGI_FORMAT_R32G32B32A32_SINT;
		case Engine::ShaderDataType::Mat3:
			break;
		case Engine::ShaderDataType::Mat4:
			break;
		case Engine::ShaderDataType::Bool:
			break;
		}
		return DXGI_FORMAT_UNKNOWN;
	}

	DirectX11VertexBuffer::DirectX11VertexBuffer(const void* vertices, uint32_t size)
	{
		DirectX11RendererAPI& graphics = *(DirectX11RendererAPI*)RendererAPI::Get();
		D3D11_BUFFER_DESC vbufferDesc = { 0 };
		vbufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbufferDesc.Usage = (vertices == nullptr ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT);
		vbufferDesc.CPUAccessFlags = 0u;
		vbufferDesc.MiscFlags = 0u;
		vbufferDesc.ByteWidth = size;

		D3D11_SUBRESOURCE_DATA vbufferData = { 0 };
		vbufferData.pSysMem = vertices;

		graphics.GetDivice()->CreateBuffer(&vbufferDesc, &vbufferData, &m_Buffer);
	}

	void DirectX11VertexBuffer::Bind() const
	{
		DirectX11RendererAPI& graphics = *(DirectX11RendererAPI*)RendererAPI::Get();

		const UINT stride = m_Layout.GetStride();
		const UINT offset = 0u;
		graphics.GetContext()->IASetVertexBuffers(0u, 1u, m_Buffer.GetAddressOf(), &stride, &offset);
	}

	void DirectX11VertexBuffer::Unbind() const
	{
		DirectX11RendererAPI& graphics = *(DirectX11RendererAPI*)RendererAPI::Get();
		graphics.GetContext()->IASetVertexBuffers(0u, 1u, nullptr, nullptr, nullptr);
	}

	void DirectX11VertexBuffer::SetLayout(const BufferLayout& layout)
	{
		const std::vector<BufferElement>& elements = layout.GetElements();
		for (uint32_t i = 0; i < elements.size(); i++)
			BufferElement element = elements[i];

		m_Layout = layout;
	}

	void DirectX11VertexBuffer::SetData(const void* data, const uint32_t size)
	{
		DirectX11RendererAPI& graphics = *(DirectX11RendererAPI*)RendererAPI::Get();

		// TODO
	}


	// -------------------------------- Index Buffer -------------------------------- // 
	DirectX11IndexBuffer::DirectX11IndexBuffer(const uint32_t* indices, const uint32_t count)
	{
		DirectX11RendererAPI& graphics = *(DirectX11RendererAPI*)RendererAPI::Get();
		m_Count = count;

		D3D11_BUFFER_DESC ibufferDesc = { 0 };
		ibufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibufferDesc.Usage = D3D11_USAGE_DEFAULT;
		ibufferDesc.CPUAccessFlags = 0u;
		ibufferDesc.MiscFlags = 0u;
		ibufferDesc.ByteWidth = sizeof(uint32_t) * count;
		ibufferDesc.StructureByteStride = sizeof(uint32_t);

		D3D11_SUBRESOURCE_DATA ibufferData = { 0 };
		ibufferData.pSysMem = indices;

		graphics.GetDivice()->CreateBuffer(&ibufferDesc, &ibufferData, &m_Buffer);
	}

	void DirectX11IndexBuffer::Bind() const
	{
		DirectX11RendererAPI& graphics = *(DirectX11RendererAPI*)RendererAPI::Get();
		graphics.GetContext()->IASetIndexBuffer(m_Buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	}

	void DirectX11IndexBuffer::Unbind() const
	{
		DirectX11RendererAPI& graphics = *(DirectX11RendererAPI*)RendererAPI::Get();
		graphics.GetContext()->IASetIndexBuffer(nullptr, DXGI_FORMAT_R32_UINT, 0);
	}

	void DirectX11IndexBuffer::SetData(const uint32_t* indices, uint32_t size)
	{
		DirectX11RendererAPI& graphics = *(DirectX11RendererAPI*)RendererAPI::Get();
		// TODO
	}


	// -------------------------------- Constent Buffer -------------------------------- // 
	DirectX11ConstentBuffer::DirectX11ConstentBuffer(const void* data, uint32_t size)
	{
		DirectX11RendererAPI& graphics = *(DirectX11RendererAPI*)RendererAPI::Get();

		D3D11_BUFFER_DESC cbufferDesc = {  };
		cbufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		cbufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbufferDesc.MiscFlags = 0u;
		cbufferDesc.ByteWidth = size;
		cbufferDesc.StructureByteStride = 0u;
		D3D11_SUBRESOURCE_DATA cbufferData = {  };
		cbufferData.pSysMem = data;
		
		graphics.GetDivice()->CreateBuffer(&cbufferDesc, &cbufferData, &m_Buffer);

	}

	void DirectX11ConstentBuffer::SetData(const void* data, uint32_t size)
	{
	}

}