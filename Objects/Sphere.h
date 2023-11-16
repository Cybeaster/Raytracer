#pragma once
#include "Hittable.h"
#include "../Types/Math.h"

struct SHitRecord;
class SRay;

class OSphere : public IHittable
{
public:
	OSphere(SVec3 C, float R, std::shared_ptr<IMaterial> M)
		: Center(C)
		, Radius(R)
		, Material(M)
	{
	}

	bool Hit(const SRay& Ray, SInterval Interval, SHitRecord& OutHitRecord) const override;

private:
	SVec3 Center;
	float Radius;
	std::shared_ptr<IMaterial> Material;
};


