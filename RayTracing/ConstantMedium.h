#pragma once


#include "Resources.h"
#include "MemoryPool.h"

class CConstantMedium : public CResources
{
public:
	explicit CConstantMedium(const CResources* pResource, const float& fDensity);
	virtual void MakeMinMax();
	virtual bool Hit(const CRay& inRay, float& fMin, float& fMax, HitRecord& hitRecord) const;

	DECLARE_CREATE_BY_MEMORYPOOL(CConstantMedium, CMemoryPool::OBJECT)
private:
	float m_fDensity;
	const CResources* m_pResource;
};