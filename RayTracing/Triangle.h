#pragma once

#include "Resources.h"
#include "Public.h"
#include "MemoryPool.h"

class CTriangle : public CResources
{
public:
	explicit CTriangle();
	explicit CTriangle(vec3 v0, vec3 v1, vec3 v2);
protected:
	virtual ~CTriangle();
public:
	DECLARE_CREATE_BY_MEMORYPOOL(CTriangle, CMemoryPool::OBJECT)

	virtual void MakeMinMax();
	virtual bool Hit(const CRay& inRay, float& fMin, float& fMax, HitRecord& hitRecord)const final;
private:
	vec3 m_vPos[TRI_END];
	vec3 m_vNormal;
};