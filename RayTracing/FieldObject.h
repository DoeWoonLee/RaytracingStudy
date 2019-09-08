#pragma once

#include "Material.h"
#include "Transform.h"
#include "Resources.h"
#include "MemoryPool.h"

class CFiledObject
{
public:
	CFiledObject(void);
	CFiledObject(const vec3& vPos);
	CFiledObject(CTransform* pTransform);
	CFiledObject(CTransform* pTransform, CResources* pResource, CMaterial* pMaterial);
	DECLARE_CREATE_BY_MEMORYPOOL(CFiledObject, CMemoryPool::OBJECT)

	bool Hit(const CRay& inRay, CRay& outRay, float& fMin, float& fMax, vec3& vColor);

private:
	CTransform* m_pTransform;
	CResources* m_pResource;
	CMaterial* m_pMaterial;
};