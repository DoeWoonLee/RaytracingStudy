#pragma once

#include "Vector3.h"
#include "Public.h"
#include "Vertex.h"
#include <vector>
#include "MemoryPool.h"
#include "KDTreeStructs.h"
#include "Ray.h"

class CResources;
class CKDTree
{
public:
	explicit CKDTree(void);
	explicit CKDTree(const std::vector<VERTEX>& vecVertex, const std::vector<INDEX16>* vecIndices, const vec3& vMin, const vec3& vMax);
	virtual ~CKDTree(void);

	DECLARE_CREATE_BY_MEMORYPOOL(CKDTree, CMemoryPool::KD_TREE)
private:
	void MakeKDTree(const std::vector<VERTEX>& vecVertex, const std::vector<INDEX16>* vecIndices, const vec3& vMin, const vec3& vMax);
	void MakeKDTreeMiddle(const std::vector<VERTEX>& vecVertex, const std::vector<INDEX16>* vecIndices, const vec3& vMin, const vec3& vMax);

	void Create_CycleArr(void);
public:
	std::list<LEAF_SORT> Ray_Intersection(const CRay& inRay/*, UINT& uiOutPickTri*/);
private://=====  Ray Check ======================
	bool Check_Ray_AABB(const KD_CYCLE& tCycle, const CRay& inRay, AABB_2D_SPLIT& _tOutS_AABBSplit, KD_CYCLE* pArrCycle);
	void Add_Child(KD_CYCLE & tCycle, AABB_2D_SPLIT tAABB_2D_Split, std::list<LEAF_SORT>& LeafList, const CRay& tRay, float& fMinLength, float& fMaxLength, KD_CYCLE* pArrCycle);
	bool Check_LeafNode(KD_CYCLE & tLeaf, std::list<LEAF_SORT>& LeafList, const CRay & tRay, float & fMinLength, float & fMaxLength, KD_CYCLE* pArrCycle);

private:
	KD_TREENODE* m_pHead = nullptr;
	USHORT m_usDepth = 0;
	vec3 m_vMin;
	vec3 m_vMax;
private: // For Ray InterSection
	KD_CYCLE* m_pArrCycle = nullptr;
	UINT m_uiCycleCnt = 0;

};