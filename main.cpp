#include <iostream>
#include "Application/OApplication.h"

int main()
{
	auto application = OApplication::Get();
	application->RenderScene("CornellBox");
}
