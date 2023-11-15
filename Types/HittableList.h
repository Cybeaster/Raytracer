#pragma once
#include <memory>
#include <vector>

#include "Hittable.h"


class OHittableList final : public IHittable
{
public:
	OHittableList() = default;

	OHittableList(const std::shared_ptr<IHittable>& Object)
	{
		Objects.push_back(Object);
	}

	void Add(const std::shared_ptr<IHittable>& Object)
	{
		Objects.push_back(Object);
	}

	void Clear()
	{
		Objects.clear();
	}

	bool Hit(const SRay& Ray, SInterval Interval, SHitRecord& OutHitRecord) const override
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

private:
	std::vector<std::shared_ptr<IHittable>> Objects;
};

