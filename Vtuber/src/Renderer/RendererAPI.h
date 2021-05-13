#pragma once
#include "Core/Core.h"

namespace Engine
{
	class RendererAPI
	{
	public:
		enum API
		{
			DirectX11,
			OpenGL, // not sapported
			Vulkan, // not sapported
		};

	private:

		static RendererAPI* s_Instance;
		static API s_API;

	public:
		inline static RendererAPI* Get() { return s_Instance; }
		inline static API GetAPI() { return s_API; }
		static void CreateAPI(API api);

		virtual void Init() = 0;

		virtual void SetViewPort(int width, int height, int x, int y) = 0;
		virtual void DrawIndexed(uint32_t count) = 0;


		
	};
}