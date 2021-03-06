#include "stdafx.h"
#include "FieldObject.h"
#include "MemoryPool.h"
#include "Sphere.h"
#include "Lambertain.h"
#include "Transform.h"
#include "Utility.h"
#include "AABB.h"
#include "LogMgr.h"

CFieldObject::CFieldObject(void)
{
	// Default Create
	// Sphere
	m_pResource =  CMemoryPool::New<CSphere>(CMemoryPool::OBJECT);
	// Lambertain
	m_pMaterial = CMemoryPool::New<CLambertain>(CMemoryPool::OBJECT);
	// Origin Coordinate
	m_pTransform = CMemoryPool::New<CTransform>(CMemoryPool::OBJECT);
	// AABB
	m_pAABB = CAABB::Create(m_pResource, m_pTransform);
}

CFieldObject::CFieldObject(const vec3 & vPos)
{
	// Default Create
	// Sphere
	m_pResource = CMemoryPool::New<CSphere>(CMemoryPool::OBJECT);
	// Lambertain
	m_pMaterial = CMemoryPool::New<CLambertain>(CMemoryPool::OBJECT);
	// Origin Coordinate
	m_pTransform = CMemoryPool::New<CTransform>(CMemoryPool::OBJECT, vPos);
	// AABB
	m_pAABB = CAABB::Create(m_pResource, m_pTransform);
}

CFieldObject::CFieldObject(CTransform * pTransform):
	m_pTransform(pTransform)
{
	// Sphere
	m_pResource = CMemoryPool::New<CSphere>(CMemoryPool::OBJECT);
	// Lambertain
	m_pMaterial = CMemoryPool::New<CLambertain>(CMemoryPool::OBJECT);

	// AABB
	m_pAABB = CAABB::Create(m_pResource, m_pTransform);
}

CFieldObject::CFieldObject(const std::wstring & Name, CTransform * pTransform, CResources * pResource, CMaterial * pMaterial) :
	m_Name(Name), m_pTransform(pTransform), m_pResource(pResource), m_pMaterial(pMaterial)
{
	// AABB
	m_pAABB = CAABB::Create(m_pResource, m_pTransform);
	m_pResource->AddRef();
}
CFieldObject::CFieldObject(CTransform * pTransform, CResources * pResource, CMaterial * pMaterial) :
	m_pTransform(pTransform), m_pResource(pResource), m_pMaterial(pMaterial)
{
	// AABB
	m_pAABB = CAABB::Create(m_pResource, m_pTransform);
	m_pResource->AddRef();
}

CFieldObject::~CFieldObject(void)
{
#ifdef MEMORYPOOLUSE
	SAFE_RELEASE(m_pResource);
	SAFE_DELETE(m_pTransform);
	SAFE_DELETE(m_pMaterial);
	SAFE_DELETE(m_pAABB);
#endif
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

bool CFieldObject::Scatter(HitRecord & Record, const CRay & inRay, CRay & outRay, vec3 & vColor, float& fPdf) const
{
	m_pTransform->WorldNormal(Record.vNormal);

	Record.vPos = inRay.PointAtParameter(Record.fTime);

	bool bReturn = m_pMaterial->Scatter(Record, inRay, outRay, vColor, fPdf);
	if (bReturn)
	{
		vColor = vColor / fPdf * m_pMaterial->ScatteringPdf(inRay, outRay, Record);
	}
	return bReturn;
}

vec3 CFieldObject::Emitted(const vec3 & vPos)
{
	return m_pMaterial->Emitted(vPos);
}

const CAABB * CFieldObject::GetAABB(void)
{
	return m_pAABB;
}

const std::wstring & CFieldObject::GetName(void)
{
	return m_Name;
}

