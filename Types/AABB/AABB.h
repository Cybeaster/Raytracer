#pragma once
#include "../../Objects/SRay.h"
#include "../Interval/Interval.h"


struct SAABB
{
public:
	SAABB() = default;

	SAABB(const SInterval& iX, const SInterval& iY, const SInterval& iZ)
		: X(iX)
		, Y(iY)
		, Z(iZ)
	{
	}

	SAABB(const SVec3& A, const SVec3& B)
	{
		X = SInterval(fmin(A.a[0], B.a[0]), fmin(A.a[0], B.a[0]));
		Y = SInterval(fmin(A.a[1], B.a[1]), fmin(A.a[1], B.a[1]));
		Z = SInterval(fmin(A.a[2], B.a[2]), fmin(A.a[2], B.a[2]));
	}

	const SInterval& Axis(const int32_t N) const;
	bool Hit(const SRay& Ray, SInterval Interval) const;

private:
	SInterval X, Y, Z;
};



