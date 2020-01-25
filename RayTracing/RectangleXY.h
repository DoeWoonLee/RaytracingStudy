#pragma once

#include "Resources.h"
#include "Public.h"
#include "MemoryPool.h"

class CTransform;
class CRectangleXY : public CResources
{
public:
	CRectangleXY(const float& fWidth, const float& fHeight);
protected:
	virtual ~CRectangleXY();
public:
	DECLARE_CREATE_BY_MEMORYPOOL(CRectangleXY, CMemoryPool::OBJECT)

	virtual void MakeMinMax() final;
	virtual bool Hit(const CRay& inRay, float& fMin, float& fMax, HitRecord& hitRecord)const final;
	virtual float PdfValue(const vec3& vHitPos,
		const vec3& vScale ,
		const vec3& vOutRayDir,
		const vec3& vOutWRayDir)const override;
	virtual vec3 GenerateRandDirinRes(const vec3 & vWorldPos, const CTransform* pTransform) const override;
private:
	float m_fWidth;
	float m_fHeight;
};