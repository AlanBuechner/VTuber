#pragma once
#include "Core/Time.h"

namespace Engine
{
	class DirectX11Time : public Time
	{
		virtual double GetTimeImpl() override;
	};
}