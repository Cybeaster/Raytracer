# Raytracer

Asynchronous CPU raytracer, supporting diffrenet materials (Glass, Metal, Lambertian) and post process effects (Blur).

Has a simple profiler inside to measure and compare execution time.

### Overview

First, the scene is created and setup in `OApplication` class.
We may want to serialize the scene to different files or use different renderers and raytracers. For this
purpose `OApplication` has pointers to all of them.

`ORenderer` is responsible for rendering the scene into a frame buffer. It receives and raytracer to build the scene.
Once the scene is build it gives the buffer to onto the next stage - serialization (`OPPMSerializer` in this case).

Each type of the material inherits from the base class SMaterial and implements the virtual function IMaterial::Scatter.

```cpp 
virtual bool Scatter(const SRay& Ray, const SHitRecord& HitRecord, SColor& OutAttenuation, SRay& OutScattered) const = 0;
```

The image is rendered and then serialized to a file in PPM format.

```cpp
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
```

## Examples
![ManySpheres](https://github.com/Cybeaster/Raytracer/blob/ExtendedScene/Examples/MaySpheresCenter.png)
![Blur](https://github.com/Cybeaster/Raytracer/blob/develop/Examples/FocusGlassLabMetal.png)
![Metal](https://github.com/Cybeaster/Raytracer/blob/develop/Examples/GlassLabertianFuzzedMetal.png)

