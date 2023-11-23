#pragma once
#include "Serializable.h"
#include "../../Types/Color.h"

#include <cstdint>
#include <fstream>


class OPPMSerializer final : public ISerializer
{
public:
	explicit OPPMSerializer(const int32_t Pixels)
		: SamplesPerPixel(Pixels)
	{
	}

	OPPMSerializer() = default;

	virtual void Init(const int32_t Samples) override
	{
		SamplesPerPixel = Samples;
	}

	void Serialize(const SFrameBuffer& Buffer, const string& FileName) override;

private:
	int32_t SamplesPerPixel;
};
