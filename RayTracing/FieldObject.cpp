#include "stdafx.h"
#include "FieldObject.h"
#include "MemoryPool.h"
#include "Sphere.h"
#include "Lambertain.h"
#include "Transform.h"
#include "Utility.h"
#include "AABB.h"

CFieldObject::CFieldObject(void)
{
	// Default Create
	// Sphere
	m_pResource =  CMemoryPool::Allocate<CSphere>::NEW(CMemoryPool::OBJECT);
	// Lambertain
	m_pMaterial = CMemoryPool::Allocate<CLambertain>::NEW(CMemoryPool::OBJECT);
	// Origin Coordinate
	m_pTransform = CMemoryPool::Allocate<CTransform>::NEW(CMemoryPool::OBJECT);
	// AABB
	m_pAABB = CAABB::Create(m_pResource, m_pTransform);
}

CFieldObject::CFieldObject(const vec3 & vPos)
{
	// Default Create
	// Sphere
	m_pResource = CMemoryPool::Allocate<CSphere>::NEW(CMemoryPool::OBJECT);
	// Lambertain
	m_pMaterial = CMemoryPool::Allocate<CLambertain>::NEW(CMemoryPool::OBJECT);
	// Origin Coordinate
	m_pTransform = CMemoryPool::Allocate<CTransform>::NEW(CMemoryPool::OBJECT, vPos);
	// AABB
	m_pAABB = CAABB::Create(m_pResource, m_pTransform);
}

CFieldObject::CFieldObject(CTransform * pTransform):
	m_pTransform(pTransform)
{
	// Sphere
	m_pResource = CMemoryPool::Allocate<CSphere>::NEW(CMemoryPool::OBJECT);
	// Lambertain
	m_pMaterial = CMemoryPool::Allocate<CLambertain>::NEW(CMemoryPool::OBJECT);

	// AABB
	m_pAABB = CAABB::Create(m_pResource, m_pTransform);
}

CFieldObject::CFieldObject(CTransform * pTransform, CResources * pResource, CMaterial * pMaterial):
	m_pTransform(pTransform), m_pResource(pResource), m_pMaterial(pMaterial)
{
	// AABB
	m_pAABB = CAABB::Create(m_pResource, m_pTransform);
}

thread_local CRay InverseRay;
thread_local vec3 vMaterialColor;


bool CFieldObject::Hit(HitRecord& Record, const CRay & inRay,float & fMin, float & fMax)
{
	
	if(m_pAABB->Hit(inRay, fMin, fMax, Record))
	{
		m_pTransform->InverseRay(inRay, InverseRay);

		if (m_pResource->Hit(InverseRay, fMin, fMax, Record))
		{
			fMax = Record.fTime;
			Record.vPos = inRay.PointAtParameter(fMax);

			return true;
		}
	}
	
	

	return false;
}

bool CFieldObject::Scatter(HitRecord & Record, const CRay & inRay, CRay & outRay, vec3 & vColor) const
{
	m_pTransform->WorldNormal(Record.vNormal);

	Record.vPos = inRay.PointAtParameter(Record.fTime);

	return m_pMaterial->Scatter(Record, inRay, outRay, vColor);
}

vec3 CFieldObject::Emitted(const vec3 & vPos)
{
	return m_pMaterial->Emitted(vPos);
}

const CAABB * CFieldObject::GetAABB(void)
{
	return m_pAABB;
}
