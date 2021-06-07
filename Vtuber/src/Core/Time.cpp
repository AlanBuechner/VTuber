#include "Core.h"
#include "Time.h"

#include "Renderer/RendererAPI.h"

#ifdef PLATFORM_WINDOWS

#include "Platform/DirectX11/DirectX11Time.h"

#endif

double Engine::Time::m_DeltaTime = 0.0f;
double Engine::Time::m_LastFrameTime = 0.0f;

Engine::Time* Engine::Time::s_Instance = nullptr;

namespace Engine
{

	double Time::GetTime()
	{
		return s_Instance->GetTimeImpl();
	}

	void Time::UpdateDeltaTime()
	{
		double time = GetTime();
		m_DeltaTime = time - m_LastFrameTime;
		m_LastFrameTime = time;
	}

	void Time::CreateTimer()
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::DirectX11:
			s_Instance = new DirectX11Time(); // TODO : create new instance DirectX11Time class
			break;
		default:
			break;
		}

		m_LastFrameTime = GetTime();
	}
	
}
