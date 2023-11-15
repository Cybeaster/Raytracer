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

	SColor(float r, float g, float b, float a)
		: R(r)
		, G(g)
		, B(b)
		, A(a)
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

	template<typename T>
	SColor operator *(T Value)
	{
		SColor color;
		color.R = R * Value;
		color.G = G * Value;
		color.B = B * Value;
		color.A = A * Value;
		return color;
	}

	template<typename T>
	void operator *=(T Value)
	{
		R *= Value;
		G *= Value;
		B *= Value;
		A *= Value;
	}

	operator SVec3() const
	{
		return SVec3{ R, G, B };
	}

	operator SVec4() const
	{
		return SVec4{ R, G, B, A };
	}

	SColor operator+(const SColor& Other) const
	{
		return SColor{ R + Other.R, G + Other.G, B + Other.B, A + Other.A };
	}

	void operator+=(const SColor& Other)
	{
		R += Other.R;
		G += Other.G;
		B += Other.B;
		A += Other.A;
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
