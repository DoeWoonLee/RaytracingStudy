#pragma once

#include <list>

struct KD_TREENODE
{
	//vec3 vMin, vMax;
	KD_TREENODE* pLeft, *pRight;
	float fPos;
	AXIS eAxis;

	KD_TREENODE(void) : pLeft(nullptr), pRight(nullptr) {}
};
struct KD_TREENODE_LEAF : public KD_TREENODE
{
public:
	UINT* pTriArray;
	UINT uiTriCnt;
public:
	KD_TREENODE_LEAF(void) {  }
	KD_TREENODE_LEAF(KD_TREENODE_LEAF& Instance) :
		pTriArray(Instance.pTriArray),
		uiTriCnt(Instance.uiTriCnt)
	{
	}
	virtual ~KD_TREENODE_LEAF(void)
	{
		delete[] pTriArray;
	}
};
enum TREE_DIRECTION
{
	TREE_LEFT,
	TREE_RIGHT,
	TREE_END
};
template<class T>
struct STACK_INFO
{
	XMVECTOR vMin;
	XMVECTOR vMax;
	std::list<T> ListTriIdx;
	KD_TREENODE* pChildOfParent;
	TREE_DIRECTION eTreeType;
	unsigned short usStack;
	STACK_INFO(void) : pChildOfParent(nullptr), eTreeType(TREE_END), usStack(0) {}
};
enum TRI_EVENT : unsigned char
{
	TRI_MIN,
	TRI_MAX
};
struct SPLIT_PLANE
{
	float fValue;
	TRI_EVENT eEvent;
};
struct KD_CYCLE
{
	vec3 vMax;
	vec3 vMin;
	KD_TREENODE* pkD_Tree;

	void Init(const vec3& _vMax, const vec3& _vMin, KD_TREENODE* _pkD_Tree)
	{
		vMax = _vMax;
		vMin = _vMin;
		pkD_Tree = _pkD_Tree;
	}
};
struct AABB_2D_SPLIT
{
	float fMin;
	float fMax;
	float fSplit;
};
struct LEAF_SORT
{
	KD_TREENODE_LEAF* pLeafNode;
	float fLength;
};