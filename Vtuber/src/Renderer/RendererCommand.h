#pragma once
#include "Core/Core.h"

namespace Engine
{
	class RendererCommand
	{

	public:
		static void Init();

		static void SetViewPort(int width, int height, int x = 0, int y = 0);

		static void DrawIndexed(uint32_t count);


	};
}