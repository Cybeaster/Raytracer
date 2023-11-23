#pragma once
#include <format>
#include <iostream>
#include <memory>
#include <ostream>

#define TEXT(Arg) \
L##Arg

enum class ELogType
{
	Log,
	Warning,
	Error,
	Critical
};

#define LOG(LogType, String, ...) \
	SLogUtils::Log(SLogUtils::Format(String, ##__VA_ARGS__), ELogType::LogType);

struct SLogUtils
{
	template<typename Object>
	static void Log(const Object& String, ELogType Type = ELogType::Log) noexcept
	{
		switch (Type)
		{
		case ELogType::Log:
			std::cout << "\n"
				<< "Log: " << String << std::endl;
			break;

		case ELogType::Warning:
			std::clog << "\n"
				<< "Warning: " << String << std::endl;
			break;

		case ELogType::Error:
			std::clog << "\n \t \t"
				<< "Error: " << String << std::endl;
			break;

		case ELogType::Critical:
			std::clog << "\n \t \t"
				<< "Critical: " << String << std::endl;
			break;
		}
	}

	template<typename... ArgTypes>
	static string Format(std::wstring_view Str, ArgTypes&&... Args)
	{
		try
		{
			return std::vformat(Str, std::make_format_args(Args...));
		}
		catch (const std::format_error& error)
		{
			return error.what() + string(Str.begin(), Str.end());
		}
	}

	template<typename... ArgTypes>
	static string Format(std::string_view Str, ArgTypes&&... Args)
	{
		try
		{
			return std::vformat(Str, std::make_format_args(Args...));
		}
		catch (const std::format_error& error)
		{
			return error.what() + string(Str);
		}
	}

	template<typename... ArgTypes>
	static void Printf(const string& Str, ArgTypes&&... Args) noexcept
	{
		std::printf(Str.c_str(), ToCString(Args)...);
	}
};


template<typename T>
struct SLogger
{
	SLogger() = default;

	explicit SLogger(std::shared_ptr<T> S)
		: Stream(S)
	{
	}

	void Log(const std::string& Message) const
	{
		auto stream = Stream.get();
		*stream << Message << "\n" << std::flush;
	}

	void SetStream(std::shared_ptr<T> S)
	{
		Stream = S;
	}

	~SLogger()
	{
		Stream->close();
	}

private:
	std::shared_ptr<T> Stream;
};
