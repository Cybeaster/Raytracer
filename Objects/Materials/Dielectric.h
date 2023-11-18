#pragma once
#include "Material.h"


class ODielectric : public IMaterial
{
public:
	ODielectric(double RI)
		: RefractionIndex(RI)
	{
	}

	bool Scatter(const SRay& Ray, const SHitRecord& HitRecord, SColor& OutAttenuation, SRay& OutScattered) const override
	{
		OutAttenuation = SColor(1.0, 1.0, 1.0);
		double refractionRatio = HitRecord.FrontFace ? (1.0 / RefractionIndex) : RefractionIndex;
		SVec3 unitDir = Utils::Math::Normalize(Ray.GetDirection());

		double cosTheta = fmin(Utils::Math::Dot(-unitDir, HitRecord.Normal), 1.0);
		double sinTheta = sqrt(1.0 - cosTheta * cosTheta);

		bool cannotRefract = refractionRatio * sinTheta > 1.0;
		SVec3 direction;

		if (cannotRefract || Utils::Math::Reflectance(cosTheta, refractionRatio) > Utils::Math::Random())
		{
			direction = Utils::Math::Reflect(unitDir, HitRecord.Normal);
		}
		else
		{
			direction = Utils::Math::Refract(unitDir, HitRecord.Normal, refractionRatio);
		}

		OutScattered = SRay(HitRecord.Point, direction);
		return true;
	}

private:
	double RefractionIndex;
};
