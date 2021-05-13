#pragma once

#include <stdint.h>
#include <string>
#include <vector>
#include <memory>
#include <DirectXMath.h>

namespace dx = DirectX;

#if defined(_WIN32) || defined(_WIN64)
	#define PLATFORM_WINDOWS
#elif defined(__APPLE__) || defined(__MACH__)
	#include <TargetConditionals.h>
	#if TARGET_OS_MAC == 1
		#define PLATFORM_MACOS
		#error "MacOS is not sapported"
	#endif
#elif defined(__linux__)
	#define PLATFORM_LINUX
	#error "Linux is not sapported"
#else
	#error "Unknown Platform!"
#endif



namespace Engine
{
	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T>
	using Scope = std::unique_ptr<T>;
}