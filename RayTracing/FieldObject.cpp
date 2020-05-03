#include "stdafx.h"
#include "FieldObject.h"
#include "MemoryPool.h"
#include "Sphere.h"
#include "Lambertain.h"
#include "Transform.h"
#include "Utility.h"
#include "AABB.h"
#include "LogMgr.h"
#include "CosinePdf.h"
#include "EmittedObjectsMgr.h"

CFieldObject::CFieldObject(void)
{
	// Default Create
	// Sphere
	m_pResource =  CMemoryPool::New<CSphere>(CMemoryPool::OBJECT);
	// Lambertain
	m_pMaterial = CMemoryPool::New<CLambertain>(CMemoryPool::OBJECT);
	m_pMaterial->SetOwner(this);
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
	m_pMaterial->SetOwner(this);
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
	m_pMaterial->SetOwner(this);
	// AABB
	m_pAABB = CAABB::Create(m_pResource, m_pTransform);
}
CFieldObject::CFieldObject(const std::wstring & Name, CTransform * pTransform, CResources * pResource, CMaterial * pMaterial) :
	m_Name(Name), m_pTransform(pTransform), m_pResource(pResource), m_pMaterial(pMaterial)
{
	// AABB
	m_pAABB = CAABB::Create(m_pResource, m_pTransform);
	m_pResource->AddRef();
	m_pMaterial->SetOwner(this);
}
CFieldObject::CFieldObject(CTransform * pTransform, CResources * pResource, CMaterial * pMaterial) :
	m_pTransform(pTransform), m_pResource(pResource), m_pMaterial(pMaterial)
{
	// AABB
	m_pAABB = CAABB::Create(m_pResource, m_pTransform);
	m_pResource->AddRef();
	m_pMaterial->SetOwner(this);
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
			Record.localRay = InverseRay;

			return true;
		}
	}

	return false;
}

bool CFieldObject::Scatter(HitRecord & hRec,
	const CRay & inRay,
	CRay & outRay,
	vec3 & vColor, 
	float& fPdf) const
{
	ScatterRecord sRec;
	float fScatteringPdf;
	// 월드로 변환
	m_pTransform->WorldNormal(hRec.vNormal);
	m_pTransform->WorldNormal(hRec.vBiNormal);
	m_pTransform->WorldNormal(hRec.vTangent);
	
	hRec.vNormal.Normalize();
	hRec.vBiNormal.Normalize();
	hRec.vTangent.Normalize();


	hRec.vPos = inRay.PointAtParameter(hRec.fTime);

	bool bCollision = m_pMaterial->Scatter(hRec, inRay, sRec);
	if (bCollision)
	{
		if (sRec.bIsSpecular)
		{
			vColor = sRec.vAttenuation;
		}
		else
		{
			fPdf = 0.f;
			EMITTEDOBJECTMGR->GenerateScatterRay(outRay, fPdf, hRec.vPos, sRec);


			SAFE_DELETE(sRec.pPdfPtr);
			fScatteringPdf = m_pMaterial->ScatteringPdf(hRec, inRay, outRay);
			if (fPdf > 0.00001f)
			{
				vColor = sRec.vAttenuation / fPdf * fScatteringPdf;

				//if (vColor.x > 1.f)
				//{
				//	LOGMGR->WriteLog(L"Pdf : %lf ScatteringPdf : %lf Color.x : %lf", fPdf, fScatteringPdf, vColor.x);
				//}
			}
			else
			{
				vColor = vec3(0.f, 0.f, 0.f);
			}
			

		
		}
		
	}
	return bCollision;
}

vec3 CFieldObject::Emitted(const CRay& inRay, const HitRecord& hitRecord)
{
	return m_pMaterial->Emitted(inRay, hitRecord);
}

const CAABB * CFieldObject::GetAABB(void)
{
	return m_pAABB;
}

const std::wstring & CFieldObject::GetName(void)
{
	return m_Name;
}

const CTransform * CFieldObject::GetTransform(void)
{
	return m_pTransform;
}

const CResources * CFieldObject::GetResource(void)
{
	return m_pResource;
}

