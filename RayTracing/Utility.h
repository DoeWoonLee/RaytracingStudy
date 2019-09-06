#pragma once


#include "Vector3.h"
#include "Ray.h"
#include "FieldObject.h"
#include <vector>

class CUtility
{
public:
	static void GetSkyColor(float& y, vec3& vOut);
	static void GetPixelColor(CRay& Ray, std::vector<CFiledObject*>& vecFieldObject, vec3& vOutColor);
	static bool CalculateColor(CRay& Ray);

};
