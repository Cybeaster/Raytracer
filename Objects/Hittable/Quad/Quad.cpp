#include "Quad.h"

OQuad::OQuad(const SVec3& _Q, const SVec3& _U, const SVec3& _V, const shared_ptr<IMaterial>& _M)
	: Origin(_Q)
	, U(_U)
	, V(_V)
	, Material(_M)
{
	auto n = Utils::Math::Cross(U, V);
	Normal = Utils::Math::Normalize(n);
	Offset = Utils::Math::Dot(Normal, Origin);
	W = n / Utils::Math::Dot(n, n);
	SetBoundingBox();
}

void OQuad::SetBoundingBox()
{
	BoundingBox = SAABB(Origin, Origin + U + V).Pad();
}

SAABB OQuad::GetBoundingBox() const
{
	return BoundingBox;
}

bool OQuad::Hit(const SRay& Ray, SInterval Interval, SHitRecord& HitRecord) const
{
	using namespace Utils::Math;
	const auto denom = Dot(Normal, Ray.GetDirection());
	if (fabs(denom) < 1e-8)
	{
		return false;
	}

	//If outside of the ray interval
	const auto t = (Offset - Dot(Normal, Ray.GetOrigin())) / denom;
	if (!Interval.Contains(t))
	{
		return false;
	}

	//Determine if the intersection point is within the quad
	const auto intersection = Ray.PointAtParameter(t);
	const SVec3 planar = intersection - Origin;
	const auto alpha = Dot(W, Cross(planar, V));
	const auto beta = Dot(W, Cross(U, planar));

	//Check barymetric coordinates
	if (!IsInterior(alpha, beta, HitRecord))
	{
		return false;
	}

	HitRecord.T = t;
	HitRecord.Material = Material;
	HitRecord.Point = intersection;
	HitRecord.SetFaceNormal(Ray, Normal);
	return true;
}

bool OQuad::IsInterior(double A, double B, SHitRecord& HitRecord) const
{
	// Given the hit point in plane coordinates, return false if it is outside the
	// primitive, otherwise set the hit record UV coordinates and return true.

	if ((A < 0) || (1 < A) || (B < 0) || (1 < B))
	{
		return false;
	}

	HitRecord.U = A;
	HitRecord.V = B;
	return true;
}
