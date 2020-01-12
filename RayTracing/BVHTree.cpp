#include "stdafx.h"
#include "BVHTree.h"
#include "FieldObject.h"
#include "MathUtility.h"
#include "BVHLeaf.h"
#include "BVHLeaf.h"

//CBVHTree::CBVHTree(UINT uiObjectCnt)
//{
//
//}

int BoxXCompare(const void* a, const void* b)
{
	if ((*(CFieldObject**)a)->GetAABB()->Min().x -
		(*(CFieldObject**)b)->GetAABB()->Max().x < 0.0f)
		return -1;
	else
		return 1;
}
int BoxYCompare(const void* a, const void* b)
{
	if ((*(CFieldObject**)a)->GetAABB()->Min().y -
		(*(CFieldObject**)b)->GetAABB()->Max().y < 0.0f)
		return -1;
	else
		return 1;
}
int BoxZCompare(const void* a, const void* b)
{
	if ((*(CFieldObject**)a)->GetAABB()->Min().z -
		(*(CFieldObject**)b)->GetAABB()->Max().z < 0.0f)
		return -1;
	else
		return 1;
}

CBVHTree::CBVHTree(void)
{
}

CBVHTree::CBVHTree(const UINT uiStart, const UINT uiObjectCnt, std::vector<CFieldObject*>& vecFieldObjects)
{
	int iAxis = CMathUtility::XorShift32() % 3;

	if (X == iAxis)
		std::qsort(&vecFieldObjects[uiStart], uiObjectCnt, sizeof(CFieldObject*), BoxXCompare);
	else if (Y == iAxis)
		std::qsort(&vecFieldObjects[uiStart], uiObjectCnt, sizeof(CFieldObject*), BoxYCompare);
	else
		std::qsort(&vecFieldObjects[uiStart], uiObjectCnt, sizeof(CFieldObject*), BoxZCompare);

	if (uiObjectCnt == 1)
	{
		m_pLeft = m_pRight = CBVHLeaf::Create(vecFieldObjects[uiStart]);
	}
	else if (uiObjectCnt == 2)
	{
		m_pLeft = CBVHLeaf::Create(vecFieldObjects[uiStart]);
		m_pRight = CBVHLeaf::Create(vecFieldObjects[uiStart + 1]);
	}
	else
	{
		m_pLeft = CBVHTree::Create(uiStart, uiObjectCnt / 2, vecFieldObjects);
		m_pRight = CBVHTree::Create(uiStart + uiObjectCnt / 2, uiObjectCnt - uiObjectCnt / 2, vecFieldObjects);
	}

	m_pAABB = SurroundingBox(m_pLeft, m_pRight);
}
CAABB* CBVHTree::SurroundingBox(CBVHTree * pLeft, CBVHTree * pRight)
{
	const CAABB* pLAABB = pLeft->GetAABB();
	const CAABB* pRAABB = pRight->GetAABB();

	vec3 vMin(fmin(pLAABB->Min().x, pRAABB->Min().x),
		fmin(pLAABB->Min().y, pRAABB->Min().y),
		fmin(pLAABB->Min().z, pRAABB->Min().z));
	vec3 vMax(fmax(pLAABB->Max().x, pRAABB->Max().x),
		fmax(pLAABB->Max().y, pRAABB->Max().y),
		fmax(pLAABB->Max().z, pRAABB->Max().z));

	//return CMemoryPool::Allocate<CAABB>::NEW(CMemoryPool::BVH_TREE, vMin, vMax);
	return new CAABB(vMin, vMax);
}
const CAABB * CBVHTree::GetAABB(void) const
{
	return (CAABB*)m_pAABB;
}

bool CBVHTree::Hit(const CRay & inRay, float& fMin, float & fMax, HitRecord & hitRecord, CFieldObject** pFieldObject)
{
	HitRecord LRec;
	HitRecord RRec;

	bool bHitLeft = false;
	bool bHitRight = false;
	if (m_pLeft == m_pRight)
	{
		bHitLeft = m_pLeft->Hit(inRay, fMin, fMax, LRec, pFieldObject);
	}
	else
	{
		bHitLeft = m_pLeft->Hit(inRay, fMin, fMax, LRec, pFieldObject);
		bHitRight = m_pRight->Hit(inRay, fMin, fMax, RRec, pFieldObject);
	}
	
	if (bHitLeft & bHitRight)
	{
		if (LRec.fTime < RRec.fTime)
			hitRecord = LRec;
		else
			hitRecord = RRec;
	}
	else if (bHitLeft)
	{
		hitRecord = LRec;
	}
	else if (bHitRight)
	{
		hitRecord = RRec;
	}
	else
		return false;

	return true;
}

