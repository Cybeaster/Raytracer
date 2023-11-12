//
// Created by Cybea on 12/11/2023.
//

#ifndef RAYTRACER_OAPPLICATION_H
#define RAYTRACER_OAPPLICATION_H


#include <cstdint>
#include "../Types/Math.h"
#include "Camera/Camera.h"

class OApplication {
public:
    OApplication();


private:

    void Init();

    void Render();

    float ImageWidth = 800;
    float ImageHeight = -1;

    float ViewportWidth = 2;
    float ViewportHeight = 1;

    SVec3 ViewportUpperLeft;
    SVec3 PixelZeroLoc;

    SVec3 PixelDeltaU;
    SVec3 PixelDeltaV;

    double AspectRatio = 16.0f / 9.0f;

    OCamera Camera;
};


#endif //RAYTRACER_OAPPLICATION_H
