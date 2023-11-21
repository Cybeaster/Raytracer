#pragma once
#include "Material.h"
#include "../../Types/Color.h"
#include "../Hittable.h"
#include "..\Ray.h"

class OLambertian : public IMaterial
{
public:
	OLambertian(const SColor& A)
		: Albedo(A)
	{
	}

	bool Scatter(const SRay& Ray, const SHitRecord& HitRecord, SColor& OutAttenuation, SRay& OutScattered) const override
	{
		auto scatterDirection = HitRecord.Normal + Utils::Math::RandomUnitVector();

		if (Utils::Math::IsNearZero(scatterDirection))
		{
			scatterDirection = HitRecord.Normal;
		}

		OutScattered = SRay(HitRecord.Point, scatterDirection, Ray.GetTime());
		OutAttenuation = Albedo;
		return true;
	}

private:
	SColor Albedo;
};
