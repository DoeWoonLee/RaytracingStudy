#pragma once

#include "HitRecord.h"
#include "Ray.h"

class CMaterial
{
public :
	virtual bool Scatter(HitRecord& Record, CRay& OutRay, vec3& vColor) = 0;
};