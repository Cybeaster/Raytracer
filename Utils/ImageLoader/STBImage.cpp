#include "STBImage.h"

#ifdef _MSC_VER
	#pragma warning (push, 0)
#endif

#define STBI_FAILURE_USERMSG
#define STB_IMAGE_IMPLEMENTATION
#include "../../Externals/Image/stb_image.h"

OSTBImage::OSTBImage(const char* ImageFileName)
{
	// Loads image data from the specified file. If the RTW_IMAGES environment variable is
	// defined, looks only in that directory for the image file. If the image was not found,
	// searches for the specified image file first from the current directory, then in the
	// images/ subdirectory, then the _parent's_ images/ subdirectory, and then _that_
	// parent, on so on, for six levels up. If the image was not loaded successfully,
	// width() and height() will return 0.

	auto filename = string(ImageFileName);
	auto imageDir = getenv("RTW_IMAGES");

	if (imageDir && Load(string(imageDir) + "/" + ImageFileName))
		return;
	if (Load(filename))
		return;
	if (Load("Images/" + filename))
		return;
	if (Load("../Images/" + filename))
		return;
	if (Load("../../Images/" + filename))
		return;
	if (Load("../../../Images/" + filename))
		return;
	if (Load("../../../../Images/" + filename))
		return;
	if (Load("../../../../../Images/" + filename))
		return;
	if (Load("../../../../../../Images/" + filename))
		return;
	std::cerr << "Unable to load image file: No image has been found. " << filename << std::endl;
}

OSTBImage::~OSTBImage()
{
	STBI_FREE(Data);
}

bool OSTBImage::Load(const string& FileName)
{
	auto n = BytesPerPixel;
	Data = stbi_load(FileName.c_str(), &ImageWidth, &ImageHeight, &n, BytesPerPixel);
	BytesPerScanline = ImageWidth * BytesPerPixel;
	return Data != nullptr;
}

int32_t OSTBImage::GetHeight() const
{
	return (Data == nullptr) ? 0 : ImageHeight;
}

int32_t OSTBImage::GetWidth() const
{
	return (Data == nullptr) ? 0 : ImageWidth;
}

const uint8_t* OSTBImage::GetPixelData(int32_t X, int32_t Y) const
{
	static uint8_t magenta[] = { 255, 0, 255 };
	if (Data == nullptr)
	{
		return magenta;
	}
	X = Clamp(X, 0, ImageWidth);
	Y = Clamp(Y, 0, ImageHeight);
	return Data + Y * BytesPerScanline + X * BytesPerPixel;
}

int32_t OSTBImage::Clamp(const int32_t X, const int32_t Low, const int32_t High)
{
	if (X < Low)
	{
		return Low;
	}
	if (X < High)
	{
		return X;
	}
	return High - 1;
}

#ifdef _MSC_VER
#pragma warning (pop)
#endif
