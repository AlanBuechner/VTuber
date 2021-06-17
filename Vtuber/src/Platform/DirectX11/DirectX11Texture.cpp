#include "Platform/Windows/Win.h"
#include "DirectX11Texture.h"
#include "stb_image.h"

#include "Renderer/RendererCommand.h"
#include "Renderer/RendererAPI.h"
#include "DirectX11RendererAPI.h"

namespace Engine
{
	DXGI_FORMAT GetFormat(int channels)
	{
		switch (channels)
		{
		case 1:
			return DXGI_FORMAT_R8_UNORM;
		case 2:
			return DXGI_FORMAT_R8G8_UNORM;
		case 3:
			return DXGI_FORMAT_B8G8R8X8_UNORM;
		case 4:
			return DXGI_FORMAT_B8G8R8A8_UNORM;
		default:
			break;
		}
	}

	D3D11_FILTER GetMinMagFilter(TextureAttribute::MinMagFilter min, TextureAttribute::MinMagFilter mag)
	{
		switch (min)
		{
		case Engine::TextureAttribute::MinMagFilter::Nearest:
			switch (mag)
			{
			case Engine::TextureAttribute::MinMagFilter::Nearest:
				return D3D11_FILTER_MIN_MAG_MIP_POINT;
			case Engine::TextureAttribute::MinMagFilter::Linear:
				return D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
			}
		case Engine::TextureAttribute::MinMagFilter::Linear:
			switch (mag)
			{
			case Engine::TextureAttribute::MinMagFilter::Nearest:
				return D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;
			case Engine::TextureAttribute::MinMagFilter::Linear:
				return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			}
		}
	}

	D3D11_TEXTURE_ADDRESS_MODE GetAddressMode(TextureAttribute::WrapMode mode)
	{
		switch (mode)
		{
		case Engine::TextureAttribute::WrapMode::Repeat:
			return D3D11_TEXTURE_ADDRESS_WRAP;
		case Engine::TextureAttribute::WrapMode::MirroredRepeat:
			return D3D11_TEXTURE_ADDRESS_MIRROR;
		case Engine::TextureAttribute::WrapMode::ClampToEdge:
			return D3D11_TEXTURE_ADDRESS_CLAMP;
		}
	}

	uint32_t ChannelSize(int channels)
	{
		if (channels == 3)
			channels = 4;
		return channels * sizeof(uint8_t);
	}

	DirectX11Texture::DirectX11Texture(const std::string& path, Ref<TextureAttribute> attribute) :
		m_Attribute(attribute)
	{
		LoadFromFile(path);
		GenSampler();
	}

	DirectX11Texture::DirectX11Texture(const uint32_t width, const uint32_t height, Ref<TextureAttribute> attribute) :
		m_Attribute(attribute)
	{
		stbi_uc* data = new stbi_uc[width * height * sizeof(uint8_t) * 4];
		for (uint32_t i = 0; i < width * height * sizeof(uint8_t) * 4; i++)
			data[i] = 0xff;
		GenTextureView(data, width, height, 4);

		delete[] data;

		GenSampler();
	}

	DirectX11Texture::~DirectX11Texture()
	{
	}

	void DirectX11Texture::SetData(void* data, uint32_t size)
	{
		// TODO
	}

	void DirectX11Texture::Bind(uint32_t slot) const
	{
		DirectX11RendererAPI& graphics = *(DirectX11RendererAPI*)RendererAPI::Get();
		graphics.GetContext()->PSSetShaderResources(slot, 1u, m_pTextureView.GetAddressOf());
		graphics.GetContext()->PSSetSamplers(slot, 1u, m_pSampler.GetAddressOf());
	}

	void DirectX11Texture::LoadFromFile(const std::string& path)
	{

		int width, height, channels;
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 4);
		m_Width = width; m_Height = height;

		if (data == nullptr)
		{
			DBOUT("failed to load image " << path.c_str() << std::endl);
			return;
		}

		GenTextureView(data, width, height, channels);

		delete[] data;

	}

	bool DirectX11Texture::operator==(const Texture& other) const
	{
		DirectX11Texture& o = (DirectX11Texture&)other;
		return (m_pTextureView == o.m_pTextureView) && (m_pSampler == o.m_pSampler);
	}

	void DirectX11Texture::GenTextureView(void* data, uint32_t width, uint32_t height, uint32_t channels)
	{
		DirectX11RendererAPI& graphics = *(DirectX11RendererAPI*)RendererAPI::Get();

		D3D11_TEXTURE2D_DESC textureDesc = { 0 };
		textureDesc.Width = width;
		textureDesc.Height = height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = GetFormat(channels);
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA sd = { 0 };
		sd.pSysMem = (void*)data;
		sd.SysMemPitch = width * ChannelSize(channels);

		wrl::ComPtr<ID3D11Texture2D> pTexture;
		HRESULT hr = graphics.GetDivice()->CreateTexture2D(&textureDesc, &sd, &pTexture);
		if (FAILED(hr)) {
			DBOUT("failed to create texture");
			return;
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = { };
		srvDesc.Format = textureDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;
		hr = graphics.GetDivice()->CreateShaderResourceView(pTexture.Get(), &srvDesc, &m_pTextureView);
		if (FAILED(hr)) {
			DBOUT("failed to create texture resorce");
			return;
		}
	}

	void DirectX11Texture::GenSampler()
	{
		DirectX11RendererAPI& graphics = *(DirectX11RendererAPI*)RendererAPI::Get();

		D3D11_SAMPLER_DESC samplerDesc = {};
		samplerDesc.Filter = GetMinMagFilter(m_Attribute->Min, m_Attribute->Mag);
		samplerDesc.AddressU = GetAddressMode(m_Attribute->U);
		samplerDesc.AddressV = GetAddressMode(m_Attribute->V);
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

		graphics.GetDivice()->CreateSamplerState(&samplerDesc, &m_pSampler);
	}

}