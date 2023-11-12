cmake_minimum_required(VERSION 3.25)
project(Raytracer)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost REQUIRED)

add_executable(Raytracer main.cpp Ray/ORay.cpp Ray/ORay.h Types/Math.h Utils/Color.h Types/Math.h Application/OApplication.cpp Application/OApplication.h Application/Camera/Camera.cpp Application/Camera/Camera.h Utils/Utils.h Objects/Sphere.cpp Objects/Sphere.h)

target_link_libraries(Raytracer Boost::boost)