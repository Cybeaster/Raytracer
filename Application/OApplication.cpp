#include <iostream>
#include <fstream>
#include "OApplication.h"

#include "../Objects/Sphere.h"
#include "../Ray/SRay.h"
#include "../Types/Types.h"

OApplication::OApplication()
{
	Init();
	Render();
}

void OApplication::Init()
{
	World.Add(make_shared<OSphere>(SVec3{ 0, 0, -1 }, 0.5));
}

void OApplication::Render()
{
	Camera.Render(World);
}
