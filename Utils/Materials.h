#pragma once
#include "../Objects/Materials/Dielectric.h"
#include "../Objects/Materials/Lambertian.h"
#include "../Objects/Materials/Metal.h"


namespace Utils::Materials
{
template<typename MatType, typename... Types>
inline auto CreateMaterial(const Types&... Params)
{
	return make_shared<MatType>(Params...);
}
}
