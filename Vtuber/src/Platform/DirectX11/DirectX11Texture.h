#pragma once
#include "Core/Core.h"
#include "Renderer/Texture.h"

namespace Engine
{
	class DirectX11Texture : public Texture2D
	{
	public:
		DirectX11Texture(const std::string& path, Ref<TextureAttribute> attribute);
		DirectX11Texture(const uint32_t width, const uint32_t height, Ref<TextureAttribute> attribute);
		virtual ~DirectX11Texture();

		virtual uint32_t GetWidth() const override { return m_Width; };
		virtual uint32_t GetHeight() const override { return m_Height; };
		virtual uint32_t GetRendererID() const override { return 0; };

		virtual void SetData(void* data, uint32_t size) override;
		virtual void Bind(uint32_t slot) const override;
		virtual void LoadFromFile(const std::string& path) override;
		virtual Ref<TextureAttribute> GetAttributes() const override { return m_Attribute; };

		virtual bool operator==(const Texture& other) const override;

	private:
		void GenTextureView(void* data, uint32_t width, uint32_t height, uint32_t channels);
		void GenSampler();

	private:

		uint32_t m_Width = 0, m_Height = 0;
		Ref<TextureAttribute> m_Attribute;

		wrl::ComPtr<ID3D11ShaderResourceView> m_pTextureView;
		wrl::ComPtr<ID3D11SamplerState> m_pSampler;


	};
}