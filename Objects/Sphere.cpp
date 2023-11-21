#include "Sphere.h"
#include "Ray.h"
#include "Hittable.h"

bool OSphere::Hit(const SRay& Ray, SInterval Interval, SHitRecord& OutHitRecord) const
{
	using namespace Utils::Math;

	SVec3 center = IsMoving ? GetPositionAtTime(Ray.GetTime()) : Center;

	/*Find intersection points*/
	auto oc = Ray.GetOrigin() - center;
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
		if (!Interval.Contains(root, false))
		{
			return false;
		}
	}

	// We have a valid intersection
	OutHitRecord.T = root;
	OutHitRecord.Point = Ray.PointAtParameter(OutHitRecord.T);
	const auto outwardNormal = (OutHitRecord.Point - center) / Radius;
	OutHitRecord.SetFaceNormal(Ray, outwardNormal);
	OutHitRecord.Material = Material;
	return true;
}
