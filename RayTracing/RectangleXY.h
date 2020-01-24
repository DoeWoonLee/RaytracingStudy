#pragma once

#include "Resources.h"
#include "Public.h"
#include "MemoryPool.h"

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
private:
	float m_fWidth;
	float m_fHeight;
};