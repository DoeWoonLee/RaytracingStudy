#include "stdafx.h"
#include "FBXLoader.h"
#include "Mesh.h"

fbxsdk::FbxManager* CFBXLoader::g_pFbxManager = nullptr;
fbxsdk::FbxScene* CFBXLoader::g_pFbxScene = nullptr;

void FindMeshInfo(fbxsdk::FbxNode * pNode, UINT & uiVerticesCnt, UINT & uiIndicesCnt, std::vector<SUBSET>& vecSubsets);

HRESULT CFBXLoader::LoadFBXFile(const std::string& strFilePath, CMesh* pLoadMesh)
{
	if (nullptr == g_pFbxManager)
	{
		g_pFbxManager = fbxsdk::FbxManager::Create();

		if (nullptr == g_pFbxManager)
		{
			return E_FAIL;
		}
		g_pFbxManager->SetIOSettings(FbxIOSettings::Create(g_pFbxManager, IOSROOT));
		g_pFbxScene = fbxsdk::FbxScene::Create(g_pFbxManager, "MyScene");
	}
	
	fbxsdk::FbxImporter* pFbxImporter = fbxsdk::FbxImporter::Create(g_pFbxManager, "MyImporter");

	if (nullptr == pFbxImporter) return E_FAIL;
	if (!pFbxImporter->Initialize(strFilePath.c_str(), -1, g_pFbxManager->GetIOSettings())) return E_FAIL;
	if (!pFbxImporter->Import(g_pFbxScene)) return E_FAIL;
	pFbxImporter->Destroy();

	// Geomery Load
	
	fbxsdk::FbxNode* pFbxNode = g_pFbxScene->GetRootNode();

	std::vector<VERTEX> vecVertices;


	void* pIndices = nullptr;
	std::vector<INDEX16> vecIndices16;
	std::vector<INDEX32> vecIndices32;
	std::vector<SUBSET> vecSubset;

	UINT uiVerticesCnt = 0;
	UINT uiIndicesCnt = 0;
	FindMeshInfo(pFbxNode, uiVerticesCnt, uiIndicesCnt, vecSubset);

	vecVertices.reserve(uiVerticesCnt);
	bool b16bit;
	//65536
	if (65535 > uiVerticesCnt)
	{
		pIndices = &vecIndices16;
		vecIndices16.reserve(uiIndicesCnt);
		b16bit = true;
	}
	else
	{
		pIndices = &vecIndices32;
		vecIndices32.reserve(uiIndicesCnt);
		b16bit = false;
	}

	TraversalNode(pFbxNode, vecVertices, pIndices, b16bit);

	vecSubset.shrink_to_fit();
	
	pLoadMesh->SetMeshInfo(vecVertices, pIndices, b16bit, vecSubset);

	return S_OK;
}
void FindMeshInfo(fbxsdk::FbxNode * pNode, UINT & uiVerticesCnt, UINT & uiIndicesCnt, std::vector<SUBSET>& vecSubsets)
{
	int iChildCnt = pNode->GetChildCount();

	{
		fbxsdk::FbxMesh* pMesh = pNode->GetMesh();

		if (nullptr != pMesh)
		{
			SUBSET tSubset;

			tSubset.uiFaceStart = uiIndicesCnt;
			tSubset.uiFaceCount = pMesh->GetPolygonCount();

			uiVerticesCnt += pMesh->GetControlPointsCount();
			uiIndicesCnt += tSubset.uiFaceCount;

			vecSubsets.push_back(tSubset);
		}
	}
	for (int i = 0; i < iChildCnt; ++i)
	{
		FindMeshInfo(pNode->GetChild(i), uiVerticesCnt, uiIndicesCnt, vecSubsets);
	}
}
void CFBXLoader::TraversalNode(fbxsdk::FbxNode * pNode, std::vector<VERTEX>& vecVertices, void* pIndices, const bool& b16bit)
{
	int iChildCnt = pNode->GetChildCount();

	LoadMesh(pNode, vecVertices, pIndices, b16bit);
	for (int i = 0; i < iChildCnt; ++i)
	{
		TraversalNode(pNode->GetChild(i), vecVertices, pIndices, b16bit);
	}
}

void CFBXLoader::LoadMesh(fbxsdk::FbxNode * pNode, std::vector<VERTEX>& vecVertices, void* pIndices, const bool& b16bit)
{
	fbxsdk::FbxMesh* pMesh = pNode->GetMesh();
	if (nullptr == pMesh) return;

	UINT uiCtrlIdx = 0;

	VERTEX Vertex;
	fbxsdk::FbxVector4 fbxVtx;
	fbxsdk::FbxVector4 fbxNormal;
	fbxsdk::FbxGeometryElementNormal* pNormal = nullptr;
	
	UINT uiCtrlPointCnt = pMesh->GetControlPointsCount();
	// Find Vertex
	for (UINT i = 0; i < uiCtrlPointCnt; ++i)
	{
		fbxVtx = pMesh->GetControlPointAt(i);
		Vertex.vPos.x = (float)fbxVtx.mData[0];
		Vertex.vPos.y = (float)fbxVtx.mData[1];
		Vertex.vPos.z = (float)fbxVtx.mData[2];

		fbxNormal = pMesh->GetElementNormal()->GetDirectArray().GetAt(i);
		Vertex.vNormal.x = (float)fbxNormal.mData[0];
		Vertex.vNormal.y = (float)fbxNormal.mData[1];
		Vertex.vNormal.z = (float)fbxNormal.mData[2];

		vecVertices.push_back(Vertex);
	}
	


	// Find Index
	UINT uiTrinagle = pMesh->GetPolygonCount();
	if (b16bit)
	{
		std::vector<INDEX16>* pIndex = (std::vector<INDEX16>*)pIndices;
		INDEX16 Index;
		for (UINT i = 0; i < uiTrinagle; ++i)
		{
			Index._1 = pMesh->GetPolygonVertex(i, 0);
			Index._2 = pMesh->GetPolygonVertex(i, 1);
			Index._3 = pMesh->GetPolygonVertex(i, 2);

			pIndex->push_back(Index);
		}
	}
	else
	{
		std::vector<INDEX32>* pIndex = (std::vector<INDEX32>*)pIndices;
		INDEX32 Index;
		for (UINT i = 0; i < uiTrinagle; ++i)
		{
			Index._1 = pMesh->GetPolygonVertex(i, 0);
			Index._2 = pMesh->GetPolygonVertex(i, 1);
			Index._3 = pMesh->GetPolygonVertex(i, 2);

			pIndex->push_back(Index);
		}
	}
	

	
}

