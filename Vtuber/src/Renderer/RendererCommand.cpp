#include "RendererCommand.h"
#include "RendererAPI.h"
#include "Core/Core.h"

namespace Engine
{
	void RendererCommand::Init()
	{
		RendererAPI::CreateAPI(RendererAPI::DirectX11);
		RendererAPI::Get()->Init();
	}

	void RendererCommand::SetViewPort(int width, int height, int x, int y)
	{
		RendererAPI::Get()->SetViewPort(width, height, x, y);
	}

	void RendererCommand::DrawIndexed(uint32_t count)
	{
		RendererAPI::Get()->DrawIndexed(count);
	}


}