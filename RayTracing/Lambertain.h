#pragma once

#include "Material.h"
#include "MemoryPool.h"

class CLambertain : public CMaterial
{
public:
	explicit CLambertain(void);
	explicit CLambertain(const vec3& vAlbedo);
	virtual bool Scatter(HitRecord& Record, CRay& OutRay, vec3& vColor);
	DECLARE_CREATE_BY_MEMORYPOOL(CLambertain, CMemoryPool::OBJECT);
private:
	vec3 m_vAlbedo;
};