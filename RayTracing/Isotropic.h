#pragma once

#include "Material.h"
#include "MemoryPool.h"

class CIsotropic : public CMaterial
{
public:
	explicit CIsotropic(const vec3& vAlbedo);
	virtual bool Scatter(const HitRecord& hRec, const CRay& InRay, ScatterRecord& sRec)const override;

	DECLARE_CREATE_BY_MEMORYPOOL(CIsotropic, CMemoryPool::OBJECT)
private:
	vec3 m_vAlbedo;
};