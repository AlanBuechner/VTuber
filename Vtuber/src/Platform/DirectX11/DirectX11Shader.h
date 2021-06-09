#pragma once
#include "Platform/Windows/Win.h"
#include "Core/Core.h"
#include "Renderer/Shader.h"

#include <map>
#include <string>

namespace Engine
{
	class ConstentBuffer;
}

namespace Engine
{
	class DirectX11Shader : public Shader
	{

		struct ConstentBufferInfo
		{
			Ref<ConstentBuffer> buffer;
			uint32_t slot;
			ShaderType type;
		};

	public:

		virtual void LoadVertexShader(std::wstring fileName) override;
		virtual void LoadPixleShader(std::wstring fileName) override;

		virtual void SetInputLayout(BufferLayout& layout) override;
		virtual void GenInputLayoutFromReflection() override;

		virtual void SetBuffer(const std::string& name, const void* data) override;

		virtual void Bind() override;
		virtual void Unbind() override;

	private:
		void SetConstantBuffer(uint32_t slot, const ConstentBufferInfo& cb);
		void GenConstentBuffers(wrl::ComPtr<ID3DBlob> pBlob, ShaderType type);

	private:
		wrl::ComPtr<ID3DBlob> ReadBlob(std::wstring& fileName);

	private:
		std::wstring m_VertexShaderFile;
		wrl::ComPtr<ID3D11InputLayout> m_pInputLayout;

		wrl::ComPtr<ID3D11VertexShader> m_pVertexShader;
		wrl::ComPtr<ID3D11PixelShader> m_pPixelShader;

		std::map<const std::string, ConstentBufferInfo> m_Buffers;
	};
}