#pragma once
#include "Core/Core.h"
#include "Platform/Windows/Win.h"
#include "Renderer/RendererAPI.h"


#include <d3dcompiler.h>
#include <dxgi.h>

#pragma comment(lib, "DXGI.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

namespace Engine
{
	class DirectX11RendererAPI : public RendererAPI
	{
	public:
		virtual void Init() override;

		virtual void SetViewPort(int width, int height, int x, int y) override;
		virtual void DrawIndexed(uint32_t count) override;

		void DrawTestTriangle();


		inline wrl::ComPtr<ID3D11Device> GetDivice() { return pDevice; }
		inline wrl::ComPtr<ID3D11DeviceContext> GetContext() { return pContext; }
		inline wrl::ComPtr<IDXGIFactory> GetFactory() { return pDXGIFactory; }

	private:
		wrl::ComPtr<ID3D11Device> pDevice;
		wrl::ComPtr<ID3D11DeviceContext> pContext;
		wrl::ComPtr<IDXGIFactory> pDXGIFactory;
	};
}