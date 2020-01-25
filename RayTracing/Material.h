#pragma once

#include "HitRecord.h"
#include "Ray.h"

class CFieldObject;
class CMaterial
{
public :
	virtual bool Scatter(const HitRecord& hRec,const CRay& InRay, ScatterRecord& sRec)const = 0;
	virtual float ScatteringPdf(const HitRecord& hRec, const CRay& inRay, const CRay& OutRay)const { return 0.f; }
	virtual vec3 Emitted(const CRay& inRay, const HitRecord& hitRecord) { return vec3(0.f, 0.f, 0.f); }

	virtual void SetOwner(CFieldObject* pOwner);
protected:
	CFieldObject* m_pOwner = nullptr;
};