// https://gist.github.com/TheCherno/31f135eea6ee729ab5f26a6908eb3a5e#file-instrumentor-h

#pragma once
#include <iostream>
#include <fstream>
#include "Core/Time.h"
#include "Core/Log.h"
#include <string>
#include <vector>
#include <thread>

namespace Engine
{
	struct ProfileResult
	{
		const char* name;
		long long start, end;
		uint32_t ThreadID;
	};

	struct InstrumentationSession
	{
		std::string Name;
	};

	class Instrumentor
	{
	private:
		InstrumentationSession* m_CurrentSession;
		std::ofstream m_OutputStream;
		int m_ProfileCount;
		bool m_RecordData;
	public:
		Instrumentor(bool record = false)
			: m_CurrentSession(nullptr), m_ProfileCount(0), m_RecordData(record)
		{
		}

		void BeginSession(const std::string& name, const std::string& filepath = "results.json");

		void EndSession();

		void WriteProfile(const ProfileResult& result);

		void WriteHeader();

		void WriteFooter();

		void RecordData(bool record);

		static Instrumentor& Get();
	};

	template<typename Fn>
	class Timer
	{
	public:
		Timer(Fn&& func) :
			m_Func(func)
		{	
		}

		void Start(const std::string& name)
		{
			m_Name = name;
			m_Start = Time::GetTime();
		}

		void End()
		{
			m_End = Time::GetTime();
			m_Elapsed = m_End - m_Start;

			long long start = (long long)(m_Start * 1000000.0f);
			long long end = (long long)(m_End * 1000000.0f);
			uint32_t threadID = (uint32_t)std::hash<std::thread::id>{}(std::this_thread::get_id());
			ProfileResult result = { m_Name.c_str(), start, end, threadID };
			m_Func(result);
		}

		double GetSeconds()
		{
			return m_Elapsed;
		}

		double GetMilliseconds()
		{
			return m_Elapsed * 1000.0f;
		}

		double GetMicroseconds()
		{
			return m_Elapsed * 1000000.0f;
		}

		void PrintTime()
		{
			DEBUG_INFO("{0} : {1}", GetMilliseconds(), m_Name);
		}
	private:
		double m_Start;
		double m_End;

		double m_Elapsed;

		std::string m_Name;

		Fn m_Func;
	};

	class InstrumentationTimer
	{
	public:
		InstrumentationTimer()
		{

		}

		void Start(const std::string& name)
		{
			m_Name = name;
			m_Start = Time::GetTime();
		}

		void End()
		{
			m_End = Time::GetTime();
			m_Elapsed = m_End - m_Start;

			long long start = (long long)(m_Start * 1000000.0f);
			long long end = (long long)(m_End * 1000000.0f);
			uint32_t threadID = (uint32_t)std::hash<std::thread::id>{}(std::this_thread::get_id());
			ProfileResult result = { m_Name.c_str(), start, end, threadID };

			Instrumentor::Get().WriteProfile(result);
		}

		double GetSeconds()
		{
			return m_Elapsed;
		}

		double GetMilliseconds()
		{
			return m_Elapsed * 1000.0f;
		}

		double GetMicroseconds()
		{
			return m_Elapsed * 1000000.0f;
		}

		void PrintTime()
		{
			DBOUT(GetMilliseconds() << " : " << m_Name.c_str() << std::endl);
		}
	private:
		double m_Start;
		double m_End;

		double m_Elapsed;

		std::string m_Name;
	};

	template<typename Fn>
	class TimerScoped
	{
	public:
		TimerScoped(const std::string& name, Fn&& func) :
			m_Func(func), m_Name(name)
		{
			m_Start = Time::GetTime();
		}

		~TimerScoped()
		{
			m_End = Time::GetTime();
			m_Elapsed = m_End - m_Start;

			long long start = m_Start * 1000000.0f;
			long long end = m_End * 1000000.0f;
			uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
			ProfileResult result = { m_Name.RawString(), start, end, threadID };
			m_Func(result);
		}

		double GetMilliseconds()
		{
			return m_Elapsed * 1000.0f;
		}
	private:
		double m_Start;
		double m_End;

		double m_Elapsed;

		const std::string m_Name;

		Fn m_Func;
	};

	class InstrumentationTimerScoped
	{
	public:
		InstrumentationTimerScoped(const std::string& name) :
			m_Name(name)
		{
			m_Start = Time::GetTime();
		}

		~InstrumentationTimerScoped()
		{
			m_End = Time::GetTime();
			m_Elapsed = m_End - m_Start;

			long long start = (long long)(m_Start * 1000000.0f);
			long long end = (long long)(m_End * 1000000.0f);
			uint32_t threadID = (uint32_t)std::hash<std::thread::id>{}(std::this_thread::get_id());
			ProfileResult result = { m_Name.c_str(), start, end, threadID };
			
			Instrumentor::Get().WriteProfile(result);
		}

		double GetMilliseconds()
		{
			return m_Elapsed * 1000.0f;
		}
	private:
		double m_Start;
		double m_End;

		double m_Elapsed;

		const std::string m_Name;
	};

	class Performance
	{
	public:
		Performance()
		{

		}

		static void PushResult(ProfileResult result)
		{
			m_ProfileResults.push_back(result);
		}

	private:
		static std::vector<ProfileResult> m_ProfileResults;
		
	};
}

#define CREATE_PROFILE() Engine::Timer([&](Engine::ProfileResult profileResult) { Engine::Performance::PushResult(profileResult); });
#define CREATE_PROFILE_SCOPE(name) Engine::TimerScoped timer##__LINE__(name, [&](Engine::ProfileResult profileResult) { Engine::Performance::PushResult(profileResult); });
#define CREATE_PROFILE_FUNCTION() CREATE_PROFILE_SCOPE(__FUNCSIG__)

#define CREATE_PROFILEI() Engine::InstrumentationTimer();
#define CREATE_PROFILE_SCOPEI(name) Engine::InstrumentationTimerScoped timer##__LINE__(name);
#define CREATE_PROFILE_FUNCTIONI() CREATE_PROFILE_SCOPEI(__FUNCSIG__)