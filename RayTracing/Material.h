#pragma once

#include "HitRecord.h"
#include "Ray.h"

class CMaterial
{
public :
	virtual bool Scatter(HitRecord& Record,const CRay& InRay, CRay& OutRay, vec3& vColor) = 0;
	virtual vec3 Emitted(const vec3& vPos) { return vec3(0.f, 0.f, 0.f); }
};