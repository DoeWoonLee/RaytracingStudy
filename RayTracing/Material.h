#pragma once

#include "HitRecord.h"
#include "Ray.h"

class CMaterial
{
public :
	virtual bool Scatter(HitRecord& Record,const CRay& InRay, CRay& OutRay, vec3& vColor, float& fPdf)const = 0;
	virtual float ScatteringPdf(const CRay& inRay, const CRay& OutRay, const HitRecord& hitRecord) { return 0.f; }
	virtual vec3 Emitted(const vec3& vPos) { return vec3(0.f, 0.f, 0.f); }
};