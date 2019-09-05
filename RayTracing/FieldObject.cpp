#include "stdafx.h"
#include "FieldObject.h"

CFiledObject::CFiledObject(void)
{
	// Default Create
	// Sphere
	// Lambertain
	// Origin Coordinate
}

CFiledObject::CFiledObject(CTransform * pTransform, CResources * pResource, CMaterial * pMaterial):
	m_pTransform(pTransform), m_pResource(pResource), m_pMaterial(pMaterial)
{
}
