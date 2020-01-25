#pragma once

#include "Ray.h"
#include "HitRecord.h"
#include <float.h>
#include "BaseClass.h"

class CTransform;
class CResources : public IBaseClass
{
protected:
	virtual ~CResources() {}
public:
	virtual void MakeMinMax() {};
	virtual bool Hit(const CRay& inRay, float& fMin, float& fMax, HitRecord& hitRecord) const = 0 ;
	virtual float PdfValue(const vec3& vHitPos,
		const vec3& vScale , 
		const vec3& vOutRayDir,
		const vec3& vOutWRayDir)const { return 0.f; }
	virtual vec3 GenerateRandDirinRes(const vec3 & vWorldPos, const CTransform* pTransform) const { return vec3(1.f, 0.f, 0.f); }

	const vec3& Min(void) const;
	const vec3& Max(void) const;
protected:
	vec3 m_vMin = vec3(FLT_MAX, FLT_MAX, FLT_MAX );
	vec3 m_vMax = vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
};