#include <iostream>


#define DEBUG 1

#include "Application/OApplication.h"

int main()
{
	const auto application = OApplication::Get();
	application->RenderScene("ManyObjectsSmoke", ECameraPresets::Insane);
}
