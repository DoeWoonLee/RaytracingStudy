#pragma once

#include "Material.h"

class CLambertain : public CMaterial
{
public:
	explicit CLambertain(void);
	explicit CLambertain(const vec3& vAlbedo);
	virtual bool Scatter(HitRecord& Record, CRay& OutRay, vec3& vColor);
private:
	vec3 m_vAlbedo;
};