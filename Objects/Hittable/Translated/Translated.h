#pragma once
#include "../Hittable.h"
#include "../../../Types/Types.h"


/**
 * \brief Wraps an object and translates it by a given vector
 */
class OTranslated final : public IHittable
{
public:
	OTranslated(const shared_ptr<IHittable>& _Object, const SVec3& _Offset)
		: Object(_Object)
		, Offset(_Offset)
	{
		BoundingBox = Object->GetBoundingBox() + Offset;
	}

	virtual bool Hit(const SRay& Ray, SInterval Interval, SHitRecord& HitRecord) const override
	{
		SRay offsetRay = SRay(Ray.GetOrigin() - Offset, Ray.GetDirection(), Ray.GetTime());

		if (!Object->Hit(offsetRay, Interval, HitRecord))
		{
			return false;
		}

		HitRecord.Point += Offset;
		return true;
	}

	SAABB GetBoundingBox() const override
	{
		return SAABB(Object->GetBoundingBox().GetMin() + Offset, Object->GetBoundingBox().GetMax() + Offset);
	}

private:
	shared_ptr<IHittable> Object;
	SVec3 Offset;
	SAABB BoundingBox;
};
