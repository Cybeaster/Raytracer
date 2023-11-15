#pragma once
#include "Interval/Interval.h"
#include "Math.h"
#include "../Utils/Math.h"
#include "../Utils/Exceptions.h"


class SRay;


struct SHitRecord
{
	void SetFaceNormal(const SRay& Ray, const SVec3& OutwardNormal)
	{
		ENSURE(Utils::Math::IsNormalized(OutwardNormal));
		FrontFace = Utils::Math::Dot(Ray.GetDirection(), OutwardNormal) < 0;
		Normal = FrontFace ? OutwardNormal : -OutwardNormal;
	}


	SVec3 Point;
	SVec3 Normal;
	double T;
	bool FrontFace;
};

class IHittable
{
public:
	virtual ~IHittable() = default;

	virtual bool Hit(const SRay& Ray, SInterval Interval, SHitRecord& HitRecord) const = 0;
};
