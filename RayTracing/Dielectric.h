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


	virtual bool Scatter(const HitRecord& hRec, const CRay& InRay, ScatterRecord& sRec)const override;

private:
	vec3 m_vAlbedo;
	float m_fRefIdx;
};