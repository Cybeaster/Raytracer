#pragma once
#include "../Logger.h"
#include "../ExitHelper.h"
#include <chrono>

#ifdef _MSC_VER
#define FUNCTION_SIGNATURE __FUNCSIG__
#elif defined(__GNUC__) || defined(__clang__)
#define FUNCTION_SIGNATURE __PRETTY_FUNCTION__
#else
#define FUNCTION_SIGNATURE __FUNCTION__
#endif

#define PROFILE(...)\
	auto _now = std::chrono::steady_clock::now();\
	auto _funcName = std::string(FUNCTION_SIGNATURE);\
	auto _meta = std::string(__VA_ARGS__);\
	SExitHelper _ExitHelper([_now,_funcName,_meta]()\
	{\
		auto thisEnd = std::chrono::steady_clock::now();\
		auto diff = thisEnd - _now;\
		auto ms = std::chrono::duration<double, std::milli>(diff).count();\
		auto time = std::to_string(ms);\
		OProfiler::Get()->Log(SLogUtils::Format("[{}] Execution time [{}] {} ms", _meta, _funcName, time));\
	});

class OProfiler
{
public:
	static OProfiler* Get()
	{
		if (Profiler == nullptr)
		{
			Profiler = new OProfiler();
		}
		return Profiler;
	}

	void Log(const std::string& Message)
	{
		LOG(Log, Message);
		Logger.Log(Message);
	}

private:
	OProfiler()
		: Logger(std::make_shared<std::ofstream>("profile.txt"))
	{
	}


	static inline OProfiler* Profiler = nullptr;
	SLogger<std::ofstream> Logger;
};
