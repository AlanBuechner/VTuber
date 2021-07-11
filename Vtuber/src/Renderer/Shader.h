#pragma once

#include "Platform/Windows/Win.h"
#include "Core/Core.h"
#include "Buffer.h"
#include <string>

namespace Engine
{
	struct ShaderSource
	{
		std::wstring VetexShader;
		std::wstring PixelShader;
	};

	class Shader
	{
	public:

		enum class ShaderType
		{
			Vertex,
			Pixel
		};

		virtual void SetInputLayout(BufferLayout& layout) = 0;
		virtual void GenInputLayoutFromReflection() = 0;

		virtual void SetBuffer(const std::string& name, const void* data) = 0;
		virtual void SetBuffer(const std::string& name, const void* data, uint32_t count) = 0;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual bool operator==(const Shader& other) = 0;

		static Ref<Shader> Create(const ShaderSource& src);
		

	};
	
}