#include "stdafx.h"
#include "Material.h"
#include "FieldObject.h"

void CMaterial::SetOwner(CFieldObject * pOwner)
{
	m_pOwner = pOwner;
}
