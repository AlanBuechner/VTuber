#include "RendererCommand.h"
#include "RendererAPI.h"

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


}