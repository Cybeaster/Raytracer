#pragma once
#include "../Types/Hittable.h"
#include "../Types/Math.h"

struct SHitRecord;
class SRay;

class OSphere : public IHittable
{
public:
	OSphere(SVec3 C, float R)
		: Center(C)
		, Radius(R)
	{
	}

	bool Hit(const SRay& Ray, SInterval Interval, SHitRecord& OutHitRecord) const override;

private:
	SVec3 Center;
	float Radius;
};


