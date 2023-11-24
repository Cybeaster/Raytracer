#include "ConstantMediumVolume.h"

OConstantMediumVolume::OConstantMediumVolume(const shared_ptr<IHittable>& _Boundary, const double _Density, shared_ptr<ITexture> _PhaseFunction)
	: Boundary(_Boundary)
	, NegativeInvertedDensity(-1.0 / _Density)
	, PhaseFunction(make_shared<OIsotropic>(_PhaseFunction))
{
}

OConstantMediumVolume::OConstantMediumVolume(const shared_ptr<IHittable>& _Boundary, const double _Density, const SVec3& Color)
	: Boundary(_Boundary)
	, NegativeInvertedDensity(-1.0 / _Density)
	, PhaseFunction(make_shared<OIsotropic>(Color))
{
}

bool OConstantMediumVolume::Hit(const SRay& Ray, SInterval Interval, SHitRecord& HitRecord) const
{
	using namespace Utils::Math;

	SHitRecord hitRecord1, hitRecord2;

	//Check the entry and exit points of the ray in the medium.
	// First, check if the ray hits the boundary of the medium at all.
	if (!Boundary->Hit(Ray, SInterval::Full, hitRecord1))
	{
		return false;
	}

	// Check if the ray exits the boundary after the first hit.
	// The slight offset (1e-4) is to avoid precision errors.
	if (!Boundary->Hit(Ray, SInterval(hitRecord1.T + 1e-4, INFINITY), hitRecord2))
	{
		return false;
	}

	DLOG(Log, "\n ray t_min {} t_max {}", hitRecord1.T, hitRecord2.T);

	// Clamp the hit intervals to the provided interval.
	if (hitRecord1.T < Interval.Min)
	{
		hitRecord1.T = Interval.Min;
	}
	if (hitRecord2.T > Interval.Max)
	{
		hitRecord2.T = Interval.Max;
	}

	// If the intervals are invalid or inverted, there is no hit.
	if (hitRecord1.T > hitRecord2.T)
	{
		return false;
	}

	// Ensure the first hit point is not behind the ray origin.
	if (hitRecord1.T < 0)
	{
		hitRecord1.T = 0;
	}

	// Calculate the length of the ray's path inside the medium.
	const auto rayLenght = Length(Ray.GetDirection());
	const auto distanceInsideBoundary = (hitRecord2.T - hitRecord1.T) * rayLenght;

	// Calculate a random hit distance based on the medium's density.
	// NegativeInvertedDensity is used to control the density of the medium.
	const auto hitDistance = NegativeInvertedDensity * log(Random());

	// If the random hit distance is larger than the actual distance inside the medium, there is no hit.
	if (hitDistance > distanceInsideBoundary)
	{
		return false;
	}

	// Calculate the actual hit point on the ray.
	HitRecord.T = hitRecord1.T + hitDistance / rayLenght;
	HitRecord.Point = Ray.PointAtParameter(HitRecord.T);

	if (Random() < 1e-5)
	{
		DLOG(Log, "Hit distance: {} \n, hit record t: {} \n, hit record point: {}", hitDistance, HitRecord.T, TO_STRING(HitRecord.Point));
	}

	// For a volumetric medium, the normal and whether the face is front or back are arbitrary.
	HitRecord.Normal = { 1, 0, 0 }; // arbitrary
	HitRecord.FrontFace = true; // also arbitrary

	// Set the material of the hit record to be the phase function of the medium.
	HitRecord.Material = PhaseFunction;

	return true;
}
