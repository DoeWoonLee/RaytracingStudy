#pragma once

#include "Material.h"
#include "MemoryPool.h"


class CDiffuseLight : public CMaterial
{
public:
	explicit CDiffuseLight(void);
	explicit CDiffuseLight(const vec3& vLight);
	DECLARE_CREATE_BY_MEMORYPOOL(CDiffuseLight, CMemoryPool::OBJECT)

	virtual bool Scatter(const HitRecord& hRec, const CRay& InRay, ScatterRecord& sRec)const override{ return false;}
	virtual vec3 Emitted(const CRay& inRay, const HitRecord& hitRecord)override;
	virtual void SetOwner(CFieldObject* pOwner)override;
private:
	vec3 m_vLight;
};