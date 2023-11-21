#pragma once

#include "Raytracer.h"
#include "../../Types/Color.h"
#include "../../Types/Math.h"

class SRay;

class OCamera final : public IRaytracer
{
public:
	void Init();
	SRay GetRay(float U, float V) const;

	auto GetFocalLength() const
	{
		return FocalLength;
	}

	auto GetCameraCenter() const
	{
		return CameraCenter;
	}

	SColor RayColor(const SRay& Ray, const IHittable& World);
	SVec3 PixelSampleSquare() const;
	SVec3 DefocusDiskSample() const;

private:
	SColor RayColor(const SRay& Ray, const class IHittable& World, uint32_t Depth);

public:
	float FocalLength = 0.5f;
	SVec3 CameraCenter = { 0, 0, 0 };

	//View parameters
	int32_t ImageWidth = 800;
	int32_t ImageHeight = 1;
	double AspectRatio = 16.0 / 9.0;

	float ViewportWidth = 2;
	float ViewportHeight = 1;

	// Maximum number of ray bounces into scene
	uint32_t MaxDepth = 100;

	SVec3 ViewportUpperLeft{};
	SVec3 PixelZeroLoc{};

	//Delta vectors for each pixel
	SVec3 PixelDeltaU{};
	SVec3 PixelDeltaV{};

	SVec3 CameraV = {}, CameraU = {}, CameraW = {}; // Camera coordinate system
	SVec3 DefocusDiskU{};
	SVec3 DefocusDiskV{};

	// Count of random samples for each pixel
	int32_t SamplesPerPixel = 100;

	// Vertical field of view in degrees
	double VFov = 45;

	// Camera parameters
	SVec3 LookFrom = { -2, 2, 1 };
	SVec3 LookAt = { 0, 0, -1 };
	SVec3 Up = { 0, 1, 0 };

	double DefocusAngle = 0.f;
	double FocusDist = 10.f;
};


