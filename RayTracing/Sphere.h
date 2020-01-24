#pragma once

#include "Resources.h"
#include "Vector3.h"
#include "HitRecord.h"
#include "MemoryPool.h"
class CSphere : public CResources
{
public:
	explicit CSphere();
	explicit CSphere(const float& fRadius);
	explicit CSphere(const vec3& vCenter,const float& fRadius);
protected:
	virtual ~CSphere();
public:
	virtual void MakeMinMax();
	virtual bool Hit(const CRay& InputRay, float& fMin, float & fMax, HitRecord& hitRecord)const;

	DECLARE_CREATE_BY_MEMORYPOOL(CSphere, CMemoryPool::OBJECT)
private:
	vec3 m_vCenter;
	float m_fRadius;

};