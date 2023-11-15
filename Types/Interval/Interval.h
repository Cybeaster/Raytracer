#pragma once
#include "../Math.h"


struct SInterval
{
	const static SInterval Empty, Full;

	SInterval()
		: Max(+INFINITY)
		, Min(-INFINITY)
	{
	}

	SInterval(const double MinP, const double MaxP)
		: Min(MinP)
		, Max(MaxP)
	{
	}


	template<typename T>
	bool Contains(T Value, bool Inclusive = true) const
	{
		return Inclusive ? (Value >= Min && Value <= Max) : (Value > Min && Value < Max);
	}

	double Clamp(double Value) const
	{
		if (Value < Min)
		{
			return Min;
		}
		if (Value > Max)
		{
			return Max;
		}
		return Value;
	}

	double Min, Max;
};
