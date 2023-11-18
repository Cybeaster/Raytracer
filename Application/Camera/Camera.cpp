//
// Created by Cybea on 12/11/2023.
//
#include "../../Utils/Math.h"
#include "../../Objects/Sphere.h"

#include "Camera.h"

#include "../../Objects/Materials/Material.h"

#include <fstream>
#include <iostream>
#include <thread>


void OCamera::Init()
{
	using namespace Utils::Math;

	// Camera basic parameters
	CameraCenter = LookFrom;

	// Camera coordinate system
	CameraW = Normalize(LookFrom - LookAt);
	CameraU = Normalize(Cross(Up, CameraW));
	CameraV = Cross(CameraW, CameraU);

	const auto theta = Utils::Math::DegreesToRaians(VFov);
	const auto h = tan(theta / 2);

	//Viewport parameters
	ViewportHeight = 2.0 * h * FocusDist;
	ImageHeight = (ImageWidth / AspectRatio);
	ImageHeight = (ImageHeight < 1) ? 1 : ImageHeight;
	ViewportWidth = (static_cast<double>(ImageWidth) / ImageHeight) * ViewportHeight;

	//Delta vectors for each pixel
	const auto viewportU = ViewportWidth * CameraU;
	const auto viewportV = ViewportHeight * -CameraV;

	PixelDeltaU = viewportU / ImageWidth;
	PixelDeltaV = viewportV / ImageHeight;

	//Upper left pixel
	ViewportUpperLeft =
		GetCameraCenter() - (FocusDist * CameraW) - (viewportU / 2) - (viewportV / 2);
	PixelZeroLoc = ViewportUpperLeft + 0.5 * (PixelDeltaU + PixelDeltaV);

	const auto defocuseRadius = FocusDist * tan(DegreesToRaians(DefocusAngle / 2));
	DefocusDiskU = defocuseRadius * CameraU;
	DefocusDiskV = defocuseRadius * CameraV;
}

SRay OCamera::GetRay(float U, float V) const
{
	//Get a random sampled camera ray for a pixel specified

	auto pixelCenter = PixelZeroLoc + (U * PixelDeltaU) + (V * PixelDeltaV);
	auto pixelSample = pixelCenter + PixelSampleSquare();

	auto rayOrigin = (DefocusAngle <= 0) ? GetCameraCenter() : DefocusDiskSample();
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

SColor OCamera::RayColor(const SRay& Ray, const IHittable& World)
{
	return RayColor(Ray, World, MaxDepth);
}

SVec3 OCamera::PixelSampleSquare() const
{
	auto px = Utils::Math::Random() - 0.5;
	auto py = Utils::Math::Random() - 0.5;

	return (px * PixelDeltaU) + (py * PixelDeltaV);
}

SVec3 OCamera::DefocusDiskSample() const
{
	const auto [a] = Utils::Math::RandomUnitInDisk();
	return GetCameraCenter() + (DefocusDiskU * a[0]) + (DefocusDiskV * a[1]);
}


