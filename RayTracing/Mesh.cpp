#include "stdafx.h"
#include "Mesh.h"
#include "FBXLoader.h"
#include "KDTree.h"
#include <DirectXCollision.h>

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
thread_local XMVECTOR g_n0, g_n1, g_n2;
thread_local vec2 g_uv0, g_uv1, g_uv2;
thread_local std::vector<INDEX16>* g_pIndices16;
thread_local std::vector<INDEX32>* g_pIndices32;
thread_local size_t g_IdxCnt = 0;

void GetTriangle(XMVECTOR& v0, XMVECTOR& v1, XMVECTOR& v2, 
				XMVECTOR& n0, XMVECTOR& n1, XMVECTOR& n2,
				vec2& uv0, vec2& uv1, vec2& uv2,
				XMVECTOR& sNormal, const bool& b16bit, const std::vector<VERTEX>& vecVertices, void* pIndices, const UINT& uiIndex)
{
	if (b16bit)
	{
		v0 = vecVertices[(*g_pIndices16)[uiIndex]._1].vPos.ToSIMD();
		v1 = vecVertices[(*g_pIndices16)[uiIndex]._2].vPos.ToSIMD();
		v2 = vecVertices[(*g_pIndices16)[uiIndex]._3].vPos.ToSIMD();

		n0 = vecVertices[(*g_pIndices16)[uiIndex]._1].vNormal.ToSIMD();
		n1 = vecVertices[(*g_pIndices16)[uiIndex]._2].vNormal.ToSIMD();
		n2 = vecVertices[(*g_pIndices16)[uiIndex]._3].vNormal.ToSIMD();

		uv0 = vecVertices[(*g_pIndices16)[uiIndex]._1].vUV;
		uv1 = vecVertices[(*g_pIndices16)[uiIndex]._2].vUV;
		uv2 = vecVertices[(*g_pIndices16)[uiIndex]._3].vUV;


		sNormal = XMVector3Normalize(XMPlaneFromPoints(v0, v1, v2));
		
	}
	else
	{
		v0 = vecVertices[(*g_pIndices32)[uiIndex]._1].vPos.ToSIMD();
		v1 = vecVertices[(*g_pIndices32)[uiIndex]._2].vPos.ToSIMD();
		v2 = vecVertices[(*g_pIndices32)[uiIndex]._3].vPos.ToSIMD();

		n0 = vecVertices[(*g_pIndices32)[uiIndex]._1].vNormal.ToSIMD();
		n1 = vecVertices[(*g_pIndices32)[uiIndex]._2].vNormal.ToSIMD();
		n2 = vecVertices[(*g_pIndices32)[uiIndex]._3].vNormal.ToSIMD();

		uv0 = vecVertices[(*g_pIndices32)[uiIndex]._1].vUV;
		uv1 = vecVertices[(*g_pIndices32)[uiIndex]._2].vUV;
		uv2 = vecVertices[(*g_pIndices32)[uiIndex]._3].vUV;

		sNormal = XMVector3Normalize(XMPlaneFromPoints(v0, v1, v2));
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
	//float fDist = fMax;

	
	float fD = 0.f;
	vec2 vUV;
	XMVECTOR sPos;
	//XMVECTOR sDir, sEdge;


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

				GetTriangle(g_v0, g_v1, g_v2, g_n0, g_n1, g_n2, g_uv0, g_uv1, g_uv2,
					g_sNormal, m_b16bit, m_vecVertices, m_pIndices, pTriArray[i]);
				
				
				// 거리 판별 (내꺼가 더 빨라서 안씀 ㅎ;)
				/*if (TriangleTests::Intersects(sRayOrigin, sRayDir, g_v0, g_v1, g_v2, fDist))
				{
					if (  fMin < fDist && fDist < fResultTime)
					{
						vNormal = g_sNormal;
						bHit = true;
						fResultTime = fDist;
					}
				}*/

				fDiscriminant = XMVector3Dot(g_sNormal, sRayDir).m128_f32[0];

				if (fabs(fDiscriminant) < 0.00001f) continue;

				fD = -XMVector3Dot(g_v0, g_sNormal).m128_f32[0];

				t = -(XMVector3Dot(g_sNormal, sRayOrigin).m128_f32[0] + fD) / fDiscriminant;

				if (fMin >= t || t >= fMax) continue;

				if (fResultTime <= t)
					continue;

				sPos = inRay.PointAtParameter(t).ToSIMD();
				//http://realtimecollisiondetection.net/
				// Compute barycentric coordinates (u, v, w) for
				// point p with respect to triangle (a, b, c)
				XMVECTOR v0 (g_v1 - g_v0);
				XMVECTOR v1(g_v2 - g_v0);
				XMVECTOR v2(sPos - g_v0);

				float d00 = GetSX(XMVector3Dot(v0, v0));
				float d01 = GetSX(XMVector3Dot(v0, v1));
				float d11 = GetSX(XMVector3Dot(v1, v1));
				float d20 = GetSX(XMVector3Dot(v2, v0));
				float d21 = GetSX(XMVector3Dot(v2, v1));
				float denom = d00 * d11 - d01 * d01;

				float v = (d11 * d20 - d01 * d21) / denom;
				float w = (d00 * d21 - d01 * d20) / denom;
				float u = 1.0f - v - w;

				// 삼각형 밖에 점이 있다면 저 원소중 하나는 0보다 작게된다.
				if (v < 0.f || w < 0.f || u < 0.f) continue;

				g_sNormal = g_n0 * u + g_n1 * v + g_n2 * w;
				vUV = g_uv0 * u + g_uv1 * v + g_uv2 * w;
				/*

				sDir = sPos - g_v0;
				sEdge = g_v1 - g_v0;
				if (XMVector3Dot(g_sNormal, XMVector3Cross(sEdge, sDir)).m128_f32[0] <= 0.f) continue;

				sDir = sPos - g_v1;
				sEdge = g_v2 - g_v1;
				if (XMVector3Dot(g_sNormal, XMVector3Cross(sEdge, sDir)).m128_f32[0] <= 0.f) continue;

				sDir = sPos - g_v2;
				sEdge = g_v0 - g_v2;
				if (XMVector3Dot(g_sNormal, XMVector3Cross(sEdge, sDir)).m128_f32[0] <= 0.f) continue;
*/

				fResultTime = t;
				bHit = true;
				vNormal = g_sNormal;
			}
			if (bHit)
			{
				hitRecord.fTime = fResultTime;
				hitRecord.vPos = inRay.PointAtParameter(fResultTime);
				hitRecord.vNormal = vNormal;
				hitRecord.vUV = vUV;

				return true;
			}
		}
		

		return bHit;
	}
	else
	{
		/*for (UINT i = 0; i < m_uiIdxCnt; ++i)
		{

			GetTriangle(g_v0, g_v1, g_v2, g_n0, g_n1, g_n2,
				g_sNormal, m_b16bit, m_vecVertices, m_pIndices, i);

			fDiscriminant = XMVector3Dot(g_sNormal, sRayDir).m128_f32[0];

			if (fabs(fDiscriminant) < 0.00001f) continue;

			fD = -XMVector3Dot(g_v0, g_sNormal).m128_f32[0];

			t = -(XMVector3Dot(g_sNormal, sRayOrigin).m128_f32[0] + fD) / fDiscriminant;

			if (fMin >= t || t >= fMax) continue;

			if (fResultTime <= t)
				continue;

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


	
			fResultTime = t;
			bHit = true;
			vNormal = g_sNormal;
		}

		if (bHit)
		{
			hitRecord.fTime = fResultTime;
			hitRecord.vPos = inRay.PointAtParameter(fResultTime);
			hitRecord.vNormal = vNormal;
		}



		return bHit;*/
	}
	return false;

}


