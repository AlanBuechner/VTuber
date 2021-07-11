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

		struct StructuredBufferInfo
		{
			Ref<StructuredBuffer> buffer;
			uint32_t slot;
			ShaderType type;
		};

	public:

		DirectX11Shader(const ShaderSource& src);

		virtual void SetInputLayout(BufferLayout& layout) override;
		virtual void GenInputLayoutFromReflection() override;

		virtual void SetBuffer(const std::string& name, const void* data) override;
		virtual void SetBuffer(const std::string& name, const void* data, uint32_t count) override;

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual bool operator==(const Shader& other) override;

	private:
		void LoadShader(const std::wstring& file, ShaderType type);

		void SetConstantBuffer(const ConstentBufferInfo& cb);
		void SetStructuredBuffer(const StructuredBufferInfo& sb);
		void GenBuffers(wrl::ComPtr<ID3DBlob> pBlob, ShaderType type);

		wrl::ComPtr<ID3DBlob> ReadBlob(const std::wstring& fileName);

	private:
		std::wstring m_VertexShaderFile;
		wrl::ComPtr<ID3D11InputLayout> m_pInputLayout;

		wrl::ComPtr<ID3D11VertexShader> m_pVertexShader;
		wrl::ComPtr<ID3D11PixelShader> m_pPixelShader;

		std::map<const std::string, ConstentBufferInfo> m_ConstentBuffers;
		std::map<const std::string, StructuredBufferInfo> m_StructuredBuffers;
	};
}