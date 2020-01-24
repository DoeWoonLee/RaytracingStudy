#pragma once

#include "Ray.h"
#include "HitRecord.h"
#include <float.h>
#include "BaseClass.h"

class CResources : public IBaseClass
{
protected:
	virtual ~CResources() {}
public:
	virtual void MakeMinMax() {};
	virtual bool Hit(const CRay& inRay, float& fMin, float& fMax, HitRecord& hitRecord) const = 0 ;
	
	const vec3& Min(void) const;
	const vec3& Max(void) const;
protected:
	vec3 m_vMin = vec3(FLT_MAX, FLT_MAX, FLT_MAX );
	vec3 m_vMax = vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
};