#pragma once


#include "Vector3.h"
#include "Ray.h"
#include "FieldObject.h"
#include <vector>

class CBVHTree;
class CUtility
{
public:
	static void GetSkyColor(float& y, vec3& vOut);
	static void GetPixelColor(CRay& Ray, std::vector<CFieldObject*>& vecFieldObject, vec3& vOutColor);
	static void GetPixelColor(CRay& Ray, CBVHTree* pBVHTree, vec3& vOutColor);

};
