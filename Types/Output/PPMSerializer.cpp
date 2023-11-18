#include "PPMSerializer.h"
#include "../../Application/Renderers/Renderer.h"
#include "../../Types/Interval/Interval.h"
#include "../../Utils/Math.h"

#include <iostream>

inline void OPPMSerializer::Serialize(const SFrameBuffer& Buffer)
{
	std::ofstream outFile("image.ppm");
	outFile << "P3\n" << Buffer.Width << ' ' << Buffer.Height << "\n255\n";
	auto buffer = Buffer.Data.get();
	for (auto color : *buffer)
	{
		color *= 1.f / SamplesPerPixel;
		color = Utils::Math::LinearToGamma(color);
		static const SInterval intensity(0.0, 0.999);

		outFile << static_cast<int>(256 * intensity.Clamp(color.R)) << ' '
			<< static_cast<int>(256 * intensity.Clamp(color.G)) << ' '
			<< static_cast<int>(256 * intensity.Clamp(color.B)) << '\n';
	}

	outFile.close();
	std::clog << "\nDone.\n";
}
