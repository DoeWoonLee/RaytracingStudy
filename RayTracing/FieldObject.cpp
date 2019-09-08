#include "stdafx.h"
#include "FieldObject.h"
#include "MemoryPool.h"
#include "Sphere.h"
#include "Lambertain.h"
#include "Transform.h"
#include "Utility.h"

CFiledObject::CFiledObject(void)
{
	// Default Create
	// Sphere
	m_pResource =  CMemoryPool::Allocate<CSphere>::NEW(CMemoryPool::OBJECT);
	// Lambertain
	m_pMaterial = CMemoryPool::Allocate<CLambertain>::NEW(CMemoryPool::OBJECT);
	// Origin Coordinate
	m_pTransform = CMemoryPool::Allocate<CTransform>::NEW(CMemoryPool::OBJECT);
}

CFiledObject::CFiledObject(const vec3 & vPos)
{
	// Default Create
	// Sphere
	m_pResource = CMemoryPool::Allocate<CSphere>::NEW(CMemoryPool::OBJECT);
	// Lambertain
	m_pMaterial = CMemoryPool::Allocate<CLambertain>::NEW(CMemoryPool::OBJECT);
	// Origin Coordinate
	m_pTransform = CMemoryPool::Allocate<CTransform>::NEW(CMemoryPool::OBJECT, vPos);
}

CFiledObject::CFiledObject(CTransform * pTransform):
	m_pTransform(pTransform)
{
	// Sphere
	m_pResource = CMemoryPool::Allocate<CSphere>::NEW(CMemoryPool::OBJECT);
	// Lambertain
	m_pMaterial = CMemoryPool::Allocate<CLambertain>::NEW(CMemoryPool::OBJECT);
}

CFiledObject::CFiledObject(CTransform * pTransform, CResources * pResource, CMaterial * pMaterial):
	m_pTransform(pTransform), m_pResource(pResource), m_pMaterial(pMaterial)
{
	
}

thread_local CRay InverseRay;
thread_local vec3 vMaterialColor;
thread_local HitRecord Record;

bool CFiledObject::Hit(const CRay & inRay, CRay& outRay, float & fMin, float & fMax, vec3 & vColor)
{
	

	
	m_pTransform->InverseRay(inRay, InverseRay);

	if (m_pResource->Hit(InverseRay, fMin, fMax, Record))
	{
		if(fMax > Record.fTime)
			fMax = Record.fTime;

		
		m_pTransform->WorldRecord(Record);
		if (m_pMaterial->Scatter(Record, outRay, vMaterialColor))
		{
			
			vColor.LoadSIMD( vMaterialColor.ToSIMD() * vColor.ToSIMD());
		}
		//vec3 vDir = outRay.GetDirection();
		vColor = (Record.vNormal + 1.f) * 0.5f;
		return true;
	}
	

	return false;
}
