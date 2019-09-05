#pragma once

#include "Material.h"
#include "Transform.h"
#include "Resources.h"

class CFiledObject
{
public:
	CFiledObject(void);
	CFiledObject(CTransform* pTransform, CResources* pResource, CMaterial* pMaterial);
private:
	CTransform* m_pTransform;
	CResources* m_pResource;
	CMaterial* m_pMaterial;
};