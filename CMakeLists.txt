cmake_minimum_required(VERSION 3.25)

# Auto-detect vcpkg toolchain if not explicitly provided
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    if(DEFINED ENV{VCPKG_ROOT} AND EXISTS "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
        set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE FILEPATH "Vcpkg toolchain file")
    elseif(DEFINED ENV{VCPKG_INSTALLATION_ROOT} AND EXISTS "$ENV{VCPKG_INSTALLATION_ROOT}/scripts/buildsystems/vcpkg.cmake")
        set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_INSTALLATION_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE FILEPATH "Vcpkg toolchain file")
    endif()
endif()

project(Raytracer)

set(CMAKE_CXX_STANDARD 20)

# Prefer Boost via CONFIG mode (vcpkg), fall back to module, then fallback to header-only include discovery
find_package(Boost CONFIG QUIET COMPONENTS qvm)

if(NOT Boost_FOUND)
    find_package(Boost QUIET)
endif()

if(NOT Boost_FOUND)
    # Try to locate Boost headers installed by vcpkg manifest mode in common locations
    set(_VCPKG_TRIPLETS x64-windows x86-windows arm64-windows)
    set(_BOOST_CANDIDATE_DIRS)
    foreach(_t ${_VCPKG_TRIPLETS})
        list(APPEND _BOOST_CANDIDATE_DIRS
            "${CMAKE_BINARY_DIR}/vcpkg_installed/${_t}"
            "${CMAKE_SOURCE_DIR}/vcpkg_installed/${_t}"
        )
    endforeach()
    if(EXISTS "${CMAKE_SOURCE_DIR}/out/build")
        file(GLOB _OUT_BUILD_VCPKG_DIRS "${CMAKE_SOURCE_DIR}/out/build/*/vcpkg_installed/*")
        list(APPEND _BOOST_CANDIDATE_DIRS ${_OUT_BUILD_VCPKG_DIRS})
    endif()

    set(_BOOST_HEADERS_FOUND FALSE)
    foreach(_base ${_BOOST_CANDIDATE_DIRS})
        if(EXISTS "${_base}/include/boost/qvm.hpp")
            set(Boost_INCLUDE_DIRS "${_base}/include")
            set(_BOOST_HEADERS_FOUND TRUE)
            message(STATUS "Found Boost headers at: ${Boost_INCLUDE_DIRS}")
            if(NOT TARGET Boost::boost)
                add_library(Boost::boost INTERFACE IMPORTED)
                set_target_properties(Boost::boost PROPERTIES
                    INTERFACE_INCLUDE_DIRECTORIES "${Boost_INCLUDE_DIRS}"
                )
            endif()
            if(NOT TARGET Boost::qvm)
                add_library(Boost::qvm INTERFACE IMPORTED)
                set_target_properties(Boost::qvm PROPERTIES
                    INTERFACE_LINK_LIBRARIES Boost::boost
                    INTERFACE_INCLUDE_DIRECTORIES "${Boost_INCLUDE_DIRS}"
                )
            endif()
            break()
        endif()
    endforeach()

    if(_BOOST_HEADERS_FOUND)
        set(Boost_FOUND TRUE)
    endif()
endif()

if(NOT Boost_FOUND)
    message(FATAL_ERROR
        "Boost not found. Set CMAKE_TOOLCHAIN_FILE to your vcpkg toolchain "
        "or install Boost and/or set Boost_INCLUDE_DIR(S). If using vcpkg manifest mode, "
        "ensure the vcpkg toolchain is active."
    )
endif()

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
        Objects/Hittable/Translated/Translated.h
        Objects/Hittable/YRotated/YRotated.h
        Objects/Hittable/YRotated/YRotated.cpp
        Objects/Hittable/Volumes/ConstantMediumVolume.h
        Objects/Materials/Isotropic.h
        Objects/Hittable/Volumes/ConstantMediumVolume.cpp
)

# Link Boost imported targets when provided by CONFIG mode (e.g., vcpkg)
if(TARGET Boost::boost)
    target_link_libraries(Raytracer PRIVATE Boost::boost)
endif()
if(TARGET Boost::qvm)
    target_link_libraries(Raytracer PRIVATE Boost::qvm)
endif()

# Fallback for module mode: add include directories
if(Boost_INCLUDE_DIRS)
    target_include_directories(Raytracer PRIVATE ${Boost_INCLUDE_DIRS})
endif()