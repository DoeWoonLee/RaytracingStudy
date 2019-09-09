#pragma once

#include "HitRecord.h"
#include "Ray.h"

class CMaterial
{
public :
	virtual bool Scatter(HitRecord& Record,const CRay& InRay, CRay& OutRay, vec3& vColor) = 0;
};