#pragma once

#include "../Types/Math.h"
#include "../Objects/SRay.h"

#include <random>

// Constants
namespace Utils::Math
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
inline auto Random()
{
	static std::uniform_real_distribution<T> Distribution(0.0, 1.0); // Random double distribution
	static std::mt19937 Generator; // Random number generator
	return Distribution(Generator); // Return a random double between 0 and 1
}

template<typename T = double>
inline auto Random(float Min, float Max)
{
	return Min + (Max - Min) * Random<T>(); // Return a random double between Min and Max
}

inline SVec3 RandomVec()
{
	return SVec3{ Random<float>(), Random<float>(), Random<float>() }; // Return a random vector
}

inline SVec3 RandomVec(const float Min, const float Max)
{
	return { Random<float>(Min, Max), Random<float>(Min, Max), Random<float>(Min, Max) };
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
	return -OnUnitSphere; // Return the negative vector
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

inline SVec3 Refract(const SVec3& UV, const SVec3& Normal, double EtaIOverEtaT)
{
	/*Calculates the cosine of the angle between the incident vector and the surface normal.
	 *It uses the dot product for this.
	 *The fmin function ensures the value doesn't exceed 1.0,
	 *which might happen due to floating-point imprecision.*/
	auto cosTheta = fmin(Dot(-UV, Normal), 1.0);

	/*Calculates the perpendicular component of the refracted ray.
	 *It scales the sum of the incident vector and the surface normal (scaled by cosTheta) by the refractive index ratio.*/
	SVec3 rOutPerp = EtaIOverEtaT * (UV + cosTheta * Normal);

	/*Calculates the parallel component of the refracted ray.
	 *It's computed based on the length of the perpendicular component,
	 *ensuring that the total length (when combined with the parallel component) corresponds to a unit vector.*/
	SVec3 rOutParallel = -sqrt(fabs(1.0 - LengthSquared(rOutPerp))) * Normal;

	/*Combines the perpendicular and parallel components to give the final refracted vector.*/
	return rOutPerp + rOutParallel;
}

inline double Reflectance(const double Cosine, const double RefIdx)
{
	// Schlick's approximation.
	auto r0 = (1 - RefIdx) / (1 + RefIdx);
	r0 = r0 * r0;
	return r0 + (1 - r0) * pow((1 - Cosine), 5);
}

inline auto Cross(const SVec3& A, const SVec3& B)
{
	return boost::qvm::cross(A, B);
}

inline auto RandomUnitInDisk()
{
	while (true)
	{
		const auto point = SVec3{ Random<float>(-1, 1), Random<float>(-1, 1), 0.0 };
		if (LengthSquared(point) < 1.0)
		{
			return point;
		}
	}
}
}
