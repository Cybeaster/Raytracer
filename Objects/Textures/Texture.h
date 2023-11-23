#pragma once
#include "../../Types/Color.h"

class ITexture
{
public:
	virtual ~ITexture() = default;
	virtual SColor Value(float U, float V, const SVec3& Point) const = 0;

	template<typename T, typename... Args>
	static shared_ptr<ITexture> CreateTexture(Args&&... args)
	{
		return make_shared<T>(forward<Args>(args)...);
	}
};


class OSolidColor : public ITexture
{
public:
	OSolidColor(const SColor& Color)
		: ColorValue(Color)
	{
	}

	OSolidColor(float Red, float Green, float Blue)
		: OSolidColor(SColor{ Red, Green, Blue })
	{
	}

	SColor Value(float U, float V, const SVec3& Point) const override
	{
		return ColorValue;
	}

private:
	SColor ColorValue;
};

