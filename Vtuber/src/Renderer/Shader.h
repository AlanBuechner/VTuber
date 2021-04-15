#pragma once

#include "Platform/Windows/Win.h"
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

		virtual void LoadVertexShader(std::wstring fileName) = 0;
		virtual void LoadPixleShader(std::wstring fileName) = 0;

		virtual void SetInputLayout(BufferLayout& layout) = 0;
		virtual void GenInputLayoutFromReflection() = 0;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		static Ref<Shader> Create();
		

	};
}