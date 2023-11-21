#include "HittableList.h"

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
