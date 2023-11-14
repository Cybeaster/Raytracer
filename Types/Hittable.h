#pragma once
#include "Math.h"
#include "../Utils/Math.h"
#include "../Utils/Exceptions.h"


class ORay;


struct SHitRecord
{
	void SetFaceNormal(const ORay& Ray, const SVec3& OutwardNormal)
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

	virtual bool Hit(const ORay& Ray, float TMin, float TMax, SHitRecord& HitRecord) const = 0;
};
