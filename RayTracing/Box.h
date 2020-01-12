#pragma once


#include "Resources.h"
#include "MemoryPool.h"

class CBox : public CResources
{
private:
	CBox(const vec3& vMin, const vec3& vMax);
public:
	DECLARE_CREATE_BY_MEMORYPOOL(CBox, CMemoryPool::OBJECT)
	virtual bool Hit(const CRay& inRay, float& fMin, float& fMax, HitRecord& hitRecord)const;
};