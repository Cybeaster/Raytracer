//
// Created by Cybea on 12/11/2023.
//

#ifndef RAYTRACER_OAPPLICATION_H
#define RAYTRACER_OAPPLICATION_H


#include "../Types/HittableList.h"

#include <cstdint>
#include "../Types/Math.h"
#include "Camera/Camera.h"

class OApplication
{
public:
	OApplication();

private:
	void Init();

	void Render();

	OCamera Camera;

	OHittableList World;
};


#endif //RAYTRACER_OAPPLICATION_H
