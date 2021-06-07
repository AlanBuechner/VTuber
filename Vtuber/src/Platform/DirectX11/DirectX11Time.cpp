#include "DirectX11Time.h"
#include <chrono>

namespace Engine
{
	using namespace std::chrono;

	double DirectX11Time::GetTimeImpl()
	{
		return duration<double>(system_clock::now().time_since_epoch()).count();
	}
}