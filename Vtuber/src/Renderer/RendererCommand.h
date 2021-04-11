#pragma once

#include "Graphics.h"


namespace Engine
{
	class RendererCommand
	{
	private:
		static Graphics* s_Graphics;

	public:
		static void Init();

		static inline Graphics& GetGraphics() { return *s_Graphics; };

		static void SetViewPort(float width, float height, float x, float y);

		static void SetTarget(wrl::ComPtr<ID3D11RenderTargetView>& target);


	};
}