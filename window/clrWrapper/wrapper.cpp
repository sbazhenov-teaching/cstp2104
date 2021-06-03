#include "wrapper.h"

#using <logicLib.dll>
//using namespace logicLib;

float getRadiusManaged(int frameNum)
{
    // Not allowed because of .NET memory management, including garbage collection
    //Class1* c0 = new Class1();
    logicLib::Class1^ c1 = gcnew logicLib::Class1();
    return c1->getRadius(0);
    //return 50.f;
}

#pragma managed(push, off)

float getRadius(int frameNum)
{
    return getRadiusManaged(frameNum);
}

#pragma managed(pop)
