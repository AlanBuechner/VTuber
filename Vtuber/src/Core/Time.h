#pragma once

namespace Engine
{
	class Application;
}

namespace Engine
{
	class Time
	{
		friend Application;

	public:
		Time() {}

		static inline float GetDeltaTime() { return m_DeltaTime; }
		static inline float GetDeltaSeconds() { return m_DeltaTime; }
		static inline float GetDeltaMilliseconds() { return m_DeltaTime * 1000.0f; }

		static double GetTime();

	private:
		static void UpdateDeltaTime();

	protected:
		static float m_DeltaTime;
		static float m_LastFrameTime;
	};
}

typedef Engine::Time Time;