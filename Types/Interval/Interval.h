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

	SInterval(const SInterval& A, const SInterval& B)
		: Min(fmin(A.Min, B.Min))
		, Max(fmax(A.Max, B.Max))
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

	SInterval Expand(double Delta)
	{
		auto padding = Delta / 2;
		return SInterval(Min - padding, Max + padding);
	}

	double Size() const
	{
		return Max - Min;
	}


	double Min, Max;
};
