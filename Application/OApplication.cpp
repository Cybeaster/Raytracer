//
// Created by Cybea on 12/11/2023.
//

#include <iostream>
#include <fstream>
#include "OApplication.h"
#include "../Ray/ORay.h"
#include "../Utils/Color.h"

OApplication::OApplication() {

    Init();
    Render();
}

void OApplication::Init() {
    ImageHeight = (ImageWidth / AspectRatio);
    ImageHeight = (ImageHeight < 1) ? 1 : ImageHeight;

    ViewportWidth = ((ImageWidth) / ImageHeight) * ViewportHeight;

    auto viewportU = SVec3{static_cast<float>(ViewportWidth), 0.0, 0.0};
    auto viewportV = SVec3{0.0, -static_cast<float>(ViewportHeight), 0.0};

    PixelDeltaU = viewportU / ImageWidth;
    PixelDeltaV = viewportV / ImageHeight;

    ViewportUpperLeft =
            Camera.GetCameraCenter() - SVec3{0, 0, Camera.GetFocalLength()} - (viewportU / 2) + (viewportV / 2);
    PixelZeroLoc = ViewportUpperLeft + (PixelDeltaU / 2) - (PixelDeltaV / 2);
}

void OApplication::Render() {

    std::ofstream OutFile("image.ppm");

    OutFile << "P3\n" << ImageWidth << ' ' << ImageHeight << "\n255\n";

    for (int j = 0; j < ImageHeight; ++j) {
        std::clog << "\rScanlines remaining: " << ImageHeight - j << ' ' << std::flush;
        for (int i = 0; i < ImageWidth; ++i) {
            auto pixelCenter = PixelZeroLoc + (i * PixelDeltaU) + (j * PixelDeltaV);
            auto rayDir = pixelCenter - Camera.GetCameraCenter();

            ORay ray{Camera.GetCameraCenter(), rayDir};
            SColor pixelColor = Utils::GetColorAt(ray);
            Utils::WriteColor(OutFile, pixelColor);
        }
    }
    OutFile.close();
    std::clog << "\nDone.\n";
}
