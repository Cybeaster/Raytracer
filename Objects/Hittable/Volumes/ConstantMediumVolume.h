#pragma once
#include "../Hittable.h"
#include "../../../Types/Logger.h"
#include "../../Textures/Texture.h"
#include "../../Materials/Isotropic.h"

class OConstantMediumVolume : public IHittable
{
public:
	OConstantMediumVolume(const shared_ptr<IHittable>& _Boundary, const double _Density, shared_ptr<ITexture> _PhaseFunction);

	OConstantMediumVolume(const shared_ptr<IHittable>& _Boundary, const double _Density, const SVec3& Color);

	bool Hit(const SRay& Ray, SInterval Interval, SHitRecord& HitRecord) const override;

	SAABB GetBoundingBox() const override
	{
		return Boundary->GetBoundingBox();
	}

private:
	shared_ptr<IHittable> Boundary;
	double NegativeInvertedDensity;
	shared_ptr<IMaterial> PhaseFunction;
};
