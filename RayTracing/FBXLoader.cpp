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
void ReadNormal(vec3& vOutNormal, fbxsdk::FbxMesh* const pMesh, const int& controlPointIndex, const int& vertexCnt)
{
	//http://docs.autodesk.com/FBX/2014/ENU/FBX-SDK-Documentation/index.html?url=cpp_ref/_normals_2main_8cxx-example.html,topicNumber=cpp_ref__normals_2main_8cxx_example_html2cff27fd-bb1a-4142-982e-65fd5d6ad951

	fbxsdk::FbxGeometryElementNormal* pNormal = pMesh->GetElementNormal(0);

	int iIndex = 0;

	switch (pNormal->GetMappingMode())
	{
		case fbxsdk::FbxGeometryElement::eByControlPoint:
			switch (pNormal->GetReferenceMode())
			{
				case fbxsdk::FbxGeometryElement::eDirect:
					iIndex = controlPointIndex;
					break;
				case fbxsdk::FbxGeometryElement::eIndexToDirect:
					iIndex = pNormal->GetIndexArray().GetAt(controlPointIndex);
					break;
				default:
					_ASSERT(false);
					break;
			}
			break;
		case fbxsdk::FbxGeometryElement::eByPolygonVertex:
			switch (pNormal->GetReferenceMode())
			{
				case fbxsdk::FbxGeometryElement::eDirect:
					iIndex = vertexCnt;
					break;
				case fbxsdk::FbxGeometryElement::eIndexToDirect:
					iIndex = pNormal->GetIndexArray().GetAt(vertexCnt);
					break;
				default:
					_ASSERT(false);
					break;
			}
			break;
		case fbxsdk::FbxGeometryElement::eByPolygon:
			_ASSERT(false);
			break;

	}
	vOutNormal.x = static_cast<float>(pNormal->GetDirectArray().GetAt(iIndex).mData[0]);
	vOutNormal.y = static_cast<float>(pNormal->GetDirectArray().GetAt(iIndex).mData[1]);
	vOutNormal.z = static_cast<float>(pNormal->GetDirectArray().GetAt(iIndex).mData[2]);

}
void ReadUV(vec2& vOutUV, fbxsdk::FbxMesh* const pMesh, const int& controlPointIndex, const int& textureUVIndex)
{

	fbxsdk::FbxGeometryElementUV* pUV = pMesh->GetElementUV();
	int iIndex = 0;

	switch (pUV->GetMappingMode())
	{
	case fbxsdk::FbxGeometryElement::eByControlPoint:
		switch (pUV->GetReferenceMode())
		{
		case fbxsdk::FbxGeometryElement::eDirect:
			iIndex = controlPointIndex;
			break;
		case FbxGeometryElement::eIndexToDirect:
			iIndex = pUV->GetIndexArray().GetAt(controlPointIndex);
			break;
		default:
			_ASSERT(false);
			break;
		}
	case FbxGeometryElement::eByPolygonVertex:
		switch (pUV->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		case FbxGeometryElement::eIndexToDirect:
			iIndex = textureUVIndex;
			break;
		default:
			_ASSERT(false);
			break;
		}
	}
	vOutUV.x = static_cast<float>(pUV->GetDirectArray().GetAt(iIndex).mData[0]);
	vOutUV.y = static_cast<float>(pUV->GetDirectArray().GetAt(iIndex).mData[1]);
}
void CFBXLoader::LoadMesh(fbxsdk::FbxNode * pNode, std::vector<VERTEX>& vecVertices, void* pIndices, const bool& b16bit)
{
	fbxsdk::FbxMesh* pMesh = pNode->GetMesh();
	if (nullptr == pMesh) return;

	UINT uiCtrlIdx = 0;
	UINT vertexCnt = 0;
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

		vecVertices.push_back(Vertex);

	}
	

	bool* pOverlabCheck = new bool[vecVertices.size()];
	ZeroMemory(pOverlabCheck, vecVertices.size() * sizeof(bool));

	// Find Index
	UINT uiTrinagle = pMesh->GetPolygonCount();
	UINT uiIndexByPolygonVertex = 0;
	if (b16bit)
	{
		std::vector<INDEX16>* pIndex = (std::vector<INDEX16>*)pIndices;
		INDEX16 Index;
		
		for (UINT i = 0; i < uiTrinagle; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				Index[j] = pMesh->GetPolygonVertex(i, j);

				

				// ReadUV
				if (pOverlabCheck[Index[j]] == false)
				{
					// ReadNormal
					ReadNormal(Vertex.vNormal, pMesh, i, uiIndexByPolygonVertex);
					vecVertices[Index[j]].vNormal += Vertex.vNormal;

					pOverlabCheck[Index[j]] = true;
					ReadUV(vecVertices[Index[j]].vUV, pMesh, 0, pMesh->GetTextureUVIndex(i, j));
				}

				uiIndexByPolygonVertex++;
			}

			pIndex->push_back(Index);
			
		}
	}
	else
	{
		std::vector<INDEX32>* pIndex = (std::vector<INDEX32>*)pIndices;
		INDEX32 Index;
		XMVECTOR sNormal;

		for (UINT i = 0; i < uiTrinagle; ++i)
		{
			Index._1 = pMesh->GetPolygonVertex(i, 0);
			Index._2 = pMesh->GetPolygonVertex(i, 1);
			Index._3 = pMesh->GetPolygonVertex(i, 2);

			sNormal = XMVector3Normalize(XMPlaneFromPoints(
				vecVertices[Index._1].vPos.ToSIMD(),
				vecVertices[Index._2].vPos.ToSIMD(),
				vecVertices[Index._3].vPos.ToSIMD()));

			for (int j = 0; j < 3; ++j)
			{
				//ReadNormal(Vertex.vNormal, pMesh, i, uiIndexByPolygonVertex++);
				vecVertices[Index[j]].vNormal += sNormal;

				// ReadUV
				if (pOverlabCheck[Index[j]] == false)
				{
					pOverlabCheck[Index[j]] = true;
					ReadUV(vecVertices[Index[j]].vUV, pMesh, 0, pMesh->GetTextureUVIndex(i, j));
				}
			}
			


			pIndex->push_back(Index);
		}
	}
	delete[] pOverlabCheck;
	
	// Normalize Normal
	for (UINT i = 0; i < vecVertices.size(); ++i)
	{
		vecVertices[i].vNormal.Normalize();
	}


	
}

