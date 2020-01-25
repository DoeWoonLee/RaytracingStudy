#include "stdafx.h"
#include "Ray.h"

CRay::CRay()
{
}

CRay::CRay(vec3 vOrigin, vec3 vDirection)
	: m_vOrigin(vOrigin), m_vDirection(vDirection)
{
	m_vDirection.Normalize();
}

CRay::CRay(vec3 vOrigin, vec3 vDirection, bool noInit)
	: m_vOrigin(vOrigin), m_vDirection(vDirection)
{
}

const vec3 & CRay::GetOrigin() const
{
	return m_vOrigin;
}

void CRay::GetOrigin(XMVECTOR & sOut) const
{
	sOut = XMLoadFloat3(&m_vOrigin);
}

XMVECTOR CRay::GetSOrigin(void) const
{
	return XMLoadFloat3(&m_vOrigin);
}

const vec3 & CRay::GetDirection() const
{
	return m_vDirection;
}

void CRay::GetDirection(XMVECTOR & sOut) const
{
	sOut = XMLoadFloat3(&m_vDirection);
}

XMVECTOR CRay::GetSDirection(void) const
{
	return XMLoadFloat3(&m_vDirection);
}

void CRay::SetOrigin(const XMVECTOR & sOrigin)
{
	m_vOrigin.LoadSIMD(sOrigin);
}

void CRay::SetOrigin(const vec3 & vOrigin)
{
	m_vOrigin = vOrigin;
}

void CRay::SetDirection(const XMVECTOR & sDirection)
{
	m_vDirection.LoadSIMD(sDirection);
}

void CRay::SetDirection(const vec3 & vDirection)
{
	m_vDirection = vDirection;
}

vec3 CRay::PointAtParameter(const float & fTime) const
{
	return vec3(m_vOrigin.ToSIMD() + fTime * m_vDirection.ToSIMD());
}
