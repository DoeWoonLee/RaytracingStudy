#include "stdafx.h"
#include "Mesh.h"
#include "FBXLoader.h"
#include "KDTree.h"

CMesh::CMesh(std::string strFilePath)
	: m_pIndices(nullptr)
{
	if (FAILED(CFBXLoader::LoadFBXFile(strFilePath, this)))
	{
		//MessageBox(g_hWnd)
		
	}
	MakeMinMax();
}

CMesh::~CMesh(void)
{
	if (nullptr != m_pIndices)
		delete m_pIndices;
	if(nullptr != m_pKDTree)
		delete m_pKDTree;
}
void CMesh::MakeMinMax()
{
	UINT iVertexSize = (UINT)m_vecVertices.size();


	for (UINT i = 0; i < iVertexSize; ++i)
	{
		for (int j = 0; j < AXIS_END; ++j)
		{
			if (m_vecVertices[i].vPos[j] < m_vMin[j])
				m_vMin[j] = m_vecVertices[i].vPos[j];

			if (m_vecVertices[i].vPos[j] > m_vMax[j])
				m_vMax[j] = m_vecVertices[i].vPos[j];
		}
		
	}
	// KDTREE
	m_pKDTree = CKDTree::Create(m_vecVertices, (std::vector<INDEX16>*)m_pIndices, m_vMin, m_vMax);
}
void CMesh::SetMeshInfo(const std::vector<VERTEX>& vecVertices, const void * pIndices, const bool & b16bit, const std::vector<SUBSET>& vecSubset)
{
	m_vecVertices = vecVertices;
	m_b16bit = b16bit;
	m_vecSubset = vecSubset;
	if (b16bit)
	{
		std::vector<INDEX16>* pIndices16 = new std::vector<INDEX16>();
		(*pIndices16) = *(std::vector<INDEX16>*)pIndices;
		m_pIndices = pIndices16;
		m_uiIdxCnt = (UINT)pIndices16->size();
	}
	else
	{
		std::vector<INDEX32>* pIndices32 = new std::vector<INDEX32>();
		(*pIndices32) = *(std::vector<INDEX32>*)pIndices;
		m_pIndices = pIndices32;
		m_uiIdxCnt = (UINT)pIndices32->size();
	}
}

thread_local XMVECTOR g_v0, g_v1, g_v2, g_sNormal;
thread_local std::vector<INDEX16>* g_pIndices16;
thread_local std::vector<INDEX32>* g_pIndices32;
thread_local size_t g_IdxCnt = 0;

void GetTriangle(XMVECTOR& v0, XMVECTOR& v1, XMVECTOR& v2, XMVECTOR& sNormal, const bool& b16bit, const std::vector<VERTEX>& vecVertices, void* pIndices, const UINT& uiIndex)
{
	if (b16bit)
	{
		v0 = vecVertices[(*g_pIndices16)[uiIndex]._1].vPos.ToSIMD();
		v1 = vecVertices[(*g_pIndices16)[uiIndex]._2].vPos.ToSIMD();
		v2 = vecVertices[(*g_pIndices16)[uiIndex]._3].vPos.ToSIMD();

		sNormal = XMPlaneFromPoints(v0, v1, v2);
		/*sNormal = XMVector3Normalize(
			vecVertices[(*g_pIndices16)[uiIndex]._1].vNormal.ToSIMD() +
			vecVertices[(*g_pIndices16)[uiIndex]._2].vNormal.ToSIMD() +
			vecVertices[(*g_pIndices16)[uiIndex]._3].vNormal.ToSIMD());*/
	}
	else
	{
		v0 = vecVertices[(*g_pIndices32)[uiIndex]._1].vPos.ToSIMD();
		v1 = vecVertices[(*g_pIndices32)[uiIndex]._2].vPos.ToSIMD();
		v2 = vecVertices[(*g_pIndices32)[uiIndex]._3].vPos.ToSIMD();

		sNormal = XMVector3Normalize(
			vecVertices[(*g_pIndices16)[uiIndex]._1].vNormal.ToSIMD() +
			vecVertices[(*g_pIndices16)[uiIndex]._2].vNormal.ToSIMD() +
			vecVertices[(*g_pIndices16)[uiIndex]._3].vNormal.ToSIMD());
	}
}

bool CMesh::Hit(const CRay & inRay, float & fMin, float & fMax, HitRecord & hitRecord)const
{
	if (m_b16bit)
		g_pIndices16 = (std::vector<INDEX16>*)m_pIndices;
	else
		g_pIndices32 = (std::vector<INDEX32>*)m_pIndices;


	float fDiscriminant = 0.f;
	XMVECTOR sRayDir = inRay.GetDirection().ToSIMD();
	XMVECTOR sRayOrigin = inRay.GetOrigin().ToSIMD();
	float t = fMin;
	float fResultTime = fMax;

	float fD = 0.f;
	XMVECTOR sPos;
	XMVECTOR sDir, sEdge;
	vec3 vNormal;
	bool bHit = false;
	
	if (nullptr != m_pKDTree)
	{
		std::list<LEAF_SORT> SortedLeafs = m_pKDTree->Ray_Intersection(inRay);

		auto iter = SortedLeafs.begin();
		auto iter_end = SortedLeafs.end();

		for (; iter != iter_end; ++iter)
		{
			UINT* pTriArray = (*iter).pLeafNode->pTriArray;
			UINT uiTriCnt = (*iter).pLeafNode->uiTriCnt;

			for (UINT i = 0; i < uiTriCnt; ++i)
			{
				GetTriangle(g_v0, g_v1, g_v2, g_sNormal, m_b16bit, m_vecVertices, m_pIndices, pTriArray[i]);

				fDiscriminant = XMVector3Dot(g_sNormal, sRayDir).m128_f32[0];

				if (fabs(fDiscriminant) < 0.00001f) continue;

				fD = -XMVector3Dot(g_v0, g_sNormal).m128_f32[0];

				t = -(XMVector3Dot(g_sNormal, sRayOrigin).m128_f32[0] + fD) / fDiscriminant;

				if (fMin >= t || t >= fMax) continue;


				sPos = inRay.PointAtParameter(t).ToSIMD();

				sDir = sPos - g_v0;
				sEdge = g_v1 - g_v0;
				if (XMVector3Dot(g_sNormal, XMVector3Cross(sEdge, sDir)).m128_f32[0] <= 0.f) continue;

				sDir = sPos - g_v1;
				sEdge = g_v2 - g_v1;
				if (XMVector3Dot(g_sNormal, XMVector3Cross(sEdge, sDir)).m128_f32[0] <= 0.f) continue;

				sDir = sPos - g_v2;
				sEdge = g_v0 - g_v2;
				if (XMVector3Dot(g_sNormal, XMVector3Cross(sEdge, sDir)).m128_f32[0] <= 0.f) continue;


				if (fResultTime > t)
					fResultTime = t;
				else
					continue;

				bHit = true;
				vNormal = g_sNormal;
			}
			if (bHit)
			{
				hitRecord.fTime = fResultTime;
				hitRecord.vPos = inRay.PointAtParameter(fResultTime);
				hitRecord.vNormal = vNormal;
			}
		}
		

		return bHit;
	}
	else
	{
		for (UINT i = 0; i < m_uiIdxCnt; ++i)
		{
			GetTriangle(g_v0, g_v1, g_v2, g_sNormal, m_b16bit, m_vecVertices, m_pIndices, i);

			fDiscriminant = XMVector3Dot(g_sNormal, sRayDir).m128_f32[0];

			if (fabs(fDiscriminant) < 0.00001f) continue;

			fD = -XMVector3Dot(g_v0, g_sNormal).m128_f32[0];

			t = -(XMVector3Dot(g_sNormal, sRayOrigin).m128_f32[0] + fD) / fDiscriminant;

			if (fMin >= t || t >= fMax) continue;


			sPos = inRay.PointAtParameter(t).ToSIMD();

			sDir = sPos - g_v0;
			sEdge = g_v1 - g_v0;
			if (XMVector3Dot(g_sNormal, XMVector3Cross(sEdge, sDir)).m128_f32[0] <= 0.f) continue;

			sDir = sPos - g_v1;
			sEdge = g_v2 - g_v1;
			if (XMVector3Dot(g_sNormal, XMVector3Cross(sEdge, sDir)).m128_f32[0] <= 0.f) continue;

			sDir = sPos - g_v2;
			sEdge = g_v0 - g_v2;
			if (XMVector3Dot(g_sNormal, XMVector3Cross(sEdge, sDir)).m128_f32[0] <= 0.f) continue;


			if (fResultTime > t)
				fResultTime = t;
			else
				continue;

			bHit = true;
			vNormal = g_sNormal;
		}

		if (bHit)
		{
			hitRecord.fTime = fResultTime;
			hitRecord.vPos = inRay.PointAtParameter(fResultTime);
			hitRecord.vNormal = vNormal;
		}



		return bHit;
	}
	return false;

}


