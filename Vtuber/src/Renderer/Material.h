#pragma once
#include "Core/Core.h"
#include "Texture.h"

namespace Engine
{
	class Material
	{
	public:

		Ref<Texture2D> m_Diffuse;
		Ref<Texture2D> m_Specular;
	};
}