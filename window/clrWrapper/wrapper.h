#pragma once

#ifdef COMPILE_DLL
#define WRAPPER_LIB_EXPORT __declspec(dllexport)
#else
#define WRAPPER_LIB_EXPORT __declspec(dllimport)
#endif

float WRAPPER_LIB_EXPORT getRadius(int frameNum);
