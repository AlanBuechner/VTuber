#pragma once

#include "Core/Win.h"
#include "Core/Core.h"
#include "Buffer.h"


namespace Engine
{
	class Shader
	{
	public:

		enum ShaderType
		{
			Vertex,
			Pixel
		};

		void LoadVertexShader(std::wstring fileName);
		void LoadPixleShader(std::wstring fileName);

		void SetInputLayout(BufferLayout& layout);
		void GenInputLayoutFromReflection();

		void Bind();
		void Unbind();

	private:
		wrl::ComPtr<ID3DBlob> ReadBlob(std::wstring& fileName);

	private:
		std::wstring m_VertexShaderFile;
		wrl::ComPtr<ID3D11InputLayout> m_pInputLayout;

		wrl::ComPtr<ID3D11VertexShader> m_pVertexShader;
		wrl::ComPtr<ID3D11PixelShader> m_pPixelShader;

	};
}