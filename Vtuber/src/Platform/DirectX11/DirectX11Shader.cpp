#include "DirectX11Shader.h"
#include "Renderer/RendererAPI.h"
#include "DirectX11RendererAPI.h"
#include "DirectX11Buffer.h"


#include <iostream>
#include <d3dcompiler.h>
#include <D3Dcompiler.inl>
#include <d3d12shader.h>

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


	void DirectX11Shader::LoadVertexShader(std::wstring fileName)
	{
		m_VertexShaderFile = fileName;
		DirectX11RendererAPI& graphics = *(DirectX11RendererAPI*)RendererAPI::Get();

		wrl::ComPtr<ID3DBlob> pBlob = ReadBlob(fileName);

		graphics.GetDivice()->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &m_pVertexShader);
	}

	void DirectX11Shader::LoadPixleShader(std::wstring fileName)
	{
		DirectX11RendererAPI& graphics = *(DirectX11RendererAPI*)RendererAPI::Get();

		wrl::ComPtr<ID3DBlob> pBlob = ReadBlob(fileName);

		graphics.GetDivice()->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &m_pPixelShader);
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
			ied[i] = { element.Name.c_str(), i,	ShaderDataTypeToDXGIFormat(element.Type), 0, element.Offset, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		}

		graphics.GetDivice()->CreateInputLayout(ied, 1u, pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &m_pInputLayout);

		delete[] ied;
	}

	void DirectX11Shader::GenInputLayoutFromReflection()
	{
		/*Graphics& graphics = RendererCommand::GetGraphics();

		wrl::ComPtr<ID3DBlob> pBlob = ReadBlob(m_VertexShaderFile);

		wrl::ComPtr<ID3D11ShaderReflection> pReflection;
		D3DReflect(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), IID_ID3D11ShaderReflection, &pReflection);

		D3D11_SHADER_DESC shaderDesc;
		pReflection->GetDesc(&shaderDesc);

		uint32_t numInputParams = shaderDesc.InputParameters;
		D3D11_INPUT_ELEMENT_DESC* ied = new D3D11_INPUT_ELEMENT_DESC[numInputParams];

		for (uint32_t i = 0; i < numInputParams; i++)
		{
			D3D11_SIGNATURE_PARAMETER_DESC ps;
			pReflection->GetInputParameterDesc(i, &ps);

			ied[i] = { ps.SemanticName, ps.SemanticIndex, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		}

		graphics.GetDivice()->CreateInputLayout(ied, 1u, pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &m_pInputLayout);

		delete[] ied;*/
	}

	void DirectX11Shader::SetConstantBuffer(uint32_t slot, const ConstentBuffer& cb)
	{
		DirectX11RendererAPI& graphics = *(DirectX11RendererAPI*)RendererAPI::Get();
		const DirectX11ConstentBuffer& d3dcb = (DirectX11ConstentBuffer&)cb;

		graphics.GetContext()->VSSetConstantBuffers(slot, 1u, d3dcb.GetBuffer().GetAddressOf());
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
	}

	wrl::ComPtr<ID3DBlob> DirectX11Shader::ReadBlob(std::wstring& fileName)
	{
		wrl::ComPtr<ID3DBlob> pBlob;
		D3DReadFileToBlob((L"ShaderBin/" + fileName).c_str(), &pBlob);

		return pBlob;
	}

}