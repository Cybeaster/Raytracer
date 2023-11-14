#pragma once
#include "Math.h"

struct SColor
{
	SColor()
	{
		R = G = B = A = 0;
	}


	SColor(float r, float g, float b)
		: R(r)
		, G(g)
		, B(b)
		, A(1.0f)
	{
	}

	SColor(const SColor&) = default;

	SColor(const SVec3& Other)
	{
		R = Other.a[0];
		G = Other.a[1];
		B = Other.a[2];
		A = 1.0f;
	}


	operator SVec3() const
	{
		return SVec3{ R, G, B };
	}

	operator SVec4() const
	{
		return SVec4{ R, G, B, A };
	}

	SColor operator*(float Value) const
	{
		return SColor{ R * Value, G * Value, B * Value };
	}

	float R;
	float G;
	float B;
	float A;
};
