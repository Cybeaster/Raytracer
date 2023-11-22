#include "Sphere.h"
#include "../../Ray.h"
#include "../Hittable.h"

OSphere::OSphere(SVec3 C, float R, std::shared_ptr<IMaterial> M)
	: Center(C)
	, Radius(R)
	, Material(M)
	, IsMoving(false)
{
	auto rVec = SVec3{ Radius, Radius, Radius };
	BoundingBox = SAABB(Center - rVec, Center + rVec);
}

OSphere::OSphere(const SVec3& Point1, const SVec3& Point2, double R, std::shared_ptr<IMaterial> M)
	: Center(Point1)
	, Radius(R)
	, Material(M)
	, IsMoving(true)
{
	auto rVec = SVec3{ Radius, Radius, Radius };
	auto box1 = SAABB(Point1 - rVec, Point1 + rVec);
	auto box2 = SAABB(Point2 - rVec, Point2 + rVec);
	BoundingBox = SAABB(box1, box2);

	MoveVector = Point2 - Point1;
}

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

	auto [u,v] = GetUV(outwardNormal);
	OutHitRecord.U = u;
	OutHitRecord.V = v;

	return true;
}

SVec3 OSphere::GetPositionAtTime(const double Time) const
{
	return MoveVector * Time + Center;
}

SAABB OSphere::GetBoundingBox() const
{
	return BoundingBox;
}

tuple<double, double> OSphere::GetUV(const SVec3& Point)
{
	using namespace Utils::Math;
	// p: a given point on the sphere of radius one, centered at the origin.
	// u: returned value [0,1] of angle around the Y axis from X=-1.
	// v: returned value [0,1] of angle from Y=-1 to Y=+1.
	//     <1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>
	//     <0 1 0> yields <0.50 1.00>       < 0 -1  0> yields <0.50 0.00>
	//     <0 0 1> yields <0.25 0.50>       < 0  0 -1> yields <0.75 0.50>

	const auto theta = acos(-GetY(Point));
	const auto phi = atan2(-GetZ(Point), GetX(Point)) + PI;

	return { phi / (2 * PI), theta / PI };
}
