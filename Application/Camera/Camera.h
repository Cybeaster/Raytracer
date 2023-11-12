#pragma once

#include "../../Types/Math.h"

class OCamera {

public:


    auto GetFocalLength() {
        return FocalLength;
    }

    auto GetCameraCenter() {
        return CameraCenter;
    }

private:
    float FocalLength = 1.0f;
    SVec3 CameraCenter = {0, 0, 0};

};


