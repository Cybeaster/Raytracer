//
// Created by Cybea on 12/11/2023.
//
#include "../../Utils/Math.h"
#include "../../Objects/Hittable/Sphere/Sphere.h"

#include "Camera.h"

#include "../../Objects/Materials/Material.h"

#include <fstream>
#include <iostream>
#include <thread>


void OCamera::Init(ECameraPresets Preset)
{
	using namespace Utils::Math;
	SetCameraPreset(Preset);

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

	const auto pixelCenter = PixelZeroLoc + (U * PixelDeltaU) + (V * PixelDeltaV);
	const auto pixelSample = pixelCenter + PixelSampleSquare();

	const auto rayOrigin = (DefocusAngle <= 0) ? GetCameraCenter() : DefocusDiskSample();
	const auto rayDir = pixelSample - rayOrigin;

	auto rayTime = Utils::Math::Random();

	return SRay{ rayOrigin, rayDir, rayTime };
}

SColor OCamera::RayColor(const SRay& Ray, const IHittable& World, uint32_t Depth)
{
	using namespace Utils::Math;

	if (Depth == 0)
	{
		return SColor{ 0, 0, 0 };
	}

	SHitRecord hitRecord;
	if (!World.Hit(Ray, { 0.001, INFINITY }, hitRecord))
	{
		return BackgroundColor;
	}

	SRay scattered;
	SColor attenuation;
	const SColor emitted = hitRecord.Material->Emitted(hitRecord.U, hitRecord.V, hitRecord.Point);

	if (!hitRecord.Material->Scatter(Ray, hitRecord, attenuation, scattered))
	{
		return emitted;
	}

	const SColor colorFromScatter = attenuation * RayColor(scattered, World, Depth - 1);
	return emitted + colorFromScatter;
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

void OCamera::SetCameraPreset(ECameraPresets Preset)
{
	switch (Preset)
	{
	case ECameraPresets::Low:
		ImageWidth = 200;
		SamplesPerPixel = 50;
		MaxDepth = 5;
		break;
	case ECameraPresets::Medium:
		ImageWidth = 400;
		SamplesPerPixel = 100;
		MaxDepth = 25;
		break;
	case ECameraPresets::High:
		ImageWidth = 600;
		SamplesPerPixel = 250;
		MaxDepth = 50;
		break;
	case ECameraPresets::Ultra:
		ImageWidth = 700;
		SamplesPerPixel = 500;
		MaxDepth = 50;
		break;
	case ECameraPresets::Insane:
		ImageWidth = 800;
		SamplesPerPixel = 10000;
		MaxDepth = 40;
		break;
	}
}
