#include "stdafx.h"
#include "AABB.h"
#include "Transform.h"
#include "Resources.h"

CAABB::CAABB(void)
{
}

CAABB::CAABB(const vec3 & vMin, const vec3 & vMax):
	m_vMin(vMin), m_vMax(vMax)
{
}

CAABB::CAABB(const CResources* pResources, const CTransform * pTransform)
{
	Update(pResources, pTransform);
}

void CAABB::Update(const CResources* pResources, const CTransform * pTransform)
{
	m_vMin = pResources->Min();
	m_vMax = pResources->Max();

	vec3 arrAABB[8];

	arrAABB[0] = vec3(m_vMin.x, m_vMin.y, m_vMin.z);
	arrAABB[1] = vec3(m_vMin.x, m_vMin.y, m_vMax.z);
	arrAABB[2] = vec3(m_vMax.x, m_vMin.y, m_vMin.z);
	arrAABB[3] = vec3(m_vMax.x, m_vMin.y, m_vMax.z);

	arrAABB[4] = vec3(m_vMin.x, m_vMax.y, m_vMin.z);
	arrAABB[5] = vec3(m_vMin.x, m_vMax.y, m_vMax.z);
	arrAABB[6] = vec3(m_vMax.x, m_vMax.y, m_vMin.z);
	arrAABB[7] = vec3(m_vMax.x, m_vMax.y, m_vMax.z);

	m_vMin = vec3(FLT_MAX, FLT_MAX, FLT_MAX);
	m_vMax = vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	for (int i = 0; i < 8; ++i)
	{
		pTransform->WorldPos(arrAABB[i]);
		for (int j = 0; j < AXIS_END; ++j)
		{
			if (arrAABB[i][j] > m_vMax[j])
			{
				m_vMax[j] = arrAABB[i][j];
			}

			if (arrAABB[i][j] < m_vMin[j])
			{
				m_vMin[j] = arrAABB[i][j];
			}

		}
	}
}

bool CAABB::Hit(const CRay & inRay, float & fMin, float & fMax, HitRecord & hitRecord)
{
	float ffMin = fMin;
	float ffMax = fMax;


	vec3 vDir = inRay.GetDirection();
	vec3 vOrigin = inRay.GetOrigin();
	float fInvD = 0.f;
	float t0 = 0.f;
	float t1 = 0.f;

	for (int i = 0; i < AXIS_END; ++i)
	{
		fInvD = 1.0f / vDir[i];
		t0 = (m_vMin[i] - vOrigin[i]) * fInvD;
		t1 = (m_vMax[i] - vOrigin[i]) * fInvD;

		if (fInvD < 0.0f)
			std::swap(t0, t1);

		ffMin = t0 > ffMin ? t0 : ffMin;
		ffMax = t1 < ffMax ? t1 : ffMax;

		if (ffMax <= ffMin)
			return false;
	}

	return true;
}

const vec3 & CAABB::Min() const
{
	return m_vMin;
}

const vec3 & CAABB::Max() const
{
	return m_vMax;
}
