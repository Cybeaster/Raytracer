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

	void Serialize(const SFrameBuffer& Buffer) override;

private:
	int32_t SamplesPerPixel;
};
