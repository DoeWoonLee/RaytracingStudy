#include "stdafx.h"
#include "RectangleXY.h"

CRectangleXY::CRectangleXY(const float & fWidth, const float & fHeight):
	m_fWidth(fWidth), m_fHeight(fHeight)
{
	MakeMinMax();
}

CRectangleXY::~CRectangleXY()
{
}

void CRectangleXY::MakeMinMax()
{
	m_vMin.x = -m_fWidth / 2.f;
	m_vMin.y = m_fHeight / 2.f;

	m_vMax.x = -m_vMin.x;
	m_vMax.y = -m_vMin.y;
}

bool CRectangleXY::Hit(const CRay & inRay, float & fMin, float & fMax, HitRecord & hitRecord) const
{
	float t = -inRay.GetOrigin().z / inRay.GetDirection().z;
	if (t > fMax || t < fMin) return false;

	vec3 vPos = inRay.PointAtParameter(t);

	if (-m_fHeight * 0.5f > vPos.y || vPos.y > m_fHeight *0.5f)
		return false;
	if (-m_fWidth * 0.5f > vPos.x || vPos.x > m_fWidth * 0.5f)
		return false;

	hitRecord.fTime = t;
	hitRecord.vNormal = vec3(0.f, 0.f, 1.f);
	hitRecord.vPos = vPos;


	return true;
}
