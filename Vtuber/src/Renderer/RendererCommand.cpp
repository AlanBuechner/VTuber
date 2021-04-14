#include "RendererCommand.h"
#include "Platform/Windows/Win.h"

Engine::Graphics* Engine::RendererCommand::s_Graphics = nullptr;

namespace Engine
{
	void RendererCommand::Init()
	{
		s_Graphics = new Graphics();

		s_Graphics->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	}

	void RendererCommand::SetViewPort(float width, float height, float x, float y)
	{
		D3D11_VIEWPORT vp = { 0 };
		vp.Width = width;
		vp.Height = height;
		vp.MinDepth = 0;
		vp.MaxDepth = 1;
		vp.TopLeftX = x;
		vp.TopLeftY = y;
		s_Graphics->GetContext()->RSSetViewports(1u, &vp);
	}

	void RendererCommand::SetTarget(wrl::ComPtr<ID3D11RenderTargetView>& target)
	{
		s_Graphics->GetContext()->OMSetRenderTargets(1u, target.GetAddressOf(), nullptr);
	}


}