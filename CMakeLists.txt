cmake_minimum_required(VERSION 3.25)
project(Raytracer)

set(CMAKE_CXX_STANDARD 20)

find_package(Boost REQUIRED)

add_executable(Raytracer main.cpp Objects/Ray.h Types/Math.h Types/Math.h Application/OApplication.cpp Application/OApplication.h Application/Camera/Camera.cpp Application/Camera/Camera.h Objects/Hittable/Sphere/Sphere.cpp Objects/Hittable/Sphere/Sphere.h Utils/Math.h
        Types/Color.h
        Objects/Hittable/Hittable.h
        Utils/Exceptions.h
        Objects/Hittable/List/HittableList.h
        Types/Types.h
        Types/Interval/Interval.h
        Types/Interval/Interval.cpp
        Objects/Materials/Material.h
        Objects/Materials/Lambertian.h
        Objects/Materials/Metal.h
        Objects/Materials/Dielectric.h
        Application/Scenes/Scenes.h
        Utils/Materials.h
        Types/Color.cpp
        Types/Color.cpp
        Application/Renderers/SyncRenderer.h
        Application/Renderers/Renderer.h
        Application/Renderers/AsyncRenderer.h
        Application/Renderers/AsyncRenderer.h
        Types/Output/Serializable.h
        Types/Output/PPMSerializer.h
        Types/Output/PPMSerializer.cpp
        Application/Camera/Raytracer.h
        Types/Logger.h
        Types/Profiler/Profiler.h
        Types/ExitHelper.h
        Types/AABB/AABB.cpp
        Types/AABB/AABB.h
        Objects/Hittable/List/HittableList.cpp
        Objects/Hittable/List/HittableList.h
        Objects/Hittable/BVH/BVHNode.cpp
        Objects/Hittable/BVH/BVHNode.h
        Objects/Textures/Texture.h
        Objects/Textures/CheckerTexture.h
        Utils/ImageLoader/STBImage.cpp
        Utils/ImageLoader/STBImage.h
        Objects/Textures/ImageTexture.h
        Utils/Algorithms/Perlin.cpp
        Utils/Algorithms/Perlin.h
        Objects/Textures/NoiseTexture.h
        Objects/Hittable/Quad/Quad.h
        Objects/Hittable/Quad/Quad.cpp
        Objects/Hittable/Quad/Quad.cpp
        Objects/Materials/DiffuseLight.h
)

target_link_libraries(Raytracer Boost::boost)