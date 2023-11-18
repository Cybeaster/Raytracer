cmake_minimum_required(VERSION 3.25)
project(Raytracer)

set(CMAKE_CXX_STANDARD 20)

find_package(Boost REQUIRED)

add_executable(Raytracer main.cpp Objects/SRay.h Types/Math.h Types/Math.h Application/OApplication.cpp Application/OApplication.h Application/Camera/Camera.cpp Application/Camera/Camera.h Objects/Sphere.cpp Objects/Sphere.h Utils/Math.h
        Types/Color.h
        Objects/Hittable.h
        Utils/Exceptions.h
        Objects/HittableList.h
        Types/Types.h
        Types/Interval/Interval.h
        Types/Interval/Interval.cpp
        Objects/Materials/Material.h
        Objects/Materials/Lambertian.h
        Objects/Materials/Metal.h
        Objects/Materials/Dielectric.h
        Application/Scencens/Scences.h
        Utils/Materials.h
)

target_link_libraries(Raytracer Boost::boost)