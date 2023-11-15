#include "Sphere.h"
#include "../Ray/SRay.h"
#include "../Types/Hittable.h"

bool OSphere::Hit(const SRay& Ray, SInterval Interval, SHitRecord& OutHitRecord) const
{
	using namespace Utils::Math;

	auto oc = Ray.GetOrigin() - Center;
	auto a = LengthSquared(Ray.GetDirection());
	auto halfB = Dot(oc, Ray.GetDirection());
	auto c = LengthSquared(oc) - Squared(Radius);

	auto discriminant = Squared(halfB) - a * c;
	if (discriminant < 0)
	{
		return false;
	}

	auto sqrtd = sqrt(discriminant);

	// Find the nearest root that lies in the acceptable range
	auto root = (-halfB - sqrtd) / a;
	if (!Interval.Contains(root, false))
	{
		root = (-halfB + sqrtd) / a;
		if (Interval.Contains(root, false))
		{
			return false;
		}
	}

	// We have a valid intersection
	OutHitRecord.T = root;
	OutHitRecord.Point = Ray.PointAtParameter(OutHitRecord.T);
	const auto outwardNormal = (OutHitRecord.Point - Center) / Radius;
	OutHitRecord.SetFaceNormal(Ray, outwardNormal);

	return true;
}
