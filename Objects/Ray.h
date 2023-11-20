#pragma once

#include "../Types/Math.h"

class SRay
{
public:
	SRay() = default;

	SRay(const SRay&) = default;

	SRay(const SVec3& Origin, const SVec3& Dir)
		: Origin(Origin)
		, Direction(Dir)
		, Time(0.0)
	{
	}

	SRay(const SVec3& Origin, const SVec3& Dir, double T)
		: Origin(Origin)
		, Direction(Dir)
		, Time(T)
	{
	}

	SVec3 GetOrigin() const { return Origin; }
	double GetTime() const { return Time; }
	SVec3 GetDirection() const { return Direction; }
	SVec3 PointAtParameter(float T) const { return Origin + T * Direction; }

private:
	SVec3 Origin;
	SVec3 Direction;
	double Time;
};

