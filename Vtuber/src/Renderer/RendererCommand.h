#pragma once

#include "Platform/DirectX11/DirectX11RendererAPI.h"


namespace Engine
{
	class RendererCommand
	{

	public:
		static void Init();

		static void SetViewPort(int width, int height, int x = 0, int y = 0);


	};
}