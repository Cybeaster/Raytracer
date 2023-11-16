//
// Created by Cybea on 12/11/2023.
//
#include "../../Utils/Math.h"
#include "../../Objects/Sphere.h"

#include "Camera.h"

#include "../../Objects/Materials/Material.h"

#include <fstream>
#include <iostream>

void OCamera::Render(const IHittable& World)
{
	Init();
	std::ofstream OutFile("image.ppm");

	OutFile << "P3\n" << ImageWidth << ' ' << ImageHeight << "\n255\n";

	for (int j = 0; j < ImageHeight; ++j)
	{
		std::clog << "\rScanlines remaining: " << ImageHeight - j << ' ' << std::flush;
		for (int i = 0; i < ImageWidth; ++i)
		{
			SColor pixelColor{ 0, 0, 0 };
			for (int32_t sample = 0; sample < SamplesPerPixel; ++sample)
			{
				SRay ray = GetRay(i, j);
				pixelColor += RayColor(ray, World, MaxDepth);
			}
			Draw(OutFile, pixelColor);
		}
	}
	OutFile.close();
	std::clog << "\nDone.\n";
}

void OCamera::Init()
{
	ImageHeight = (ImageWidth / AspectRatio);
	ImageHeight = (ImageHeight < 1) ? 1 : ImageHeight;

	ViewportWidth = (static_cast<double>(ImageWidth) / ImageHeight) * ViewportHeight;

	const auto viewportU = SVec3{ ViewportWidth, 0, 0 };
	const auto viewportV = SVec3{ 0, -ViewportHeight, 0 };

	PixelDeltaU = viewportU / ImageWidth;
	PixelDeltaV = viewportV / ImageHeight;

	ViewportUpperLeft =
		GetCameraCenter() - SVec3{ 0, 0, GetFocalLength() } - (viewportU / 2) - (viewportV / 2);
	PixelZeroLoc = ViewportUpperLeft + 0.5 * (PixelDeltaU + PixelDeltaV);
}

SRay OCamera::GetRay(float U, float V) const
{
	//Get a random sampled camera ray for a pixel specified

	auto pixelCenter = PixelZeroLoc + (U * PixelDeltaU) + (V * PixelDeltaV);
	auto pixelSample = pixelCenter + PixelSampleSquare();

	auto rayOrigin = GetCameraCenter();
	auto rayDir = pixelSample - rayOrigin;

	return SRay{ rayOrigin, rayDir };
}

SColor OCamera::RayColor(const SRay& Ray, const IHittable& World, uint32_t Depth)
{
	using namespace Utils::Math;

	if (Depth == 0)
	{
		return SColor{ 0, 0, 0 };
	}

	if (SHitRecord hitRecord; World.Hit(Ray, { 0.001, INFINITY }, hitRecord))
	{
		SRay scattered;
		SColor attenuation;
		if (hitRecord.Material->Scatter(Ray, hitRecord, attenuation, scattered))
		{
			return RayColor(scattered, World, Depth - 1) * attenuation;
		}
		return SColor{ 0, 0, 0 };
	}

	auto [a] = Normalize(Ray.GetDirection());
	auto res = 0.5f * (a[1] + 1.0f);
	return Lerp(res, SColor{ 1.0f, 1.0f, 1.0f }, SColor{ 0.5f, 0.7f, 1.0f });
}

SVec3 OCamera::PixelSampleSquare() const
{
	auto px = Utils::Math::RandomDouble() - 0.5;
	auto py = Utils::Math::RandomDouble() - 0.5;

	return (px * PixelDeltaU) + (py * PixelDeltaV);
}

void OCamera::Draw(std::ostream& Out, SColor Color)
{
	Color *= 1.f / SamplesPerPixel;
	Color = Utils::Math::LinearToGamma(Color);
	static const SInterval intensity(0.0, 0.999);

	Out << static_cast<int>(256 * intensity.Clamp(Color.R)) << ' '
		<< static_cast<int>(256 * intensity.Clamp(Color.G)) << ' '
		<< static_cast<int>(256 * intensity.Clamp(Color.B)) << '\n';
}

