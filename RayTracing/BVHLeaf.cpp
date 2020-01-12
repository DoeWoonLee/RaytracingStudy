#include "stdafx.h"
#include "BVHLeaf.h"
#include "FieldObject.h"

CBVHLeaf::CBVHLeaf(CFieldObject * pFieldObject)
{
	m_pAABB = pFieldObject;
}

bool CBVHLeaf::Hit(const CRay & inRay, float & fMin, float & fMax, HitRecord & hitRecord, CFieldObject** pFieldObject)
{
	bool bReturn = ((CFieldObject*)m_pAABB)->Hit(hitRecord, inRay, fMin, fMax);
	if (bReturn)
	{
		*pFieldObject = ((CFieldObject*)m_pAABB);
	}
	return bReturn;
}

const CAABB * CBVHLeaf::GetAABB(void) const
{
	return ((CFieldObject*)m_pAABB)->GetAABB();
}
