#include "stdafx.h"
#include "Box.h"

CBox::CBox(const vec3 & vMin, const vec3 & vMax)
{
	m_vMin = vMin;
	m_vMax = vMax;

	MakeMinMax();
}

CBox::~CBox(void)
{
}

bool CBox::Hit(const CRay & inRay, float & fMin, float & fMax, HitRecord & hitRecord)const
{

	vec3 vDir = inRay.GetDirection();
	vec3 vOrigin = inRay.GetOrigin();
	vec3 vPos;
	vec3 vNormal;
	float fResultTime = fMax;
	bool bHit = false;

	// Right yz plane
	float t = (m_vMax.x - vOrigin.x) / vDir.x;

	if (t > fMin && t < fResultTime)
	{
		vPos = inRay.PointAtParameter(t);
		if (!(vPos.y < m_vMin.y || vPos.y > m_vMax.y ||
			vPos.z < m_vMin.z || vPos.z > m_vMax.z))
		{
			fResultTime = t; vNormal = vec3(1.f, 0.f, 0.f); bHit = true;

		}
	}
	// Left yz plane
	t = (m_vMin.x - vOrigin.x) / vDir.x;
	if (t > fMin && t < fResultTime)
	{
		vPos = inRay.PointAtParameter(t);
		if (!(vPos.y < m_vMin.y || vPos.y > m_vMax.y ||
			vPos.z < m_vMin.z || vPos.z > m_vMax.z))
		{
			fResultTime = t; vNormal = vec3(-1.f, 0.f, 0.f); bHit = true;
		}
	}

	// Top xz plane
	t = (m_vMax.y - vOrigin.y) / vDir.y;
	if (t > fMin && t < fResultTime)
	{
		vPos = inRay.PointAtParameter(t);
		if (!(vPos.x < m_vMin.x || vPos.x > m_vMax.x ||
			vPos.z < m_vMin.z || vPos.z > m_vMax.z))
		{
			fResultTime = t; vNormal = vec3(0.f, 1.f, 0.f); bHit = true;
		}
	}

	// Bottom xz plane
	t = (m_vMin.y - vOrigin.y) / vDir.y;
	if (t > fMin && t < fResultTime)
	{
		vPos = inRay.PointAtParameter(t);
		if (!(vPos.x < m_vMin.x || vPos.x > m_vMax.x ||
			vPos.z < m_vMin.z || vPos.z > m_vMax.z))
		{
			fResultTime = t; vNormal = vec3(0.f, -1.f, 0.f); bHit = true;
		}
	}

	// Front xy plane
	t = (m_vMin.z - vOrigin.z) / vDir.z;
	if (t > fMin && t < fResultTime)
	{
		vPos = inRay.PointAtParameter(t);
		if (!(vPos.x < m_vMin.x || vPos.x > m_vMax.x ||
			vPos.y < m_vMin.y || vPos.y > m_vMax.y))
		{
			fResultTime = t; vNormal = vec3(0.f, 0.f, -1.f); bHit = true;
		}
	}

	// Back xy plane
	t = (m_vMax.z - vOrigin.z) / vDir.z;
	if (t > fMin && t < fResultTime)
	{
		vPos = inRay.PointAtParameter(t);
		if (!(vPos.x < m_vMin.x || vPos.x > m_vMax.x ||
			vPos.y < m_vMin.y || vPos.y > m_vMax.y))
		{
			fResultTime = t; vNormal = vec3(0.f, 0.f, 1.f); bHit = true;
		}
	}
	if (bHit)
	{
		hitRecord.fTime = fResultTime;
		//hitRecord.vPos = inRay.PointAtParameter(hitRecord.fTime);
		hitRecord.vNormal = vNormal;
		return true;
	}


	return false;
}
