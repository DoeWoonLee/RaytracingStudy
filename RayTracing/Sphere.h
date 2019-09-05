#pragma once

#include "Resources.h"
#include "Vector3.h"

class CSphere : public CResources
{
public:
	explicit CSphere();
	virtual bool Hit(const CRay& InputRay, float& fMin, float & fMax);
private:
	vec3 m_vCenter;
	float m_fRadius;

};