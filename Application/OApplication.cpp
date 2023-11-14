#include <iostream>
#include <fstream>
#include "OApplication.h"

#include "../Objects/Sphere.h"
#include "../Ray/ORay.h"
#include "../Utils/Color.h"
#include "../Types/Types.h"

OApplication::OApplication()
{
	Init();
	Render();
}

void OApplication::Init()
{
	ImageHeight = (ImageWidth / AspectRatio);
	ImageHeight = (ImageHeight < 1) ? 1 : ImageHeight;

	ViewportWidth = (static_cast<double>(ImageWidth) / ImageHeight) * ViewportHeight;

	const auto viewportU = SVec3{ ViewportWidth, 0, 0 };
	const auto viewportV = SVec3{ 0, ViewportHeight, 0 };

	PixelDeltaU = viewportU / ImageWidth;
	PixelDeltaV = viewportV / ImageHeight;

	ViewportUpperLeft =
		Camera.GetCameraCenter() - SVec3{ 0, 0, Camera.GetFocalLength() } - (viewportU / 2) - (viewportV / 2);
	PixelZeroLoc = ViewportUpperLeft + 0.5 * (PixelDeltaU + PixelDeltaV);

	World.Add(make_shared<OSphere>(SVec3{ 0, 0, -1 }, 0.5));
	World.Add(make_shared<OSphere>(SVec3{ 0, -100.5, -1 }, 100));
}

void OApplication::Render()
{
	std::ofstream OutFile("image.ppm");

	OutFile << "P3\n" << ImageWidth << ' ' << ImageHeight << "\n255\n";

	for (int j = 0; j < ImageHeight; ++j)
	{
		std::clog << "\rScanlines remaining: " << ImageHeight - j << ' ' << std::flush;
		for (int i = 0; i < ImageWidth; ++i)
		{
			auto pixelCenter = PixelZeroLoc + (i * PixelDeltaU) + (j * PixelDeltaV);
			auto rayDir = pixelCenter - Camera.GetCameraCenter();

			ORay ray{ Camera.GetCameraCenter(), rayDir };
			SColor pixelColor = Utils::GetColorAt(ray, World);
			Utils::WriteColor(OutFile, pixelColor);
		}
	}
	OutFile.close();
	std::clog << "\nDone.\n";
}
