#pragma once

#include "Vector3.h"

class CMathUtility
{
public:
	static UINT& XorShift32(void);
	static float frand0to1(void);
	static vec3 RandUnitSphereVector(void);
};