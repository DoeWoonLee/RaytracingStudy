#include "stdafx.h"
#include "Lambertain.h"
#include "MathUtility.h"
#include "Public.h"

CLambertain::CLambertain(void) :
	m_vAlbedo(0.5f, 0.5f, 0.5f)
{
	
}

CLambertain::CLambertain(const vec3 & vAlbedo) :
	m_vAlbedo(vAlbedo)
{
}

bool CLambertain::Scatter(HitRecord & Record, const CRay& InRay, CRay & OutRay, vec3 & vColor)
{
	vColor = m_vAlbedo;

	OutRay.SetDirection(XMVector3Normalize( Record.vNormal.ToSIMD() + CMathUtility::RandUnitSphereVector().ToSIMD() ));

	

	OutRay.SetOrigin(Record.vPos);

	return true;
}
