#pragma once
#include "Texture.h"
#include "../../Types/Interval/Interval.h"
#include "../../Utils/ImageLoader/STBImage.h"


class OImageTexture : public ITexture
{
public:
	OImageTexture(const char* ImageFileName)
		: Image(ImageFileName)
	{
	}

	SColor Value(float U, float V, const SVec3& Point) const override
	{
		if (Image.GetHeight() <= 0)
		{
			return SColor(0, 1, 1); // Debugging aid
		}

		U = SInterval(0, 1).Clamp(U);
		V = 1.0 - SInterval(0, 1).Clamp(V); //Flip V to image coords

		auto i = static_cast<int32_t>(U * Image.GetWidth());
		auto j = static_cast<int32_t>(V * Image.GetHeight());

		auto pixel = Image.GetPixelData(i, j);
		auto colorScale = 1.0 / 255.0;

		return SColor(colorScale * pixel[0], colorScale * pixel[1], colorScale * pixel[2]);
	}

private:
	OSTBImage Image;
};
