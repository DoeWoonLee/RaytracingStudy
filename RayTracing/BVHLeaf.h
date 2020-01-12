#pragma once


#include "BVHTree.h"
#include "MemoryPool.h"

class CFieldObject;
class CBVHLeaf : public CBVHTree
{
public:
	explicit CBVHLeaf(CFieldObject* pFieldObject);
public:
	//explicit CBVHLeaf(CFieldObject* pFieldObject) {}

	DECLARE_CREATE_BY_MEMORYPOOL(CBVHLeaf, CMemoryPool::BVH_TREE)

	virtual bool Hit(const CRay& inRay, float& fMin, float& fMax, HitRecord& hitRecord, CFieldObject** pFieldObject);
	virtual const CAABB* GetAABB(void)const;
};