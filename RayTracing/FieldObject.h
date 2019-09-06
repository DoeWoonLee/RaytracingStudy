#pragma once

#include "Material.h"
#include "Transform.h"
#include "Resources.h"

class CFiledObject
{
public:
	CFiledObject(void);
	CFiledObject(const vec3& vPos);
	CFiledObject(CTransform* pTransform, CResources* pResource, CMaterial* pMaterial);

	bool Hit(const CRay& inRay, CRay& outRay, float& fMin, float& fMax, vec3& vColor);

private:
	CTransform* m_pTransform;
	CResources* m_pResource;
	CMaterial* m_pMaterial;
};