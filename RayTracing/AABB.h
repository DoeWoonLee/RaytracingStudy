#pragma once

#include "Vector3.h"
#include "Ray.h"
#include "HitRecord.h"
#include "MemoryPool.h"

class CResources;
class CTransform;
class CAABB
{
public:
	explicit CAABB(void);
	explicit CAABB(const vec3& vMin, const vec3& vMax);
	explicit CAABB(const CResources* pResources, const CTransform* pTransform);

	void Update(const CResources* pResources, const CTransform* pTransform);
	DECLARE_CREATE_BY_MEMORYPOOL(CAABB, CMemoryPool::OBJECT)
	bool Hit(const CRay& inRay, float& fMin, float& fMax, HitRecord& hitRecord);

	const vec3& Min() const;
	const vec3& Max() const;
private:
	vec3 m_vMin;
	vec3 m_vMax;
};