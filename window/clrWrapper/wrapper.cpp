#include "wrapper.h"

#using <logicLib.dll>
using namespace logicLib;

float getRadiusManaged(int frameNum)
{
    Class1^ c1 = gcnew Class1();
    return c1->getRadius(0);
    //return 50.f;
}

#pragma managed(push, off)

float getRadius(int frameNum)
{
    return getRadiusManaged(frameNum);
}

#pragma managed(pop)
