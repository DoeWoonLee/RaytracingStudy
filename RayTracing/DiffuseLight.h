#pragma once

#include "Material.h"
#include "MemoryPool.h"


class CDiffuseLight : public CMaterial
{
public:
	explicit CDiffuseLight(void);
	explicit CDiffuseLight(const vec3& vLight);
	DECLARE_CREATE_BY_MEMORYPOOL(CDiffuseLight, CMemoryPool::OBJECT)

	virtual bool Scatter(HitRecord& Record, const CRay& InRay, CRay& OutRay, vec3& vColor) { return false; }
	virtual vec3 Emitted(const vec3& vPos);
private:
	vec3 m_vLight;
};