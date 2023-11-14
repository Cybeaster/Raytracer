#pragma once
#include "../Types/Hittable.h"
#include "../Types/Math.h"

struct SHitRecord;
struct ORay;

class OSphere : public IHittable
{
public:
	OSphere(SVec3 C, float R)
		: Center(C)
		, Radius(R)
	{
	}

	bool Hit(const ORay& Ray, float TMin, float TMax, SHitRecord& OutHitRecord) const;

private:
	SVec3 Center;
	float Radius;
};


