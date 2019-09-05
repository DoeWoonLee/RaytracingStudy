#include "stdafx.h"
#include "Camera.h"
#include <math.h>

CCamera::CCamera(vec3 vEye, vec3 vAt, vec3 vUp, float fAspect, float fFov) : m_vEye(vEye)
{
	XMVECTOR u, v, w;

	XMVECTOR sEye = XMLoadFloat3(&vEye);
	XMVECTOR sAt = XMLoadFloat3(&vAt);
	XMVECTOR sUp = XMLoadFloat3(&vUp);

	float fTheta = fFov * XM_PI / 180.f;
	float fHalfHeight = tanf(fTheta / 2.f);
	float fHalfWidth = fAspect * fHalfHeight;

	w = XMVector3Normalize(sEye - sAt);
	u = XMVector3Normalize(XMVector3Cross(sUp, w));
	v = XMVector3Cross(w, u);

	XMStoreFloat3(&m_vLowerLeftCorner ,sEye - fHalfWidth * u - fHalfHeight * v - w);
	XMStoreFloat3(&m_vHorizontal, 2.f * fHalfWidth * u);
	XMStoreFloat3(&m_vVertical, 2.f * fHalfHeight * v);
}

CRay CCamera::GetRay(float & u, float & v)
{
	return CRay(m_vEye, vec3(m_vLowerLeftCorner.ToSIMD() + u * m_vHorizontal.ToSIMD() + v * m_vVertical.ToSIMD() - m_vEye.ToSIMD()));
}
