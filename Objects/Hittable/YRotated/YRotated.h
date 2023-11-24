#pragma once
#include "../Hittable.h"
#include "../../../Types/Types.h"


class ORotatedY : public IHittable
{
public:
	ORotatedY(shared_ptr<IHittable> Object, const double Angle);
	bool Hit(const SRay& Ray, SInterval Interval, SHitRecord& HitRecord) const override;

	SAABB GetBoundingBox() const override
	{
		return BoundingBox;
	}

private:
	shared_ptr<IHittable> RotatedObject;
	double SinTheta;
	double CosTheta;
	SAABB BoundingBox;
};
