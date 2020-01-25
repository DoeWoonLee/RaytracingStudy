#include "stdafx.h"
#include "EmittedObjectsMgr.h"
#include "MathUtility.h"
#include "FieldObject.h"
#include "MixturePdf.h"
#include "HitRecord.h"

IMPLEMENT_SINGLETON(EmittedObjectsMgr)

EmittedObjectsMgr::EmittedObjectsMgr(void)
{
}
EmittedObjectsMgr::~EmittedObjectsMgr(void)
{
	m_EmittedObjects.clear();
}

void EmittedObjectsMgr::AddEmittedObject(CFieldObject * pFieldObject)
{
	m_EmittedObjects.push_back(pFieldObject);
}

void EmittedObjectsMgr::GenerateScatterRay(CRay & vOutRay,
	float & fPdfValue,
	const vec3 & vHitPos, 
	const ScatterRecord & sRec) const
{


	// vHitPos는 월드로 들어옴

	float fRand = CMathUtility::frand0to1() * m_EmittedObjects.size();

	CFieldObject* pRandObject = m_EmittedObjects[(int)fRand];

	const CResources* pResource = pRandObject->GetResource();
	const CTransform* pTransform = pRandObject->GetTransform();

	MixturePdf mixturePdf(pResource, sRec.pPdfPtr);

	// 로컬로 변환
	vec3 vEmitObjectLocal = pTransform->InversePos(vHitPos);

	bool bType;
	// Local 영역
	vec3 vEmitObjectWorldDir = mixturePdf.Generate(vHitPos, pTransform, bType);

	vec3 vEmitObjectLocalDir = vEmitObjectWorldDir;
	
	if (true == bType)
	{
		vEmitObjectLocalDir = pTransform->InverseNormal(vEmitObjectLocalDir);
		//vEmitObjectLocalDir.Normalize();

		fPdfValue = mixturePdf.Value(vEmitObjectLocal,
			pTransform->GetScale(),
			vEmitObjectLocalDir,
			vEmitObjectWorldDir);

		pTransform->WorldNormal(vEmitObjectLocalDir);
	}
	else
	{
		fPdfValue = mixturePdf.Value(vEmitObjectLocal,
			pTransform->GetScale(),
			vEmitObjectWorldDir,
			vEmitObjectWorldDir);
	}

	CRay scatterRay(vHitPos, vEmitObjectLocalDir);
	vOutRay = scatterRay;
}
