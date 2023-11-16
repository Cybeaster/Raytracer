#pragma once

#include "../Types/Math.h"
#include "../Objects/SRay.h"

#include <random>

// Constants
namespace Utils { namespace Math
{
inline auto Lerp(float T, const SVec3& A, const SVec3& B)
{
	return (1.0f - T) * A + T * B; // The interpolated vector
}

inline auto Normalize(const SVec3& Vec)
{
	return normalized(Vec); // Return the normalized vector
}

inline auto Dot(const SVec3& A, const SVec3& B)
{
	return dot(A, B); // Return the dot product
}

inline auto LengthSquared(const SVec3& A)
{
	return mag_sqr(A); // Return the squared magnitude of the vector
}

inline auto Length(const SVec3& A)
{
	return boost::qvm::mag(A); // Return the magnitude of the vector
}

inline auto IsNormalized(const SVec3& A, float Epsilon = 1e-6f)
{
	return abs(1.0f - Length(A)) < Epsilon; // Return true if the vector is normalized
}

template<typename T>
auto Squared(T Value)
{
	return Value * Value;
}

inline auto DegreesToRaians(auto Degrees)
{
	return Degrees * (PI / 180.0f);
}


inline auto SphereIntersection(const SVec3& Center, float Radius, const SRay& Ray)
{
	using namespace Math;
	auto oc = Ray.GetOrigin() - Center; // Vector from sphere center to ray origin
	auto a = LengthSquared(Ray.GetDirection()); // Squared magnitude of the ray's direction
	auto halfB = Dot(oc, Ray.GetDirection());
	auto c = LengthSquared(oc) - Squared(Radius);
	auto discriminant = Squared(halfB) - a * c;
	if (discriminant < 0)
	{
		return -1.0f; // No intersection, return -1
	}
	else
	{
		return float((-halfB - sqrt(discriminant)) / a); // Return the smaller root of the quadratic equation
	}
}

inline SVec3 UnitVector(const SVec3 Value)
{
	return Value / Math::Length(Value); // Return the unit vector
}

template<typename T = double>
inline auto RandomDouble()
{
	static std::uniform_real_distribution<T> Distribution(0.0, 1.0); // Random double distribution
	static std::mt19937 Generator; // Random number generator
	return Distribution(Generator); // Return a random double between 0 and 1
}

template<typename T = double>
inline auto RandomDouble(float Min, float Max)
{
	return Min + (Max - Min) * RandomDouble<T>(); // Return a random double between Min and Max
}

inline SVec3 RandomVec()
{
	return SVec3{ RandomDouble<float>(), RandomDouble<float>(), RandomDouble<float>() }; // Return a random vector
}

inline SVec3 RandomVec(const float Min, const float Max)
{
	return { RandomDouble<float>(Min, Max), RandomDouble<float>(Min, Max), RandomDouble<float>(Min, Max) };
}

inline SVec3 RandomVecInUnitSphere()
{
	while (true)
	{
		auto p = RandomVec(-1.0f, 1.0f); // Get a random vector
		if (LengthSquared(p) < 1)
		{
			return p;
		}
	}
}

inline SVec3 RandomUnitVector()
{
	return UnitVector(RandomVecInUnitSphere()); // Return a random unit vector
}

inline SVec3 RandomVecOnHemisphere(const SVec3& Normal)
{
	SVec3 OnUnitSphere = RandomUnitVector(); // Get a random vector in unit sphere
	if (Dot(OnUnitSphere, Normal) > 0.0f) // If the dot product is positive
	{
		return OnUnitSphere; // Return the vector
	}
	else
	{
		return -OnUnitSphere; // Return the negative vector
	}
}

inline double LinearToGamma(double Linear)
{
	return sqrt(Linear);
}

inline SVec3 LinearToGamma(SVec3 Linear)
{
	return { sqrt(Linear.a[0]), sqrt(Linear.a[1]), sqrt(Linear.a[2]) };
}

inline bool IsNearZero(const SVec3& Other, double Epsilon = 1e-8)
{
	return (Other.a[0] < Epsilon) && (Other.a[1] < Epsilon) && (Other.a[2] < Epsilon);
}

inline SVec3 Reflect(const SVec3& Vec, const SVec3& Normal)
{
	return Vec - 2 * Dot(Vec, Normal) * Normal;
}
}}
