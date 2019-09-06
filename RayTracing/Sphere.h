#pragma once

#include "Resources.h"
#include "Vector3.h"
#include "HitRecord.h"

class CSphere : public CResources
{
public:
	explicit CSphere();
	explicit CSphere(const float& fRadius);
	explicit CSphere(const vec3& vCenter,const float& fRadius);
	virtual bool Hit(const CRay& InputRay, float& fMin, float & fMax, HitRecord& hitRecord);
private:
	vec3 m_vCenter;
	float m_fRadius;

};