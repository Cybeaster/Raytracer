#pragma once
#include "Material.h"
#include "../../Types/Color.h"
#include "../SRay.h"


class OMetal : public IMaterial
{
public:
	OMetal(const SColor& Color, double FuzzP)
		: Albedo(Color)
		, Fuzz(FuzzP < 1 ? FuzzP : 1)
	{
	}

	bool Scatter(const SRay& Ray, const SHitRecord& HitRecord, SColor& OutAttenuation, SRay& OutScattered) const override
	{
		/*Fully reflect the ray and return the albedo*/
		const SVec3 reflected = Utils::Math::Reflect(Utils::Math::Normalize(Ray.GetDirection()), HitRecord.Normal);
		OutScattered = SRay(HitRecord.Point, reflected + Fuzz * Utils::Math::RandomUnitVector());
		OutAttenuation = Albedo;
		return (Utils::Math::Dot(OutScattered.GetDirection(), HitRecord.Normal) > 0);
	}

private:
	SColor Albedo;
	double Fuzz;
};
