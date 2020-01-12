#pragma once

#include <fbxsdk.h>
#include <string>
#include <vector>
#include "Vertex.h"

class CMesh;
class CFBXLoader
{
public:
	static HRESULT LoadFBXFile(const std::string& strFilePath, CMesh* pLoadMesh);
private:
	// Bake
	static void TraversalNode(fbxsdk::FbxNode* pNode, std::vector<VERTEX>& vecVertices, void* pIndices, const bool& b16bit);
	static void LoadMesh(fbxsdk::FbxNode* pNode, std::vector<VERTEX>& vecVertices, void* pIndices, const bool& b16bit);
private:
	static fbxsdk::FbxManager* g_pFbxManager;
	static fbxsdk::FbxScene* g_pFbxScene;
};

