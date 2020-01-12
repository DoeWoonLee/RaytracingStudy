#pragma once

#include "Material.h"
#include "MemoryPool.h"

class CDielectric : public CMaterial
{
public:
	explicit CDielectric(void);
	explicit CDielectric(float& fRefIdx);
	explicit CDielectric(vec3& vAlbedo, float& fRefIdx);
	DECLARE_CREATE_BY_MEMORYPOOL(CDielectric, CMemoryPool::OBJECT)


	virtual bool Scatter(HitRecord& Record, const CRay& InRay, CRay& OutRay, vec3& vColor);
private:
	vec3 m_vAlbedo;
	float m_fRefIdx;
};