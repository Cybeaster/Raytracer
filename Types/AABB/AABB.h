#pragma once
#include "../../Objects/Ray.h"
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

private:
	SInterval X, Y, Z;
};



