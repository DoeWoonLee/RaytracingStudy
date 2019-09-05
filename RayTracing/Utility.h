#pragma once


#include "Vector3.h"
#include "Ray.h"

class CUtility
{
public:
	static void GetSkyColor(float& y, vec3& vOut);
	static void GetPixelColor(CRay& Ray);
	static bool CalculateColor(CRay& Ray);
};
