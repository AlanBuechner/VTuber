#include "Platform/Windows/Win.h"
#include "Buffer.h"
#include "Graphics.h"
#include "RendererCommand.h"

namespace Engine
{
	// ------------------------------------- Vertex Buffer ------------------------------------- //

#pragma region vertex buffer


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
	
	VertexBuffer::VertexBuffer(const void* vertices, const uint32_t size)
	{
		Graphics& graphics = RendererCommand::GetGraphics();

		D3D11_BUFFER_DESC vbufferDesc = { 0 };
		vbufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbufferDesc.Usage = (vertices==nullptr ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT);
		vbufferDesc.CPUAccessFlags = 0u;
		vbufferDesc.MiscFlags = 0u;
		vbufferDesc.ByteWidth = size;

		D3D11_SUBRESOURCE_DATA vbufferData = { 0 };
		vbufferData.pSysMem = vertices;

		graphics.GetDivice()->CreateBuffer(&vbufferDesc, &vbufferData, &m_Buffer);
	}

	void VertexBuffer::Bind() const
	{
		Graphics& graphics = RendererCommand::GetGraphics();

		const UINT stride = m_Layout.GetStride();
		const UINT offset = 0u;
		graphics.GetContext()->IASetVertexBuffers(0u, 1u, m_Buffer.GetAddressOf(), &stride, &offset);
	}

	void VertexBuffer::Unbind() const
	{
		Graphics& graphics = RendererCommand::GetGraphics();
		graphics.GetContext()->IASetVertexBuffers(0u, 1u, nullptr, nullptr, nullptr);
	}

	void VertexBuffer::SetLayout(const BufferLayout& layout)
	{
		const std::vector<BufferElement>& elements = layout.GetElements();
		for (uint32_t i = 0; i < elements.size(); i++)
			BufferElement element = elements[i];

		m_Layout = layout;
	}

	void VertexBuffer::SetData(const void* data, const uint32_t size)
	{
		Graphics& graphics = RendererCommand::GetGraphics();

		//graphics.GetDivice()->
	}

	Ref<VertexBuffer> VertexBuffer::Create(const uint32_t size)
	{
		return std::make_shared<VertexBuffer>(nullptr, size);
	}

	Ref<VertexBuffer> VertexBuffer::Create(const void* vertices, const uint32_t size)
	{
		return std::make_shared<VertexBuffer>(vertices, size);
	}

#pragma endregion

	// ------------------------------------- End Vertex Buffer ------------------------------------- //

	// ------------------------------------- Index Buffer ------------------------------------- //

#pragma region index buffer

	IndexBuffer::IndexBuffer(const uint32_t* indices, const uint32_t count)
	{
		Graphics& graphics = RendererCommand::GetGraphics();

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

	void IndexBuffer::Bind() const
	{
		Graphics& graphics = RendererCommand::GetGraphics();
		graphics.GetContext()->IASetIndexBuffer(m_Buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	}

	void IndexBuffer::Unbind() const
	{
		Graphics& graphics = RendererCommand::GetGraphics();
		graphics.GetContext()->IASetIndexBuffer(nullptr, DXGI_FORMAT_R32_UINT, 0);
	}

	Ref<IndexBuffer> IndexBuffer::Create(const uint32_t count)
	{
		return std::make_shared<IndexBuffer>(nullptr, count);
	}

	Ref<IndexBuffer> IndexBuffer::Create(const uint32_t* indices, const uint32_t count)
	{
		return std::make_shared<IndexBuffer>(indices, count);
	}

#pragma endregion

	// ------------------------------------- End Index Buffer ------------------------------------- //



}