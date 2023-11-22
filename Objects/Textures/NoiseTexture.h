#pragma once
#include "Texture.h"
#include "../../Utils/Algorithms/Perlin.h"

class ONoiseTexture : public ITexture
{
public:
	ONoiseTexture() = default;

	ONoiseTexture(double SC)
		: Scale(SC)
	{
	}

	SColor Value(float U, float V, const SVec3& Point) const override
	{
		auto scale = Scale * Point;
		return SColor(1, 1, 1) * 0.5 * (1 + sin(Utils::Math::GetZ(scale) + 10 * Noise.Turbulence(scale)));
	}

private:
	OPerlin Noise;
	double Scale = 1.0;
};
