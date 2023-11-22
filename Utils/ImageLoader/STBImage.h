#pragma once
#include <cstdint>


#include "../../Types/Types.h"

#include <iostream>


class OSTBImage
{
public:
	OSTBImage()
		: Data(nullptr)
		, ImageWidth(0)
		, ImageHeight(0)
		, BytesPerScanline(0)
	{
	}

	explicit OSTBImage(const char* ImageFileName);

	~OSTBImage();

	bool Load(const string& FileName);
	int32_t GetWidth() const;
	int32_t GetHeight() const;
	const uint8_t* GetPixelData(int32_t X, int32_t Y) const;

private:
	static int32_t Clamp(const int32_t X, const int32_t Low, const int32_t High);

	const int32_t BytesPerPixel = 3;
	uint8_t* Data;
	int32_t ImageWidth, ImageHeight;
	int32_t BytesPerScanline;
};


