#pragma once

#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>
#include <thread>

namespace Engine
{
	struct ProfileResult
	{
		std::string Name;
		long long start, end;
		uint32_t threadID;
	};

	struct InstrumentationSession
	{
		std::string Name;
	};

	class Instrumentor
	{
	private:
		InstrumentationSession* currentSession;
		std::ofstream outputStream;
		int profileCount;
	public:
		Instrumentor()
			: currentSession(nullptr), profileCount(0)
		{
		}

		void beginSession(const std::string& name, const std::string& filepath = "results.json")
		{
			outputStream.open(filepath);
			writeHeader();
			currentSession = new InstrumentationSession{ name };
		}

		void endSession()
		{
			writeFooter();
			outputStream.close();
			delete currentSession;
			currentSession = nullptr;
			profileCount = 0;
		}

		void writeProfile(const ProfileResult& result)
		{
			if (profileCount++ > 0)
				outputStream << ",";

			std::string name = result.Name;
			std::replace(name.begin(), name.end(), '"', '\'');

			outputStream << "{";
			outputStream << "\"cat\":\"function\",";
			outputStream << "\"dur\":" << (result.end - result.start) << ',';
			outputStream << "\"name\":\"" << name << "\",";
			outputStream << "\"ph\":\"X\",";
			outputStream << "\"pid\":0,";
			outputStream << "\"tid\":" << result.threadID << ",";
			outputStream << "\"ts\":" << result.start;
			outputStream << "}";

			outputStream.flush();
		}

		void writeHeader()
		{
			outputStream << "{\"otherData\": {},\"traceEvents\":[";
			outputStream.flush();
		}

		void writeFooter()
		{
			outputStream << "]}";
			outputStream.flush();
		}

		static Instrumentor& get()
		{
			static Instrumentor instance;
			return instance;
		}
	};

	class InstrumentationTimer
	{
	public:
		InstrumentationTimer(const char* _name)
			: name(_name), stopped(false)
		{
			startTimepoint = std::chrono::high_resolution_clock::now();
		}

		~InstrumentationTimer()
		{
			if (!stopped)
				stop();
		}

		void stop()
		{
			auto endTimepoint = std::chrono::high_resolution_clock::now();

			long long start = std::chrono::time_point_cast<std::chrono::microseconds>(startTimepoint).time_since_epoch().count();
			long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

			uint32_t threadID = (uint32_t)std::hash<std::thread::id>{}(std::this_thread::get_id());
			Instrumentor::get().writeProfile({ name, start, end, threadID });

			stopped = true;
		}
	private:
		const char* name;
		std::chrono::time_point<std::chrono::high_resolution_clock> startTimepoint;
		bool stopped;
	};
}

#define ENGINE_PROFILE 1
#if ENGINE_PROFILE
#define ENGINE_PROFILE_BEGIN_SESSION(name, filepath) ::Engine::Instrumentor::get().beginSession(name, filepath)
#define ENGINE_PROFILE_END_SESSION() ::Engine::Instrumentor::get().endSession()
#define ENGINE_PROFILE_SCOPE(name) ::Engine::InstrumentationTimer timer##__LINE__(name);
#define ENGINE_PROFILE_FUNCTION() ENGINE_PROFILE_SCOPE(__FUNCSIG__)
#else
#define ENGINE_PROFILE_BEGIN_SESSION(name, filepath)
#define ENGINE_PROFILE_END_SESSION()
#define ENGINE_PROFILE_SCOPE(name)
#define ENGINE_PROFILE_FUNCTION()
#endif
