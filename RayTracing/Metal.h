#pragma once

#include "Material.h"
#include "MemoryPool.h"

class CMetal : public CMaterial
{
public:
	explicit CMetal(void);
	explicit CMetal(const vec3& vAlbedo);
	virtual bool Scatter(HitRecord& Record, const CRay& InRay, CRay& OutRay, vec3& vColor);
	DECLARE_CREATE_BY_MEMORYPOOL(CMetal, CMemoryPool::OBJECT)
private:
	vec3 m_vAlbedo;
};