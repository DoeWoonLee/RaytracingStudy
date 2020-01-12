#include "stdafx.h"
#include "KDTree.h"
#include "Resources.h"

CKDTree::CKDTree(void): m_usDepth(0)
{
}

CKDTree::CKDTree(const std::vector<VERTEX>& vecVertex, const std::vector<INDEX16>* vecIndices, const vec3& vMin, const vec3& vMax) : m_usDepth(0)
{
	MakeKDTree(vecVertex, vecIndices, vMin, vMax);
	Create_CycleArr();
}

CKDTree::~CKDTree(void)
{

	std::list<KD_TREENODE*> TreeStackList;
	TreeStackList.push_back(m_pHead);

	KD_TREENODE* pDeletedNode = nullptr;

	while (!TreeStackList.empty())
	{
		pDeletedNode = *TreeStackList.rbegin();
		TreeStackList.pop_back();

		if (nullptr != pDeletedNode->pLeft)
			TreeStackList.push_back(pDeletedNode->pLeft);
		if (nullptr != pDeletedNode->pRight)
			TreeStackList.push_back(pDeletedNode->pRight);

		delete pDeletedNode;
		pDeletedNode = nullptr;
	}

	delete []m_pArrCycle;
}


thread_local UINT g_uiTopIdx = 0;

// KDTRee Make
//============== For Make KDTree ==============
bool HyperPlaneSeparation(XMVECTOR& vN, XMVECTOR& vP1, XMVECTOR& vP2, XMVECTOR vP3, XMVECTOR& vExtends)
{
	float fDot1 = XMVector3Dot(vN, vP1).m128_f32[X];
	float fDot2 = XMVector3Dot(vN, vP2).m128_f32[X];
	float fDot3 = XMVector3Dot(vN, vP3).m128_f32[X];

	float fMin = fmin(fDot3, fmin(fDot1, fDot2));
	float fMax = fmax(fDot3, fmax(fDot1, fDot2));


	XMVECTOR vAbsNormal = XMVectorAbs(vN);
	float fRadius = XMVector3Dot(vExtends, vAbsNormal).m128_f32[X];

	return -fRadius > fMax || fRadius < fMin;
}
bool HyperPlaneSeparation10(const STACK_INFO<USHORT>& tNowStack, const std::vector<VERTEX>& pVertices, const USHORT uiTriIdx,const std::vector<INDEX16> * pIndices, const FXMVECTOR& vExtends,const FXMVECTOR& vOrigin, XMVECTOR& vOutMin, XMVECTOR& vOutMax)
{
	vOutMin = XMVectorSet(
		/* X */min(pVertices[(*pIndices)[uiTriIdx]._1].vPos.x, min(pVertices[(*pIndices)[uiTriIdx]._2].vPos.x, pVertices[(*pIndices)[uiTriIdx]._3].vPos.x)),
		/* Y */min(pVertices[(*pIndices)[uiTriIdx]._1].vPos.y, min(pVertices[(*pIndices)[uiTriIdx]._2].vPos.y, pVertices[(*pIndices)[uiTriIdx]._3].vPos.y)),
		/* Z */min(pVertices[(*pIndices)[uiTriIdx]._1].vPos.z, min(pVertices[(*pIndices)[uiTriIdx]._2].vPos.z, pVertices[(*pIndices)[uiTriIdx]._3].vPos.z)), 0.f);

	vOutMax = XMVectorSet(
		/* X */max(pVertices[(*pIndices)[uiTriIdx]._1].vPos.x, max(pVertices[(*pIndices)[uiTriIdx]._2].vPos.x, pVertices[(*pIndices)[uiTriIdx]._3].vPos.x)),
		/* Y */max(pVertices[(*pIndices)[uiTriIdx]._1].vPos.y, max(pVertices[(*pIndices)[uiTriIdx]._2].vPos.y, pVertices[(*pIndices)[uiTriIdx]._3].vPos.y)),
		/* Z */max(pVertices[(*pIndices)[uiTriIdx]._1].vPos.z, max(pVertices[(*pIndices)[uiTriIdx]._2].vPos.z, pVertices[(*pIndices)[uiTriIdx]._3].vPos.z)), 0.f);

	if (vOutMin.m128_f32[X] >= tNowStack.vMax.m128_f32[X] || vOutMax.m128_f32[X] <= tNowStack.vMin.m128_f32[X]) return false;
	if (vOutMin.m128_f32[Y] >= tNowStack.vMax.m128_f32[Y] || vOutMax.m128_f32[Y] <= tNowStack.vMin.m128_f32[Y]) return false;
	if (vOutMin.m128_f32[Z] >= tNowStack.vMax.m128_f32[Z] || vOutMax.m128_f32[Z] <= tNowStack.vMin.m128_f32[Z]) return false;

	XMVECTOR vLocalPos1 = pVertices[(*pIndices)[uiTriIdx]._1].vPos.ToSIMD() - vOrigin;
	XMVECTOR vLocalPos2 = pVertices[(*pIndices)[uiTriIdx]._2].vPos.ToSIMD() - vOrigin;
	XMVECTOR vLocalPos3 = pVertices[(*pIndices)[uiTriIdx]._3].vPos.ToSIMD() - vOrigin;

	XMVECTOR vDir1 = vLocalPos2 - vLocalPos1;
	XMVECTOR vDir2 = vLocalPos3 - vLocalPos1;
	XMVECTOR vDir3 = vLocalPos3 - vLocalPos2;

	// GetNormal
	XMVECTOR vNormal = XMVector3Cross(vDir1, vDir2);

	XMVECTOR vXDir = XMVectorSet(1.f, 0.f, 0.f, 0.f);
	XMVECTOR vYDir = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	XMVECTOR vZDir = XMVectorSet(0.f, 0.f, 1.f, 0.f);
	XMVECTOR vHalfLength = vExtends * 0.5f;
	// Triangle Normal
	if (HyperPlaneSeparation(vNormal, vLocalPos1, vLocalPos2, vLocalPos3, vHalfLength)) return false;

	// 9 Cross Products
	if (HyperPlaneSeparation(XMVector3Cross(vDir1, vXDir), vLocalPos1, vLocalPos2, vLocalPos3, vHalfLength)) return false;
	if (HyperPlaneSeparation(XMVector3Cross(vDir1, vYDir), vLocalPos1, vLocalPos2, vLocalPos3, vHalfLength)) return false;
	if (HyperPlaneSeparation(XMVector3Cross(vDir1, vZDir), vLocalPos1, vLocalPos2, vLocalPos3, vHalfLength)) return false;

	if (HyperPlaneSeparation(XMVector3Cross(vDir2, vXDir), vLocalPos1, vLocalPos2, vLocalPos3, vHalfLength)) return false;
	if (HyperPlaneSeparation(XMVector3Cross(vDir2, vYDir), vLocalPos1, vLocalPos2, vLocalPos3, vHalfLength)) return false;
	if (HyperPlaneSeparation(XMVector3Cross(vDir2, vZDir), vLocalPos1, vLocalPos2, vLocalPos3, vHalfLength)) return false;

	if (HyperPlaneSeparation(XMVector3Cross(vDir3, vXDir), vLocalPos1, vLocalPos2, vLocalPos3, vHalfLength)) return false;
	if (HyperPlaneSeparation(XMVector3Cross(vDir3, vYDir), vLocalPos1, vLocalPos2, vLocalPos3, vHalfLength)) return false;
	if (HyperPlaneSeparation(XMVector3Cross(vDir3, vZDir), vLocalPos1, vLocalPos2, vLocalPos3, vHalfLength)) return false;

	return true;
}
int Compare_SP_Value(const void * pSP1, const void * pSP2)
{
	if (((SPLIT_PLANE*)(pSP1))->fValue > ((SPLIT_PLANE*)(pSP2))->fValue)
		return 1;
	else if (((SPLIT_PLANE*)(pSP1))->fValue == ((SPLIT_PLANE*)(pSP2))->fValue)
		return 0;
	return -1;
}
float Get_SAHBestCost(STACK_INFO<USHORT> & tStackInfo, SPLIT_PLANE & SP, FXMVECTOR& vExtends, float& fSurface_Area, char& Flag, int & iLeftTri, int & iRightTri, AXIS eAxis, float fTraversal_cost, float fIntersection_cost)
{
	if (SP.eEvent == TRI_MIN) { if (Flag == 1) { iRightTri--; Flag = 0; }  iLeftTri++; }
	else if (SP.eEvent == TRI_MAX) { if (Flag == 1) { iRightTri--; } Flag = 1; }

	if (SP.fValue <= tStackInfo.vMin.m128_f32[eAxis] || SP.fValue >= tStackInfo.vMax.m128_f32[eAxis]) return FLT_MAX;


	float fTempL = SP.fValue - tStackInfo.vMin.m128_f32[eAxis];
	float fTempR = tStackInfo.vMax.m128_f32[eAxis] - SP.fValue;

	float fSurface_Area_L;
	float fSurface_Area_R;

	switch (eAxis)
	{
	case X:
		fSurface_Area_L = 2.f * (fTempL * vExtends.m128_f32[Y] + vExtends.m128_f32[Y] * vExtends.m128_f32[Z] + vExtends.m128_f32[Z] * fTempL);
		fSurface_Area_R = 2.f * (fTempR * vExtends.m128_f32[Y] + vExtends.m128_f32[Y] * vExtends.m128_f32[Z] + vExtends.m128_f32[Z] * fTempR);
		break;
	case Y:
		fSurface_Area_L = 2.f * (vExtends.m128_f32[X] * fTempL + fTempL * vExtends.m128_f32[Z] + vExtends.m128_f32[Z] * vExtends.m128_f32[X]);
		fSurface_Area_R = 2.f * (vExtends.m128_f32[X] * fTempR + fTempR * vExtends.m128_f32[Z] + vExtends.m128_f32[Z] * vExtends.m128_f32[X]);
		break;
	case Z:
		fSurface_Area_L = 2.f * (vExtends.m128_f32[X] * vExtends.m128_f32[Y] + vExtends.m128_f32[Y] * fTempL + fTempL * vExtends.m128_f32[X]);
		fSurface_Area_R = 2.f * (vExtends.m128_f32[X] * vExtends.m128_f32[Y] + vExtends.m128_f32[Y] * fTempR + fTempR * vExtends.m128_f32[X]);
		break;
	}
	//if(SP.fValue)

	return fTraversal_cost +
		fSurface_Area_L / fSurface_Area * iLeftTri  * fIntersection_cost +
		fSurface_Area_R / fSurface_Area * iRightTri * fIntersection_cost;
}
void CreateLeafNode(KD_TREENODE ** ppNodePtr, std::list<USHORT>& ListTriIndex)
{

	if (nullptr != (*ppNodePtr))
	{
		delete *ppNodePtr;
		*ppNodePtr = nullptr;
	}

	*ppNodePtr = new KD_TREENODE_LEAF;
	(*ppNodePtr)->eAxis = AXIS_END;
	(*ppNodePtr)->fPos = 0.f;
	((KD_TREENODE_LEAF*)(*ppNodePtr))->uiTriCnt = (UINT)ListTriIndex.size();
	((KD_TREENODE_LEAF*)(*ppNodePtr))->pTriArray = new UINT[(UINT)ListTriIndex.size()];

	std::list<USHORT>::iterator iter = ListTriIndex.begin();
	for (USHORT i = 0; i < (USHORT)ListTriIndex.size(); ++i, ++iter)
	{
		((KD_TREENODE_LEAF*)(*ppNodePtr))->pTriArray[i] = *iter;
	}
}
// ============== Called Function
void CKDTree::MakeKDTree(const std::vector<VERTEX>& vecVertex, const std::vector<INDEX16>* vecIndices, const vec3& vMin, const vec3& vMax)
{
	STACK_INFO<USHORT> tStackInfo;
	m_vMax = tStackInfo.vMax = vMax.ToSIMD();
	m_vMin = tStackInfo.vMin = vMin.ToSIMD();

	XMVECTOR sMin ;
	XMVECTOR sMax ;

	std::list<STACK_INFO<USHORT>> ListStackInfo;
	STACK_INFO<USHORT> tNowStack;

	KD_TREENODE** ppNowNodePtr = &m_pHead;

	USHORT usMaxDepth = 0;
	// While Loop Values
	XMVECTOR vExtends, vOrigin;	AXIS eAxis;

	// Inerr For Loop Values
	UINT iTriIdx = 0;	std::list<USHORT> ListSelfVertexIdx;
	UINT uiIdxCnt = uiIdxCnt = (UINT)vecIndices->size();

	// Add All Index
	for (UINT i = 0; i < uiIdxCnt; ++i)	tStackInfo.ListTriIdx.push_back(i);
	ListStackInfo.push_back(tStackInfo);

	// For SAH
	std::vector<SPLIT_PLANE> vecSP_X, vecSP_Y, vecSP_Z; SPLIT_PLANE SP;
	float fSplitPos = 0.f;

	while (ListStackInfo.size() != 0)
	{
		ListSelfVertexIdx.clear();
		vecSP_X.clear(); vecSP_Y.clear(); vecSP_Z.clear();

		eAxis = AXIS_END;

		// Get Parent?
		tNowStack = (*ListStackInfo.rbegin()); ListStackInfo.pop_back();
		vecSP_X.reserve(tNowStack.ListTriIdx.size() * 2);
		vecSP_Y.reserve(tNowStack.ListTriIdx.size() * 2);
		vecSP_Z.reserve(tNowStack.ListTriIdx.size() * 2);

		// Bound Box Width
		vExtends = tNowStack.vMax - tNowStack.vMin;
		vOrigin = tNowStack.vMin + vExtends * 0.5f;

		std::list<USHORT>::iterator iter_begin = tNowStack.ListTriIdx.begin();
		std::list<USHORT>::iterator iter_end = tNowStack.ListTriIdx.end();

		// Step 1. Check Collision In Parent AABB ( hyperplane )
		for (; iter_begin != iter_end; ++iter_begin)
		{
			iTriIdx = (*iter_begin);

			if (!HyperPlaneSeparation10(tNowStack, vecVertex, iTriIdx, vecIndices, vExtends, vOrigin, sMin, sMax))
				continue;

			ListSelfVertexIdx.push_back(iTriIdx);

			// Step 2. Create AABB List by a Triangle In a Mesh
			SP.fValue = sMin.m128_f32[X]; SP.eEvent = TRI_MIN; vecSP_X.push_back(SP);
			SP.fValue = sMax.m128_f32[X]; SP.eEvent = TRI_MAX; vecSP_X.push_back(SP);
			SP.fValue = sMin.m128_f32[Y]; SP.eEvent = TRI_MIN; vecSP_Y.push_back(SP);
			SP.fValue = sMax.m128_f32[Y]; SP.eEvent = TRI_MAX; vecSP_Y.push_back(SP);
			SP.fValue = sMin.m128_f32[Z]; SP.eEvent = TRI_MIN; vecSP_Z.push_back(SP);
			SP.fValue = sMax.m128_f32[Z]; SP.eEvent = TRI_MAX; vecSP_Z.push_back(SP);
		}
		if (ListSelfVertexIdx.empty())	continue;

		// Step 3. Sort List
		std::qsort(&vecSP_X[0], vecSP_X.size(), sizeof(SPLIT_PLANE), Compare_SP_Value); // x
		std::qsort(&vecSP_Y[0], vecSP_Y.size(), sizeof(SPLIT_PLANE), Compare_SP_Value); // y
		std::qsort(&vecSP_Z[0], vecSP_Z.size(), sizeof(SPLIT_PLANE), Compare_SP_Value); // z

																								  // Step 4. Find Cost/AXIS/SplitPos
		UINT uiRoofCnt = (UINT)vecSP_X.size();
		int iTriCnt = (int)ListSelfVertexIdx.size() - 1;
		float fCost = FLT_MAX;
		float fBest_Cost = FLT_MAX;
		float fNoSplit_Cost = iTriCnt* 1.f /* Intersection_cost*/;
		char FlagX = 0; char FlagY = 0; char FlagZ = 0;
		int iLeftTriX = 0, iRightTriX = iTriCnt;
		int iLeftTriY = 0, iRightTriY = iTriCnt;
		int iLeftTriZ = 0, iRightTriZ = iTriCnt;

		float fSurface_Area = 2.0f *
			(vExtends.m128_f32[X] * vExtends.m128_f32[Y] +
				vExtends.m128_f32[Y] * vExtends.m128_f32[Z] +
				vExtends.m128_f32[X] * vExtends.m128_f32[Z]);
		for (UINT i = 0; i < uiRoofCnt; ++i)
		{
			// X =======================================================================================
			fCost = Get_SAHBestCost(tNowStack, vecSP_X[i], vExtends, fSurface_Area, FlagX, iLeftTriX, iRightTriX, X, 1.f, 1.f);
			if (fCost <= fBest_Cost)
			{
				fBest_Cost = fCost;	eAxis = X;	fSplitPos = vecSP_X[i].fValue;
			}
			//Y ========================================================================================
			fCost = Get_SAHBestCost(tNowStack, vecSP_Y[i], vExtends, fSurface_Area, FlagY, iLeftTriY, iRightTriY, Y, 1.f, 1.f);
			if (fCost <= fBest_Cost)
			{
				fBest_Cost = fCost;	eAxis = Y;	fSplitPos = vecSP_Y[i].fValue;
			}
			//Z ========================================================================================
			fCost = Get_SAHBestCost(tNowStack, vecSP_Z[i], vExtends, fSurface_Area, FlagZ, iLeftTriZ, iRightTriZ, Z, 1.f, 1.f);
			if (fCost <= fBest_Cost)
			{
				fBest_Cost = fCost;	eAxis = Z;	fSplitPos = vecSP_Z[i].fValue;
			}
		}

		if (eAxis == AXIS_END)
			continue;
		// Step 5. Split
		// Check Direction ( 주소 가져 오기 )
		if (0 != tNowStack.usStack)
		{
			if (TREE_LEFT == tNowStack.eTreeType)
				ppNowNodePtr = &((tNowStack.pChildOfParent)->pLeft);
			else if (TREE_RIGHT == tNowStack.eTreeType)
				ppNowNodePtr = &((tNowStack.pChildOfParent)->pRight);
		}
		// Terminate ,Beacuase Can't Split Mesh ( 삼각형이 1개로 더이상 쪼갤 필요가 없을 때 )
		if (1 == ListSelfVertexIdx.size() || fNoSplit_Cost < fBest_Cost)
		{
			CreateLeafNode(ppNowNodePtr, ListSelfVertexIdx);	continue;
		}
		// Create Node ( 스택에 들어갈 값들을 초기화 하는 부분 )
		(*ppNowNodePtr) = new KD_TREENODE;
		(*ppNowNodePtr)->eAxis = eAxis;

		// Default Value Setting
		tStackInfo.ListTriIdx = ListSelfVertexIdx;		tStackInfo.usStack = tNowStack.usStack + 1;
		tStackInfo.pChildOfParent = (*ppNowNodePtr);	(*ppNowNodePtr)->fPos = fSplitPos;
		tStackInfo.vMax = tNowStack.vMax; tStackInfo.vMin = tNowStack.vMin;

		// Left
		tStackInfo.eTreeType = TREE_LEFT; tStackInfo.vMax.m128_f32[eAxis] = fSplitPos;
		ListStackInfo.push_back(tStackInfo);

		// Right
		tStackInfo.vMax = tNowStack.vMax;
		tStackInfo.eTreeType = TREE_RIGHT; tStackInfo.vMin.m128_f32[eAxis] = fSplitPos;
		ListStackInfo.push_back(tStackInfo);

		// Update Depth
		if (usMaxDepth < tStackInfo.usStack)
			usMaxDepth = tStackInfo.usStack;

		// Termination
		if (tStackInfo.usStack == m_usDepth)
		{
			CreateLeafNode(ppNowNodePtr, ListSelfVertexIdx);
			ListStackInfo.pop_back(); ListStackInfo.pop_back();
		}
	}

	if (usMaxDepth > m_usDepth)
	{
		m_usDepth = usMaxDepth;
	}
}
void CKDTree::MakeKDTreeMiddle(const std::vector<VERTEX>& vecVertex, const std::vector<INDEX16>* vecIndices, const vec3 & vMin, const vec3 & vMax)
{
	STACK_INFO<USHORT> tStackInfo;
	tStackInfo.vMax = vMax.ToSIMD();
	tStackInfo.vMin = vMin.ToSIMD();

	std::list<STACK_INFO<USHORT>> ListStackInfo;	STACK_INFO<USHORT> tNowStack;

	XMVECTOR sMin, sMax;

	KD_TREENODE** ppNowNodePtr = &m_pHead;
	USHORT usMaxDepth = 0;

	// While Loop Values
	XMVECTOR vExtends, vOrigin; AXIS eAxis;

	// Inerr For Loop Values
	UINT iTriIdx = 0;

	std::list<USHORT> ListSelfVertexIdx;

	// Bound Box Stack
	float fMiddleValue;

	// Root Parent
	for (UINT i = 0; i < vecIndices->size(); ++i)
	{
		tStackInfo.ListTriIdx.push_back(i);
	}
	ListStackInfo.push_back(tStackInfo);
	// Stack Roof
	// Stack Info
	// AABB Size, 
	while (ListStackInfo.size() != 0)
	{
		ListSelfVertexIdx.clear();

		// Get Parent?
		tNowStack = (*ListStackInfo.rbegin()); ListStackInfo.pop_back();

		// Bound Box Width
		vExtends = tNowStack.vMax - tNowStack.vMin;
		vOrigin = tNowStack.vMin + vExtends * 0.5f;

		// Select Split Axis. It is selected by Box Length Each Axis
		eAxis = (vExtends.m128_f32[X] > vExtends.m128_f32[Y] &&
			vExtends.m128_f32[X] > vExtends.m128_f32[Z]) ?
			X : (vExtends.m128_f32[Y] > vExtends.m128_f32[Z] ? Y : Z);

		std::list<USHORT>::iterator iter_begin = tNowStack.ListTriIdx.begin();
		std::list<USHORT>::iterator iter_end = tNowStack.ListTriIdx.end();

		for (; iter_begin != iter_end; ++iter_begin)
		{
			iTriIdx = (*iter_begin);

			if (!HyperPlaneSeparation10(tNowStack, vecVertex, iTriIdx, vecIndices, vExtends, vOrigin, sMin, sMax))
				continue;

			ListSelfVertexIdx.push_back(iTriIdx);
		}

		// Check Direction
		if (0 != tNowStack.usStack)
		{
			if (TREE_LEFT == tNowStack.eTreeType)
			{
				ppNowNodePtr = &(tNowStack.pChildOfParent)->pLeft;
			}
			else if (TREE_RIGHT == tNowStack.eTreeType)
			{
				ppNowNodePtr = &(tNowStack.pChildOfParent)->pRight;
			}
		}

		// Terminate ,Beacuase Can't Split Mesh
		if (1 == ListSelfVertexIdx.size())
		{
			CreateLeafNode(ppNowNodePtr, ListSelfVertexIdx);

			continue;
		}
		else if (ListSelfVertexIdx.empty())
		{
			continue;
		}

		// Create Now Node
		(*ppNowNodePtr) = new KD_TREENODE;
		(*ppNowNodePtr)->eAxis = eAxis;

		// Create Child Stack

		tStackInfo.ListTriIdx = ListSelfVertexIdx;
		tStackInfo.usStack = tNowStack.usStack + 1;
		tStackInfo.pChildOfParent = (*ppNowNodePtr);

		fMiddleValue = (tNowStack.vMax.m128_f32[eAxis] + tNowStack.vMin.m128_f32[eAxis]) * 0.5f;
		(*ppNowNodePtr)->fPos = fMiddleValue;

		// Left 
		tStackInfo.vMax = tNowStack.vMax;	tStackInfo.vMin = tNowStack.vMin;
		tStackInfo.vMax.m128_f32[eAxis] = fMiddleValue;
		tStackInfo.eTreeType = TREE_LEFT;
		ListStackInfo.push_back(tStackInfo);

		// Right
		tStackInfo.vMax = tNowStack.vMax;
		tStackInfo.vMin.m128_f32[eAxis] = fMiddleValue;
		tStackInfo.eTreeType = TREE_RIGHT;
		ListStackInfo.push_back(tStackInfo);

		// Update Depth
		if (usMaxDepth < tStackInfo.usStack)
			usMaxDepth = tStackInfo.usStack;
		// Termination
		if (tStackInfo.usStack == m_usDepth)
		{
			CreateLeafNode(ppNowNodePtr, ListSelfVertexIdx);
			ListStackInfo.pop_back(); ListStackInfo.pop_back();
		}
	}

	if (usMaxDepth < m_usDepth)
	{
		m_usDepth = usMaxDepth;
	}
}
void CKDTree::Create_CycleArr(void)
{
	std::list<KD_TREENODE*> TreeStackList;
	TreeStackList.push_back(m_pHead);

	KD_TREENODE* pNowNode = nullptr;
	UINT uiMaxStackSize = 0;

	while (!TreeStackList.empty())
	{
		pNowNode = *TreeStackList.rbegin();
		TreeStackList.pop_back();

		if (nullptr != pNowNode->pLeft)
			TreeStackList.push_back(pNowNode->pLeft);
		if (nullptr != pNowNode->pRight)
			TreeStackList.push_back(pNowNode->pRight);

		if (uiMaxStackSize < TreeStackList.size())
			uiMaxStackSize = (UINT)TreeStackList.size();
	}

	m_uiCycleCnt = uiMaxStackSize;
	m_pArrCycle = new KD_CYCLE[m_uiCycleCnt];
}
// Ray Intersection

thread_local KD_CYCLE g_pArrCycle[100];

std::list<LEAF_SORT> CKDTree::Ray_Intersection(const CRay& inRay/*, UINT& uiOutPickTri*/)
{
	KD_CYCLE tCycle;
	KD_CYCLE tNext;

	g_pArrCycle[g_uiTopIdx++].Init(m_vMax, m_vMin, m_pHead);

	AABB_2D_SPLIT tAABB_2D_Split;
	//UINT uiPickTri = uiOutPickTri = -1;
	// Length Compare
	float fBeforeLength = -FLT_MAX;
	float fShortestLength = FLT_MAX;
	float fFartestLength = 0.f;
	// Return Value
	bool bFind = false;

	std::list<LEAF_SORT> LeafList;

	//Memo 극한의 최적화 : 원점 기준 가장 가까운 aabb블럭들을 찾고, 그 중 가장 가까운 블럭들에서 TriIntersection검사를 한 후 만약 삼각형이 있다면 거기서 종료 시키면 된다. 나중에 속도(최적화)가 더 필요 하다면 코드 수정
	while (0 != g_uiTopIdx)
	{
		tCycle = g_pArrCycle[--g_uiTopIdx];

		// Leaf Node
		if (!Check_Ray_AABB(tCycle, inRay, tAABB_2D_Split, g_pArrCycle))
		{
			continue;
		}

		Add_Child(tCycle, tAABB_2D_Split, LeafList, inRay, fShortestLength, fFartestLength, g_pArrCycle);
	}
	// Sort ===================================== (수정해야함)
	std::list<LEAF_SORT>::iterator iter = LeafList.begin();
	std::list<LEAF_SORT>::iterator iter_end = LeafList.end();
	LEAF_SORT tTemp;

	for (; iter != iter_end; ++iter)
	{
		std::list<LEAF_SORT>::iterator iter_inner = iter;
		for (++iter_inner; iter_inner != iter_end; ++iter_inner)
		{
			if (iter->fLength > iter_inner->fLength)
			{
				tTemp = *iter_inner;
				*iter_inner = *iter;
				*iter = tTemp;
			}
		}

	}
	//delete [] pArrCycle;

	return LeafList;
}

bool CKDTree::Check_Ray_AABB(const KD_CYCLE & tCycle, const CRay & inRay, AABB_2D_SPLIT & _tOutS_AABBSplit, KD_CYCLE* pArrCycle)
{
	vec3 vInvDir = 1.f / inRay.GetDirection();
	int iSign[3] = { vInvDir.x < 0,vInvDir.y < 0 ,vInvDir.z < 0 };
	vec3 vBounds[2] = { tCycle.vMin, tCycle.vMax };

	float fMin, fMax, fyMin, fyMax;

	switch (tCycle.pkD_Tree->eAxis)
	{
	case X:
		fMin = (vBounds[iSign[X]].x - inRay.GetOrigin().x) * vInvDir.x;
		fMax = (vBounds[1 - iSign[X]].x - inRay.GetOrigin().x) * vInvDir.x;

		fyMin = (vBounds[iSign[Y]].y - inRay.GetOrigin().y) * vInvDir.y;
		fyMax = (vBounds[1 - iSign[Y]].y - inRay.GetOrigin().y) * vInvDir.y;

		_tOutS_AABBSplit.fSplit = (tCycle.pkD_Tree->fPos - inRay.GetOrigin().x) * vInvDir.x;
		break;
	case Y:
		fMin = (vBounds[iSign[Y]].y - inRay.GetOrigin().y) * vInvDir.y;
		fMax = (vBounds[1 - iSign[Y]].y - inRay.GetOrigin().y) * vInvDir.y;

		fyMin = (vBounds[iSign[X]].x - inRay.GetOrigin().x) * vInvDir.x;
		fyMax = (vBounds[1 - iSign[X]].x - inRay.GetOrigin().x) * vInvDir.x;

		_tOutS_AABBSplit.fSplit = (tCycle.pkD_Tree->fPos - inRay.GetOrigin().y) * vInvDir.y;
		break;
	case Z:
		fMin = (vBounds[iSign[Z]].z - inRay.GetOrigin().z) * vInvDir.z;
		fMax = (vBounds[1 - iSign[Z]].z - inRay.GetOrigin().z) * vInvDir.z;

		fyMin = (vBounds[iSign[Y]].y - inRay.GetOrigin().y) * vInvDir.y;
		fyMax = (vBounds[1 - iSign[Y]].y - inRay.GetOrigin().y) * vInvDir.y;

		_tOutS_AABBSplit.fSplit = (tCycle.pkD_Tree->fPos - inRay.GetOrigin().z) * vInvDir.z;
		break;
	default:
		break;
	}

	if ((fMin > fyMax) || (fyMin > fMax))	return false;

	if (fMin > fyMin) _tOutS_AABBSplit.fMin = fMin;
	else _tOutS_AABBSplit.fMin = fyMin;

	if (fMax < fyMax) _tOutS_AABBSplit.fMax = fMax;
	else _tOutS_AABBSplit.fMax = fyMax;

	//if (Z == tCycle.pkD_Tree->eAxis) CMathMgr::Swap(_tOutS_AABBSplit.fMin, _tOutS_AABBSplit.fMax);

	return true;
}

void CKDTree::Add_Child(KD_CYCLE & tCycle, AABB_2D_SPLIT tAABB_2D_Split, std::list<LEAF_SORT>& LeafList, const CRay & tRay, float & fMinLength, float & fMaxLength, KD_CYCLE* pArrCycle)
{
	// Ray_Intersection에서 호추하는 함수
	// 역활 : 스택에 차일드 추가
	KD_CYCLE tNext;

	vec3 vDir = tRay.GetDirection();
	float fDirValue = vDir[tCycle.pkD_Tree->eAxis];


	if (fDirValue < 0.f)
	{
		if (tAABB_2D_Split.fSplit >= tAABB_2D_Split.fMin) // Left
		{
			if (nullptr != tCycle.pkD_Tree->pRight)
			{
				tNext = tCycle;

				tNext.pkD_Tree = tCycle.pkD_Tree->pRight;

				if (X == tCycle.pkD_Tree->eAxis)		tNext.vMin.x = tCycle.pkD_Tree->fPos;
				else if (Y == tCycle.pkD_Tree->eAxis)	tNext.vMin.y = tCycle.pkD_Tree->fPos;
				else if (Z == tCycle.pkD_Tree->eAxis)	tNext.vMin.z = tCycle.pkD_Tree->fPos;

				if (!Check_LeafNode(tNext, LeafList, tRay, fMinLength, fMaxLength, pArrCycle))
					pArrCycle[g_uiTopIdx++] = tNext;

			}
		}
		if (tAABB_2D_Split.fSplit <= tAABB_2D_Split.fMax) // Right
		{
			if (nullptr != tCycle.pkD_Tree->pLeft)
			{
				tNext = tCycle;

				tNext.pkD_Tree = tCycle.pkD_Tree->pLeft;

				if (X == tCycle.pkD_Tree->eAxis)		tNext.vMax.x = tCycle.pkD_Tree->fPos;
				else if (Y == tCycle.pkD_Tree->eAxis)	tNext.vMax.y = tCycle.pkD_Tree->fPos;
				else if (Z == tCycle.pkD_Tree->eAxis)	tNext.vMax.z = tCycle.pkD_Tree->fPos;

				if (!Check_LeafNode(tNext, LeafList, tRay, fMinLength, fMaxLength, pArrCycle))
				{
					pArrCycle[g_uiTopIdx++] = tNext;
				}
			}
		}
	}
	else
	{
		if (tAABB_2D_Split.fSplit >= tAABB_2D_Split.fMin) // Left
		{
			if (nullptr != tCycle.pkD_Tree->pLeft)
			{
				tNext = tCycle;

				tNext.pkD_Tree = tCycle.pkD_Tree->pLeft;

				if (X == tCycle.pkD_Tree->eAxis)		tNext.vMax.x = tCycle.pkD_Tree->fPos;
				else if (Y == tCycle.pkD_Tree->eAxis)	tNext.vMax.y = tCycle.pkD_Tree->fPos;
				else if (Z == tCycle.pkD_Tree->eAxis)	tNext.vMax.z = tCycle.pkD_Tree->fPos;

				if (!Check_LeafNode(tNext, LeafList, tRay, fMinLength, fMaxLength, pArrCycle))
				{
					pArrCycle[g_uiTopIdx++] = tNext;
				}
			}
		}
		if (tAABB_2D_Split.fSplit <= tAABB_2D_Split.fMax) // Right
		{
			if (nullptr != tCycle.pkD_Tree->pRight)
			{
				tNext = tCycle;

				tNext.pkD_Tree = tCycle.pkD_Tree->pRight;

				if (X == tCycle.pkD_Tree->eAxis)		tNext.vMin.x = tCycle.pkD_Tree->fPos;
				else if (Y == tCycle.pkD_Tree->eAxis)	tNext.vMin.y = tCycle.pkD_Tree->fPos;
				else if (Z == tCycle.pkD_Tree->eAxis)	tNext.vMin.z = tCycle.pkD_Tree->fPos;

				if (!Check_LeafNode(tNext, LeafList, tRay, fMinLength, fMaxLength, pArrCycle))
				{
					pArrCycle[g_uiTopIdx++] = tNext;
				}

			}
		}
	}
}

bool CKDTree::Check_LeafNode(KD_CYCLE & tLeaf, std::list<LEAF_SORT>& LeafList, const CRay & tRay, float & fMinLength, float & fMaxLength, KD_CYCLE* pArrCycle)
{
	// Add_Child에서 호출됨
	// 리프 노드인지 검사하고 리프 노드라면 거리를 정렬하면서 추가한다.

	if (nullptr != tLeaf.pkD_Tree->pLeft || nullptr != tLeaf.pkD_Tree->pRight)
		return false;

	vec3 vCenter = (tLeaf.vMax - tLeaf.vMin) / 2.f + tLeaf.vMin;
	float fLength = vCenter.Length(tRay.GetOrigin());

	LEAF_SORT tLeafSort = { (KD_TREENODE_LEAF*)tLeaf.pkD_Tree , fLength };
	LeafList.push_back(tLeafSort);

	return true;
}
