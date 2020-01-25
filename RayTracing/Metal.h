#pragma once

#include "Material.h"
#include "MemoryPool.h"

class CMetal : public CMaterial
{
public:
	explicit CMetal(void);
	explicit CMetal(const vec3& vAlbedo);
	explicit CMetal(const vec3& vAlbedo, const float& fFuzzy);
	virtual bool Scatter(const HitRecord& hRec, const CRay& InRay, ScatterRecord& sRec)const override;
	DECLARE_CREATE_BY_MEMORYPOOL(CMetal, CMemoryPool::OBJECT)
private:
	vec3 m_vAlbedo;
	float m_fFuzzy;
};