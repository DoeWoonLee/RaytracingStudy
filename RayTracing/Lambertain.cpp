#include "stdafx.h"
#include "Lambertain.h"
#include "MathUtility.h"
#include "Public.h"
#include "ConstantTexture.h"

CLambertain::CLambertain(void) :
	m_pAlbedo(new ConstantTexture(vec3(0.5f, 0.5f, 0.5f)))
{
	
}

CLambertain::CLambertain(Texture* pTexture) :
	m_pAlbedo(pTexture)
{
	m_pAlbedo->AddRef();
}

CLambertain::~CLambertain(void)
{
	if (m_pAlbedo->Release())
	{
		m_pAlbedo = nullptr;
	}
}

bool CLambertain::Scatter(HitRecord & Record, const CRay& InRay, CRay & OutRay, vec3 & vColor)
{
	vColor = m_pAlbedo->Value(0.f,0.f, Record.vPos);

	OutRay.SetDirection(XMVector3Normalize( Record.vNormal.ToSIMD() + CMathUtility::RandUnitSphereVector().ToSIMD() ));

	

	OutRay.SetOrigin(Record.vPos);

	return true;
}
