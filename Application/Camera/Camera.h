#pragma once

#include "../../Types/Color.h"
#include "../../Types/Math.h"

class SRay;

class OCamera
{
public:
	void Render(const IHittable& World);

	void Init();
	SRay GetRay(float U, float V) const;
	SColor RayColor(const SRay& Ray, const class IHittable& World, uint32_t Depth);

	auto GetFocalLength() const
	{
		return FocalLength;
	}

	auto GetCameraCenter() const
	{
		return CameraCenter;
	}


	inline SVec3 PixelSampleSquare() const;

private:
	void Draw(std::ostream& Out, SColor Color);

	float FocalLength = 0.5f;
	SVec3 CameraCenter = { 0, 0, 0 };

	int32_t ImageWidth = 800;
	int32_t ImageHeight = 1;

	float ViewportWidth = 2;
	float ViewportHeight = 1;

	// Maximum number of ray bounces into scene
	uint32_t MaxDepth = 10;

	SVec3 ViewportUpperLeft{};
	SVec3 PixelZeroLoc{};

	SVec3 PixelDeltaU{};
	SVec3 PixelDeltaV{};

	// Count of random samples for each pixel
	int32_t SamplesPerPixel = 10;

	double AspectRatio = 16.0 / 9.0;
};


