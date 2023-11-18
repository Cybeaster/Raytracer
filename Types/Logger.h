#pragma once
#include <memory>
#include <ostream>


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
		*stream << Message <<"\n"<< std::flush;
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
