#pragma once
#include "Renderer.h"
#include "../Camera/Raytracer.h"

#include <thread>


class IHittable;

class OAsyncRenderer final : public ORenderer
{
public:
	OAsyncRenderer(const uint32_t FBWidth, const uint32_t FBHeight)
		: ORenderer(FBWidth, FBHeight)
	{
	}

	virtual const SFrameBuffer& Draw(const std::shared_ptr<IRaytracer>& Raytracer, const IHittable& World, int32_t SamplesPerPixel) override
	{
		const auto numThreads = std::thread::hardware_concurrency();
		const auto heightStep = (GetBuffer().Height / numThreads) + 1;
		PROFILE();

		for (int idx = 0; idx < numThreads; ++idx)
		{
			auto startRow = idx * heightStep;
			auto endRow = (idx + 1) * heightStep;
			Threads.emplace_back([&](const uint32_t StartRow, const uint32_t EndRow, const int32_t ThreadID)
			                     {
				                     std::printf("Worker: %i begins!\n", ThreadID);
				                     for (auto i = StartRow; i < EndRow; ++i)
				                     {
					                     for (auto j = 0; j < GetBuffer().Width; ++j)
					                     {
						                     SColor pixelColor{ 0, 0, 0 };
						                     for (int32_t sample = 0; sample < SamplesPerPixel; ++sample)
						                     {
							                     SRay ray = Raytracer->GetRay(j, i);
							                     pixelColor += Raytracer->RayColor(ray, World);
						                     }
						                     UpdateBuffer(i, j, pixelColor);
					                     }
					                     const double percentage = 100.0 * static_cast<double>(i - StartRow) / (EndRow - StartRow);
					                     std::printf("Worker %i processed: %f%%!\n", ThreadID, percentage);
				                     }
				                     std::printf("Worker: %i finished!\n", ThreadID);
			                     },
			                     startRow,
			                     endRow,
			                     idx);
		}
		for (auto& thread : Threads)
		{
			thread.join();
		}
		return GetBuffer();
	}

private:
	std::vector<std::thread> Threads;
};
