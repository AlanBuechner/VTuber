#include "DirectX11Shader.h"
#include "Renderer/RendererAPI.h"
#include "DirectX11RendererAPI.h"
#include "DirectX11Buffer.h"
#include "Renderer\Light.h"


#include <iostream>
#include <d3dcompiler.h>
#include <D3Dcompiler.inl>
#include <d3d11shader.h>

namespace Engine
{
	DXGI_FORMAT ShaderDataTypeToDXGIFormat(ShaderDataType& type)
	{
		switch (type)
		{
		case ShaderDataType::Float:			return DXGI_FORMAT_R32_FLOAT;
		case ShaderDataType::Float2:		return DXGI_FORMAT_R32G32_FLOAT;
		case ShaderDataType::Float3:		return DXGI_FORMAT_R32G32B32_FLOAT;
		case ShaderDataType::Float4:		return DXGI_FORMAT_R32G32B32A32_FLOAT;

		case ShaderDataType::Int:			return DXGI_FORMAT_R32_SINT;
		case ShaderDataType::Int2:			return DXGI_FORMAT_R32G32_SINT;
		case ShaderDataType::Int3:			return DXGI_FORMAT_R32G32B32_SINT;
		case ShaderDataType::Int4:			return DXGI_FORMAT_R32G32B32A32_SINT;
		default:
			break;
		}
		return DXGI_FORMAT_UNKNOWN;
	}

	DXGI_FORMAT GetFormatFromDesc(D3D11_SIGNATURE_PARAMETER_DESC& desc)
	{
		if (desc.Mask == 1)
		{
			if (desc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) return DXGI_FORMAT_R32_UINT;
			else if (desc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) return DXGI_FORMAT_R32_SINT;
			else if (desc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) return DXGI_FORMAT_R32_FLOAT;
		}
		else if (desc.Mask <= 3)
		{
			if (desc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) return DXGI_FORMAT_R32G32_UINT;
			else if (desc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) return DXGI_FORMAT_R32G32_SINT;
			else if (desc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) return DXGI_FORMAT_R32G32_FLOAT;
		}
		else if (desc.Mask <= 7)
		{
			if (desc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) return DXGI_FORMAT_R32G32B32_UINT;
			else if (desc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) return DXGI_FORMAT_R32G32B32_SINT;
			else if (desc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) return DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if (desc.Mask <= 15)
		{
			if (desc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) return DXGI_FORMAT_R32G32B32A32_UINT;
			else if (desc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) return DXGI_FORMAT_R32G32B32A32_SINT;
			else if (desc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) return DXGI_FORMAT_R32G32B32A32_FLOAT;
		}
		return DXGI_FORMAT_UNKNOWN;
	}


	DirectX11Shader::DirectX11Shader(const ShaderSource& src) :
		m_VertexShaderFile(src.VetexShader)
	{
		LoadShader(src.VetexShader, ShaderType::Vertex);
		LoadShader(src.PixelShader, ShaderType::Pixel);

		GenInputLayoutFromReflection();
	}

	void DirectX11Shader::SetInputLayout(BufferLayout& layout)
	{
		DirectX11RendererAPI& graphics = *(DirectX11RendererAPI*)RendererAPI::Get();

		wrl::ComPtr<ID3DBlob> pBlob = ReadBlob(m_VertexShaderFile);

		std::vector<BufferElement> elements = layout.GetElements();

		D3D11_INPUT_ELEMENT_DESC* ied = new D3D11_INPUT_ELEMENT_DESC[elements.size()];

		for (uint32_t i = 0; i < elements.size(); i++)
		{
			BufferElement& element = elements[i];
			ied[i] = { element.Name.c_str(), 0,	ShaderDataTypeToDXGIFormat(element.Type), 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		}

		HRESULT hr = graphics.GetDivice()->CreateInputLayout(ied, elements.size(), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &m_pInputLayout);

		if (FAILED(hr)){
			DBOUT("failed to set input layout");
		}

		delete[] ied;
	}

	void DirectX11Shader::GenInputLayoutFromReflection()
	{
		DirectX11RendererAPI& graphics = *(DirectX11RendererAPI*)RendererAPI::Get();

		wrl::ComPtr<ID3DBlob> pBlob = ReadBlob(m_VertexShaderFile);

		// reflect on the shader
		ID3D11ShaderReflection* pReflector = nullptr;
		HRESULT hr = D3DReflect(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pReflector);

		// check if the reflection failed
		if (FAILED(hr)) {
			DBOUT("shader reflection failed" << std::endl);
			return;
		}

		// get the descriptor for the shader
		D3D11_SHADER_DESC reflectDesc;
		pReflector->GetDesc(&reflectDesc);

		uint32_t numInputParams = reflectDesc.InputParameters;
		D3D11_INPUT_ELEMENT_DESC* ied = new D3D11_INPUT_ELEMENT_DESC[numInputParams];

		for (uint32_t i = 0; i < numInputParams; i++)
		{
			D3D11_SIGNATURE_PARAMETER_DESC ps;
			pReflector->GetInputParameterDesc(i, &ps);

			ied[i] = { ps.SemanticName, ps.SemanticIndex, GetFormatFromDesc(ps), 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		}

		hr = graphics.GetDivice()->CreateInputLayout(ied, numInputParams, pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &m_pInputLayout);

		if (FAILED(hr)) {
			DBOUT("failed to create layout from reflection" << std::endl);
		}

		delete[] ied;
	}

	void DirectX11Shader::SetBuffer(const std::string& name, const void* data)
	{
		auto& bufferinfo = m_ConstentBuffers[name];
		bufferinfo.buffer->SetData(data);
		SetConstantBuffer(bufferinfo);
	}

	void DirectX11Shader::SetBuffer(const std::string& name, const void* data, uint32_t count)
	{
		auto& bufferinfo = m_StructuredBuffers[name];
		auto& buff = bufferinfo.buffer;
		if (buff->GetCount() != count)
			bufferinfo.buffer = StructuredBuffer::Create(buff->GetSize(), count);
		bufferinfo.buffer->SetData(data);
		SetStructuredBuffer(bufferinfo);
	}

	void DirectX11Shader::Bind()
	{
		DirectX11RendererAPI& graphics = *(DirectX11RendererAPI*)RendererAPI::Get();

		graphics.GetContext()->PSSetShader(m_pPixelShader.Get(), nullptr, 0u);
		graphics.GetContext()->VSSetShader(m_pVertexShader.Get(), nullptr, 0u);

		graphics.GetContext()->IASetInputLayout(m_pInputLayout.Get());
	}

	void DirectX11Shader::Unbind()
	{
		// TODO
	}

	bool DirectX11Shader::operator==(const Shader& other)
	{
		DirectX11Shader& o = (DirectX11Shader&)other;
		return m_pVertexShader == o.m_pVertexShader && m_pPixelShader == o.m_pPixelShader;
	}

	void DirectX11Shader::LoadShader(const std::wstring& file, ShaderType type)
	{
		DirectX11RendererAPI& graphics = *(DirectX11RendererAPI*)RendererAPI::Get();

		wrl::ComPtr<ID3DBlob> pBlob = ReadBlob(file);

		switch (type)
		{
		case Engine::Shader::ShaderType::Vertex:
			graphics.GetDivice()->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &m_pVertexShader);
			break;
		case Engine::Shader::ShaderType::Pixel:
			graphics.GetDivice()->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &m_pPixelShader);
			break;
		default:
			break;
		}

		GenBuffers(pBlob, type);
	}

	void DirectX11Shader::SetConstantBuffer(const ConstentBufferInfo& cb)
	{
		DirectX11RendererAPI& graphics = *(DirectX11RendererAPI*)RendererAPI::Get();
		const DirectX11ConstentBuffer& d3dcb = (DirectX11ConstentBuffer&)*cb.buffer.get();

		if (cb.type == ShaderType::Vertex)
			graphics.GetContext()->VSSetConstantBuffers(cb.slot, 1u, d3dcb.GetBuffer().GetAddressOf());
		else if (cb.type == ShaderType::Pixel)
			graphics.GetContext()->PSSetConstantBuffers(cb.slot, 1u, d3dcb.GetBuffer().GetAddressOf());
	}

	void DirectX11Shader::SetStructuredBuffer(const StructuredBufferInfo& sb)
	{
		DirectX11RendererAPI& graphics = *(DirectX11RendererAPI*)RendererAPI::Get();
		const DirectX11StructuredBuffer& d3dsb = (DirectX11StructuredBuffer&)*sb.buffer.get();

		if (sb.type == ShaderType::Vertex)
			graphics.GetContext()->VSSetShaderResources(sb.slot, 1u, d3dsb.GetSRV().GetAddressOf());
		else if (sb.type == ShaderType::Pixel)
			graphics.GetContext()->PSSetShaderResources(sb.slot, 1u, d3dsb.GetSRV().GetAddressOf());
	}

	void DirectX11Shader::GenBuffers(wrl::ComPtr<ID3DBlob> pBlob, ShaderType type)
	{
		// reflect on the shader
		ID3D11ShaderReflection* pReflector = nullptr;
		HRESULT hr = D3DReflect(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pReflector);

		// check if the reflection failed
		if (FAILED(hr)) {
			DBOUT("shader reflection failed" << std::endl);
			return;
		}

		// get the descriptor for the shader
		D3D11_SHADER_DESC reflectDesc;
		pReflector->GetDesc(&reflectDesc);

		// itorate over all the constent buffers
		for (uint32_t cbIndex = 0; cbIndex < reflectDesc.ConstantBuffers; cbIndex++) 
		{
			// get the constent buffer
			ID3D11ShaderReflectionConstantBuffer* cb = pReflector->GetConstantBufferByIndex(cbIndex);
			if (cb)
			{
				// get the descriptor of the constent buffer
				D3D11_SHADER_BUFFER_DESC cbDesc;
				cb->GetDesc(&cbDesc);
				if (cbDesc.Type == D3D11_CT_CBUFFER)
				{
					// create a new constent buffer object and added it to the map
					m_ConstentBuffers[cbDesc.Name] = { ConstentBuffer::Create(cbDesc.Size), cbIndex, type};
				}
			}
		}

		// itorate over all BoundResources
		for (uint32_t srvIndex = 0; srvIndex < reflectDesc.BoundResources; srvIndex++)
		{
			D3D11_SHADER_INPUT_BIND_DESC bindDesc;
			pReflector->GetResourceBindingDesc(srvIndex, &bindDesc);
			if (bindDesc.Type == D3D_SHADER_INPUT_TYPE::D3D_SIT_STRUCTURED)
			{
				m_StructuredBuffers[bindDesc.Name] = { StructuredBuffer::Create(sizeof(PointLight), 0), bindDesc.BindPoint, type };
			}
		}
	}

	wrl::ComPtr<ID3DBlob> DirectX11Shader::ReadBlob(const std::wstring& fileName)
	{
		wrl::ComPtr<ID3DBlob> pBlob;
		HRESULT hr = D3DReadFileToBlob((L"ShaderBin/" + fileName).c_str(), &pBlob);

		if (FAILED(hr))
			DBOUT("failed to read from file \"ShaderBin/" << fileName << "\"" << std::endl);

		return pBlob;
	}

}