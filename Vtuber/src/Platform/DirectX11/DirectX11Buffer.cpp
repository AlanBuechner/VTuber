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


	// -------------------------------- Vetrtex Buffer -------------------------------- //
	DirectX11VertexBuffer::DirectX11VertexBuffer(const void* vertices, uint32_t size) :
		m_Size(size)
	{
		DirectX11RendererAPI& graphics = *(DirectX11RendererAPI*)RendererAPI::Get();
		D3D11_BUFFER_DESC vbufferDesc = { 0 };
		vbufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbufferDesc.Usage = (vertices == nullptr ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT);
		vbufferDesc.CPUAccessFlags = (vertices == nullptr ? D3D11_CPU_ACCESS_WRITE : 0u);
		vbufferDesc.MiscFlags = 0u;
		vbufferDesc.ByteWidth = size;

		D3D11_SUBRESOURCE_DATA vbufferData = { 0 };
		if (vertices == nullptr)
			vbufferData.pSysMem = (const void*)new byte[size]{ 0 };
		else
			vbufferData.pSysMem = vertices;

		HRESULT hr = graphics.GetDivice()->CreateBuffer(&vbufferDesc, &vbufferData, &m_Buffer);

		if (FAILED(hr))
			DBOUT("failed to create vertex buffer " << TranslateError(hr) << std::endl);

		if (vertices == nullptr)
			delete[] vbufferData.pSysMem;
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

	void DirectX11VertexBuffer::SetData(const void* data, uint32_t size)
	{
		DirectX11RendererAPI& graphics = *(DirectX11RendererAPI*)RendererAPI::Get();

		D3D11_MAPPED_SUBRESOURCE msub = {};

		HRESULT hr = graphics.GetContext()->Map(m_Buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msub);

		if (FAILED(hr))
		{
			DBOUT("failed to update vertex buffer " << TranslateError(hr) << std::endl);
			return;
		}

		CopyMemory(msub.pData, data, size);
		graphics.GetContext()->Unmap(m_Buffer.Get(), 0);
	}


	// -------------------------------- Index Buffer -------------------------------- // 
	DirectX11IndexBuffer::DirectX11IndexBuffer(const uint32_t* indices, const uint32_t count)
	{
		DirectX11RendererAPI& graphics = *(DirectX11RendererAPI*)RendererAPI::Get();
		m_Count = count;

		D3D11_BUFFER_DESC ibufferDesc = { 0 };
		ibufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibufferDesc.Usage = (indices == nullptr ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT);
		ibufferDesc.CPUAccessFlags = (indices == nullptr ? D3D11_CPU_ACCESS_WRITE : 0u);
		ibufferDesc.MiscFlags = 0u;
		ibufferDesc.ByteWidth = sizeof(uint32_t) * count;
		ibufferDesc.StructureByteStride = sizeof(uint32_t);

		D3D11_SUBRESOURCE_DATA ibufferData = { 0 };
		if (indices == nullptr)
			ibufferData.pSysMem = (const void*)new uint32_t[count]{ 0 };
		else
			ibufferData.pSysMem = indices;

		graphics.GetDivice()->CreateBuffer(&ibufferDesc, &ibufferData, &m_Buffer);

		if (indices == nullptr)
			delete[] ibufferData.pSysMem;
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

	void DirectX11IndexBuffer::SetData(const uint32_t* indices, uint32_t count)
	{
		DirectX11RendererAPI& graphics = *(DirectX11RendererAPI*)RendererAPI::Get();

		D3D11_MAPPED_SUBRESOURCE msub = {};

		HRESULT hr = graphics.GetContext()->Map(m_Buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msub);

		if (FAILED(hr))
		{
			DBOUT("failed to update index buffer " << TranslateError(hr) << std::endl);
			return;
		}

		CopyMemory(msub.pData, indices, count * sizeof(uint32_t));
		graphics.GetContext()->Unmap(m_Buffer.Get(), 0);
	}


	// -------------------------------- Constent Buffer -------------------------------- // 
	DirectX11ConstentBuffer::DirectX11ConstentBuffer(const void* data, uint32_t size) :
		m_Size(size)
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
		if(data == nullptr)
			cbufferData.pSysMem = (const void*) new byte[size]{ 0 };
		else
			cbufferData.pSysMem = data;
		
		HRESULT hr = graphics.GetDivice()->CreateBuffer(&cbufferDesc, &cbufferData, &m_Buffer);

		if (FAILED(hr))
			DBOUT("faild to create constent buffer " << TranslateError(hr) << std::endl);

		if (data == nullptr)
			delete[] cbufferData.pSysMem;

	}

	void DirectX11ConstentBuffer::SetData(const void* data)
	{
		DirectX11RendererAPI& graphics = *(DirectX11RendererAPI*)RendererAPI::Get();

		D3D11_MAPPED_SUBRESOURCE msub = {};

		HRESULT hr = graphics.GetContext()->Map(m_Buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msub);

		if (FAILED(hr))
		{
			DBOUT("failed to update constent buffer " << TranslateError(hr) << std::endl);
			return;
		}

		CopyMemory(msub.pData, data, m_Size);
		graphics.GetContext()->Unmap(m_Buffer.Get(), 0);
	}

	// -------------------------------- Structured Buffer -------------------------------- // 

	DirectX11StructuredBuffer::DirectX11StructuredBuffer(uint32_t elementSize, uint32_t elementCount) :
		m_ElementSize(elementSize), m_ElementCount(elementCount)
	{
		if (elementCount == 0)
			return;

		DirectX11RendererAPI& graphics = *(DirectX11RendererAPI*)RendererAPI::Get();

		D3D11_BUFFER_DESC sbufferDesc = {  };
		sbufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		sbufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		sbufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		sbufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		sbufferDesc.ByteWidth = elementCount * elementSize;
		sbufferDesc.StructureByteStride = elementSize;

		D3D11_SUBRESOURCE_DATA sbufferData = { 0 };
		sbufferData.pSysMem = new uint8_t[elementCount * elementSize]{ 0 };

		HRESULT hr = graphics.GetDivice()->CreateBuffer(&sbufferDesc, &sbufferData, &m_Buffer);

		if (FAILED(hr))
			DBOUT("failed to create structured buffer " << TranslateError(hr) << std::endl);

		delete[] sbufferData.pSysMem;

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = DXGI_FORMAT_UNKNOWN;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		srvDesc.Buffer.FirstElement = 0;
		srvDesc.Buffer.NumElements = elementCount;

		hr = graphics.GetDivice()->CreateShaderResourceView(m_Buffer.Get(), &srvDesc, &m_SRV);
		if (FAILED(hr))
			DBOUT("failed to create structured buffer resource view " << TranslateError(hr) << std::endl);
	}

	void DirectX11StructuredBuffer::SetData(const void* data)
	{
		if (m_ElementCount == 0)
			return;

		DirectX11RendererAPI& graphics = *(DirectX11RendererAPI*)RendererAPI::Get();

		D3D11_MAPPED_SUBRESOURCE msub = {};

		HRESULT hr = graphics.GetContext()->Map(m_Buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msub);

		if (FAILED(hr))
		{
			DBOUT("failed to update Structured buffer " << TranslateError(hr) << std::endl);
			return;
		}

		CopyMemory(msub.pData, data, m_ElementCount * m_ElementSize);
		graphics.GetContext()->Unmap(m_Buffer.Get(), 0);
	}

}