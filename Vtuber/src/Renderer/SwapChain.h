#pragma once
#include<stdarg.h>
#include "Core/Core.h"

namespace Engine
{
	class SwapChain
	{
	public:
		virtual ~SwapChain() {}

		virtual void Init(void* windowHandle) = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual void ClearColor(float r, float g, float b, float a = 1.0f) = 0;

		virtual void SetVSync(bool val) = 0;

		virtual void SwapBuffers() = 0;

		virtual void Bind() = 0;

		static SwapChain& CreateSwapChain();

	};
}