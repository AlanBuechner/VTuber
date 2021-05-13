#pragma once
#include "Platform/Windows/Win.h"
#include "Core/Core.h"
#include "Renderer/Shader.h"

namespace Engine
{
	class DirectX11Shader : public Shader
	{
	public:

		virtual void LoadVertexShader(std::wstring fileName) override;
		virtual void LoadPixleShader(std::wstring fileName) override;

		virtual void SetInputLayout(BufferLayout& layout) override;
		virtual void GenInputLayoutFromReflection() override;

		virtual void SetConstantBuffer(uint32_t slot, const ConstentBuffer& cb) override;

		virtual void Bind() override;
		virtual void Unbind() override;

	private:
		wrl::ComPtr<ID3DBlob> ReadBlob(std::wstring& fileName);

	private:
		std::wstring m_VertexShaderFile;
		wrl::ComPtr<ID3D11InputLayout> m_pInputLayout;

		wrl::ComPtr<ID3D11VertexShader> m_pVertexShader;
		wrl::ComPtr<ID3D11PixelShader> m_pPixelShader;
	};
}