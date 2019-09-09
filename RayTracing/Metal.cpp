#include "stdafx.h"
#include "Metal.h"

CMetal::CMetal(void)
{
}

CMetal::CMetal(const vec3 & vAlbedo):
	m_vAlbedo(vAlbedo)
{
}

bool CMetal::Scatter(HitRecord & Record, const CRay& InRay, CRay & OutRay, vec3 & vColor)
{
	return false;
}
