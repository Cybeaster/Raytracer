#include "HittableList.h"

#include "../Quad/Quad.h"

void OHittableList::Add(const std::shared_ptr<IHittable>& Object)
{
	Objects.push_back(Object);
	BoundingBox = SAABB(BoundingBox, Object->GetBoundingBox());
}

void OHittableList::Clear()
{
	Objects.clear();
}

bool OHittableList::Hit(const SRay& Ray, SInterval Interval, SHitRecord& OutHitRecord) const
{
	bool HitAnything = false;
	auto ClosestSoFar = Interval.Max;

	SHitRecord TempHitRecord;
	for (const auto& Object : Objects)
	{
		if (Object->Hit(Ray, { Interval.Min, ClosestSoFar }, TempHitRecord))
		{
			HitAnything = true;
			ClosestSoFar = TempHitRecord.T;
			OutHitRecord = TempHitRecord;
		}
	}

	return HitAnything;
}

SAABB OHittableList::GetBoundingBox() const
{
	return BoundingBox;
}

shared_ptr<OHittableList> OHittableList::CreateBox(const SVec3& A, const SVec3& B, const shared_ptr<IMaterial>& Material)
{
	using namespace Utils::Math;
	//Return a box with 6 sides
	auto sides = make_shared<OHittableList>();
	auto min = SVec3{ fmin(GetX(A), GetX(B)), fmin(GetY(A), GetY(B)), fmin(GetZ(A), GetZ(B)) };
	auto max = SVec3{ fmax(GetX(A), GetX(B)), fmax(GetY(A), GetY(B)), fmax(GetZ(A), GetZ(B)) };

	auto dx = SVec3{ GetX(max) - GetX(min), 0, 0 };
	auto dy = SVec3{ 0, GetY(max) - GetY(min), 0 };
	auto dz = SVec3{ 0, 0, GetZ(max) - GetZ(min) };

	sides->Add(make_shared<OQuad>(SVec3{ GetX(min), GetY(min), GetZ(max) }, dx, dy, Material)); //front
	sides->Add(make_shared<OQuad>(SVec3{ GetX(max), GetY(min), GetZ(max) }, -dz, dy, Material)); //right
	sides->Add(make_shared<OQuad>(SVec3{ GetX(max), GetY(min), GetZ(min) }, -dx, dy, Material)); //back
	sides->Add(make_shared<OQuad>(SVec3{ GetX(min), GetY(min), GetZ(min) }, dz, dy, Material)); //left
	sides->Add(make_shared<OQuad>(SVec3{ GetX(min), GetY(max), GetZ(max) }, dx, -dz, Material)); //top
	sides->Add(make_shared<OQuad>(SVec3{ GetX(min), GetY(min), GetZ(min) }, dx, dz, Material)); //bottom
	return sides;
}
