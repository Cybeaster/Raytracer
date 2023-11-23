#pragma once
#include "Renderer.h"
#include "../Camera/Raytracer.h"

#include <iostream>

class OSyncRenderer : public ORenderer
{
public:
	OSyncRenderer(const uint32_t FBWidth, const uint32_t FBHeight)
		: ORenderer(FBWidth, FBHeight)
	{
	}

	OSyncRenderer() = default;

	virtual const SFrameBuffer& Draw(const std::shared_ptr<IRaytracer>& Raytracer, const IHittable& World, int32_t SamplesPerPixel) override
	{
		PROFILE();

		for (int j = 0; j < GetBuffer().Height; ++j)
		{
			for (int i = 0; i < GetBuffer().Width; ++i)
			{
				std::clog << "\rScanlines remaining: " << GetBuffer().Height - j << ' ' << std::flush;

				SColor pixelColor{ 0, 0, 0 };
				for (int32_t sample = 0; sample < SamplesPerPixel; ++sample)
				{
					SRay ray = Raytracer->GetRay(i, j);
					pixelColor += Raytracer->RayColor(ray, World);
				}
				UpdateBuffer(j, i, pixelColor);
			}
		}
		return GetBuffer();
	}
};
