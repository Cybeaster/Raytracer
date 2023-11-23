#pragma once
#include "../../Types/Color.h"

#include <iostream>
#include <memory>
#include <vector>


class IRaytracer;
class IHittable;

struct SFrameBuffer
{
	std::unique_ptr<std::vector<SColor>> Data;
	uint32_t Width = 0;
	uint32_t Height = 0;
};

class ORenderer
{
public:
	virtual ~ORenderer() = default;

	ORenderer(const uint32_t FBWidth, const uint32_t FBHeight)
		: Buffer{ std::make_unique<std::vector<SColor>>(FBWidth * FBHeight), FBWidth, FBHeight }
	{
	}

	ORenderer() = default;

	void Init(const uint32_t FBWidth, const uint32_t FBHeight)
	{
		Buffer = { std::make_unique<std::vector<SColor>>(FBWidth * FBHeight), FBWidth, FBHeight };
	}


	virtual const SFrameBuffer& Draw(const std::shared_ptr<IRaytracer>& Raytracer, const IHittable& World, int32_t SamplesPerPixel) =0;

protected:
	inline void UpdateBuffer(const uint32_t RowPixel, const uint32_t ColumnPixel, const SColor& Color)
	{
		if (Buffer.Data.get() != nullptr && RowPixel < Buffer.Height && ColumnPixel < Buffer.Width)
		{
			const size_t index = ColumnPixel + RowPixel * Buffer.Width;
			(*Buffer.Data.get())[index] = Color;
		}
	}

	SFrameBuffer& GetBuffer()
	{
		return Buffer;
	}

private:
	SFrameBuffer Buffer;
};
