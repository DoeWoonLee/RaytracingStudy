#pragma once

#include "Material.h"
#include "Transform.h"
#include "Resources.h"
#include "MemoryPool.h"
#include "AABB.h"

class CFieldObject
{
public:
	CFieldObject(void);
	CFieldObject(const vec3& vPos);
	CFieldObject(CTransform* pTransform);
	CFieldObject(CTransform* pTransform, CResources* pResource, CMaterial* pMaterial);
	CFieldObject(const std::wstring& Name, CTransform* pTransform, CResources* pResource, CMaterial* pMaterial);
	DECLARE_CREATE_BY_MEMORYPOOL(CFieldObject, CMemoryPool::OBJECT)

	bool Hit(HitRecord& Record, const CRay& inRay, float& fMin, float& fMax);
	bool Scatter(HitRecord& Record, const CRay& inRay, CRay& outRay, vec3& vColor) const;
	vec3 Emitted(const vec3& vPos);
	const CAABB* GetAABB(void);
	const std::wstring& GetName(void);
private:
	CTransform* m_pTransform;
	CResources* m_pResource;
	CMaterial* m_pMaterial;
	CAABB* m_pAABB;

	std::wstring m_Name;

};