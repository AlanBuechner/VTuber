#include "Time.h"
#include <chrono>

float Engine::Time::m_DeltaTime = 0.0f;
float Engine::Time::m_LastFrameTime = 0.0f;

namespace Engine
{
	using namespace std::chrono;

	double Time::GetTime()
	{
		return duration<double>(steady_clock::now().time_since_epoch()).count();
	}

	void Time::UpdateDeltaTime()
	{
		float time = (float)GetTime();
		m_DeltaTime = time - m_LastFrameTime;
		m_LastFrameTime = time;
	}
	
}
