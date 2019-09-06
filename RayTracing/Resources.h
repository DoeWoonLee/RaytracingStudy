#pragma once

#include "Ray.h"
#include "HitRecord.h"

class CResources
{
public:
	virtual bool Hit(const CRay& inRay, float& fMin, float& fMax, HitRecord& hitRecord) = 0;
};