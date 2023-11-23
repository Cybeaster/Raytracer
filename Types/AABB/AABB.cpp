//
// Created by Cybea on 21/11/2023.
//

#include "AABB.h"


SAABB::SAABB(const SInterval& iX, const SInterval& iY, const SInterval& iZ)
	: X(iX)
	, Y(iY)
	, Z(iZ)
{
}

SAABB::SAABB(const SVec3& A, const SVec3& B)
	: X(SInterval(fmin(A.a[0], B.a[0]), fmax(A.a[0], B.a[0])))
	, Y(SInterval(fmin(A.a[1], B.a[1]), fmax(A.a[1], B.a[1])))
	, Z(SInterval(fmin(A.a[2], B.a[2]), fmax(A.a[2], B.a[2])))
{
}

SAABB::SAABB(const SAABB& A, const SAABB& B)
	: X(SInterval(A.X, B.X))
	, Y(SInterval(A.Y, B.Y))
	, Z(SInterval(A.Z, B.Z))
{
}

const SInterval& SAABB::Axis(const int32_t N) const
{
	switch (N)
	{
	case 1:
		return Y;
	case 2:
		return Z;
	default:
		return X;
	}
}

bool SAABB::Hit(const SRay& Ray, SInterval Interval) const
{
	for (uint32_t i = 0; i < 3; ++i)
	{
		// Calculate the inverse of the ray's direction for the current axis
		const auto invD = 1 / Ray.GetDirection().a[i];

		// Get the origin component of the ray for the current axis
		const auto origin = Ray.GetOrigin().a[i];

		// Calculate the intersection parameter t0 with the min plane of the axis
		auto t0 = (Axis(i).Min - origin) * invD;

		// Calculate the intersection parameter t1 with the max plane of the axis
		auto t1 = (Axis(i).Max - origin) * invD;

		// If the ray is pointing in the negative direction of the current axis
		if (invD < 0.0f)
		{
			// Swap t0 and t1 to ensure correct ordering (entry and exit points)
			std::swap(t0, t1);
		}

		// If the entry point extends the current interval
		if (t0 > Interval.Min)
		{
			Interval.Min = t0; // Update the minimum value of the interval
		}

		// If the exit point reduces the current interval
		if (t1 < Interval.Max)
		{
			Interval.Max = t1; // Update the maximum value of the interval
		}

		if (Interval.Max <= Interval.Min) // Check if the interval is still valid (Max should be greater than Min)
		{
			return false; // If the interval is invalid, the ray does not intersect within the interval
		}

		/* Not optimized simple implemetation
		 const auto t0 = fmin((Axis(i).Min - Ray.GetOrigin().a[i]) / Ray.GetDirection().a[i],
		                     (Axis(i).Max - Ray.GetOrigin().a[i]) / Ray.GetDirection().a[i]);

		const auto t1 = fmax((Axis(i).Min - Ray.GetOrigin().a[i]) / Ray.GetDirection().a[i],
		                     Axis(i).Max - Ray.GetOrigin().a[i] / Ray.GetDirection().a[i]);

		Interval.Min = fmax(t0, Interval.Min);
		Interval.Max = fmin(t1, Interval.Max);

		if (Interval.Max <= Interval.Min)
		{
			return false;
		}*/
	}
	return true;
}

SAABB SAABB::Pad()
{
	const double delta = 1e-4;
	const SInterval newX = (X.Size() >= delta ? X : X.Expand(delta));
	const SInterval newY = (Y.Size() >= delta ? Y : Y.Expand(delta));
	const SInterval newZ = (Z.Size() >= delta ? Z : Z.Expand(delta));
	return SAABB(newX, newY, newZ);
}
