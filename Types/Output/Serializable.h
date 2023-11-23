#pragma once
#include <cstdint>
#include "../Types.h"

class ISerializer
{
public:
	virtual ~ISerializer() = default;
	virtual void Init(int32_t SamplesPerPixel) = 0;
	virtual void Serialize(const struct SFrameBuffer&, const string& FileName) = 0;
};


