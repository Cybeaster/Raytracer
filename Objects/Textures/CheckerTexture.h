#pragma once
#include "Texture.h"
#include "../../Types/Color.h"
#include "../../Types/Types.h"

class OCheckerTexture : public ITexture
{
public:
	OCheckerTexture(double _Scale, const shared_ptr<ITexture>& _Even, const shared_ptr<ITexture>& _Odd)
		: InvScale(1 / _Scale)
		, Even(_Even)
		, Odd(_Odd)
	{
	}

	OCheckerTexture(const double _Scale, SColor C1, SColor C2)
		: InvScale(1 / _Scale)
		, Even(make_shared<OSolidColor>(C1))
		, Odd(make_shared<OSolidColor>(C2))
	{
	}

	SColor Value(float U, float V, const SVec3& Point) const override
	{
		using namespace Utils::Math;
		auto xInteger = static_cast<int>(std::floor(InvScale * GetX(Point)));
		auto yInteger = static_cast<int>(std::floor(InvScale * GetY(Point)));
		auto zInteger = static_cast<int>(std::floor(InvScale * GetZ(Point)));

		bool isEven = (xInteger + yInteger + zInteger) % 2 == 0;
		return isEven ? Even->Value(U, V, Point) : Odd->Value(U, V, Point);
	}

private:
	double InvScale;
	shared_ptr<ITexture> Even;
	shared_ptr<ITexture> Odd;
};
