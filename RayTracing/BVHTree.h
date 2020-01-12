#pragma once

#include "AABB.h"
#include <vector>
#include "MemoryPool.h"

class CFieldObject;
class CBVHTree 
{
public:
	explicit CBVHTree(void);
	explicit CBVHTree(const UINT uiStart, const UINT uiObjectCnt, std::vector<CFieldObject*>& vecFieldObjects);
	DECLARE_CREATE_BY_MEMORYPOOL(CBVHTree, CMemoryPool::BVH_TREE)
private:
	CAABB* SurroundingBox(CBVHTree* pLeft, CBVHTree* pRight);
	virtual const CAABB* GetAABB(void)const;
public:
	virtual bool Hit(const CRay& inRay, float& fMin, float& fMax, HitRecord& hitRecord, CFieldObject** pFieldObject);
protected:
	CBVHTree* m_pLeft;
	CBVHTree* m_pRight;
	void* m_pAABB;
};