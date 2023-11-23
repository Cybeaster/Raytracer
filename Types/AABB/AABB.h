#pragma once
#include "../../Objects/Ray.h"
#include "../../Utils/Math.h"
#include "../Interval/Interval.h"


class SRay;

struct SAABB
{
public:
	SAABB() = default;

	SAABB(const SInterval& iX, const SInterval& iY, const SInterval& iZ);
	SAABB(const SVec3& A, const SVec3& B);
	SAABB(const SAABB& A, const SAABB& B);

	const SInterval& Axis(const int32_t N) const;
	bool Hit(const SRay& Ray, SInterval Interval) const;
	SAABB Pad();

	SInterval X, Y, Z;
};

inline SAABB operator+(const SAABB& Box, const SVec3& Offset)
{
	using namespace Utils::Math;
	return SAABB(Box.X + GetX(Offset), Box.Y + GetY(Offset), Box.Z + GetZ(Offset));
}

inline SAABB operator+(const SVec3& Offset, const SAABB& Box)
{
	return Box + Offset;
}


