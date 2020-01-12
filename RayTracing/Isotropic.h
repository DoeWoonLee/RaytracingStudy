#pragma once

#include "Material.h"
#include "MemoryPool.h"

class CIsotropic : public CMaterial
{
public:
	explicit CIsotropic(const vec3& vAlbedo);
	virtual bool Scatter(HitRecord& Record, const CRay& InRay, CRay& OutRay, vec3& vColor);

	DECLARE_CREATE_BY_MEMORYPOOL(CIsotropic, CMemoryPool::OBJECT)
private:
	vec3 m_vAlbedo;
};