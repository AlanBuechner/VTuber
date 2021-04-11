#pragma once

#include <stdint.h>
#include <string>
#include <vector>
#include <memory>

namespace Engine
{
	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T>
	using Scope = std::unique_ptr<T>;
}