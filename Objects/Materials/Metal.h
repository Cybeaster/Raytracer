#pragma once
#include "Material.h"
#include "../../Types/Color.h"
#include "../SRay.h"


class OMetal : public IMaterial
{
public:
	OMetal(const SColor& Color)
		: Albedo(Color)
	{
	}

	bool Scatter(const SRay& Ray, const SHitRecord& HitRecord, SColor& OutAttenuation, SRay& OutScattered) const override
	{
		/*Fully reflect the ray and return the albedo*/
		SVec3 reflected = Utils::Math::Reflect(Utils::Math::Normalize(Ray.GetDirection()), HitRecord.Normal);
		OutScattered = SRay(HitRecord.Point, reflected);
		OutAttenuation = Albedo;
		return true;
	}

private:
	SColor Albedo;
};
