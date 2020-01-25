#include "stdafx.h"
#include "DiffuseLight.h"
#include "EmittedObjectsMgr.h"

CDiffuseLight::CDiffuseLight(void) :
	m_vLight(1.f, 1.f, 1.f)
{

}

CDiffuseLight::CDiffuseLight(const vec3 & vLight):
	m_vLight(vLight)
{

}
void CDiffuseLight::SetOwner(CFieldObject * pOwner)
{
	m_pOwner = pOwner;

	EMITTEDOBJECTMGR->AddEmittedObject(m_pOwner);

}

vec3 CDiffuseLight::Emitted(const CRay& inRay, const HitRecord& hitRecord)
{
	//if (vec3::Dot(hitRecord.vNormal, inRay.GetDirection()) > 0.0f)
		return m_vLight;
	//else
	//	return vec3(0.f, 0.f, 0.f);
}

