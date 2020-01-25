#include "stdafx.h"
#include "Main.h"
#include "Camera.h"
#include "Utility.h"
#include "MathUtility.h"
#include "ThreadPool.h"

#include "MemoryPool.h"

#include "Sphere.h"
#include "Triangle.h"
#include "Box.h"
#include "ConstantMedium.h"
#include "Mesh.h"

#include "Metal.h"
#include "Lambertain.h"
#include "Dielectric.h"
#include "Isotropic.h"

#include <string>

#include "BVHTree.h"
#include "DiffuseLight.h"


#include "LogMgr.h"
#include "RectangleXY.h"


vec3 g_vCamDir;
int g_iSample = 100;


CMain::CMain(HWND hWnd):
	m_pScreenBuffers(nullptr),
	m_bThreadExit(false)
{
	m_hWnd = hWnd;
	m_hdc = GetDC(hWnd);

}
CMain::~CMain(void)
{
	ReleaseDC(m_hWnd, m_hdc);

	// ======== Object Release =======
#ifdef MEMORYPOOLUSE
	for (UINT i = 0; i < m_vecObjects.size(); ++i)
	{
		SAFE_DELETE(m_vecObjects[i]);
	}
	m_vecObjects.clear();
#endif 

	
	// ===============================
	//SAFE_DELETE(m_pCamera);

	CMemoryPool::GetInstance()->DestroyInstance();
	CLogMgr::GetInstance()->DestroyInstance();


	delete[] m_pScreenBuffers;
	
}
void MakeSongPyeongMap(CCamera** ppCamera, std::vector<CFieldObject*>& vecFieldObjects)
{
	*ppCamera = CMemoryPool::New<CCamera>(CMemoryPool::OBJECT, vec3(0.f, 15.f, 0.f), vec3(0.f, 0.f, -1.f), vec3(0.f, 1.f, 0.f), g_iScreenX / (float)g_iScreenY, 20.f);

	CMesh* pEagle = CMesh::Create(std::string("../Scene/Eagle.fbx"));

	vecFieldObjects.push_back(CFieldObject::Create(
		_T("Eagle"),CTransform::Create(vec3(), vec3(-XMConvertToRadians(90.f), XMConvertToRadians(225.f), 0.f), vec3(28.f, 28.f, 28.f)), pEagle, CDielectric::Create(vec3(0.75f, 0.929f, 1.f), 1.6f)));


	CMesh* pSongPyeon = CMesh::Create(std::string("../Scene/SongPyeon.fbx"));
	

	
	vecFieldObjects.push_back(CFieldObject::Create(
		_T("SongPyeon"),CTransform::Create(vec3(0.0f, 0.0f, 0.f)), pSongPyeon, CMetal::Create(vec3(0.7f, 0.4f, 0.2f))));

	vecFieldObjects.push_back(CFieldObject::Create(CTransform::Create(vec3(), vec3(-XMConvertToRadians(90.f), 0.f, 0.f), vec3(2.f, 2.f, 2.f)), CMesh::Create(std::string("../Scene/Plate.fbx")), CDielectric::Create(1.2f)));

	vec3 vRotate = vec3(0.f, -XMConvertToRadians(45.f), 0.f);
	vecFieldObjects.push_back(CFieldObject::Create(CTransform::Create(vec3(2.5f, 0.5f, 2.5f), vRotate), pSongPyeon, CMetal::Create(vec3(0.23f, 0.6f, 0.3f), 1.f)));

	vRotate = vec3(0.f, -XMConvertToRadians(90.f), 0.f);
	vecFieldObjects.push_back(CFieldObject::Create(CTransform::Create(vec3(0.f, 0.5f, 3.5f), vRotate), pSongPyeon, CMetal::Create(vec3(1.f, 1.f, 1.f), 0.5f)));

	vRotate = vec3(0.f, -XMConvertToRadians(135.f), 0.f);
	vecFieldObjects.push_back(CFieldObject::Create(CTransform::Create(vec3(-2.5f, 0.5f, 2.5f), vRotate), pSongPyeon, CDielectric::Create(vec3(1.f, 0.59f, 1.f), 1.8f)));

	vRotate = vec3(0.f, -XMConvertToRadians(180.f), 0.f);
	vecFieldObjects.push_back(CFieldObject::Create(CTransform::Create(vec3(-3.5f, 0.5f, 0.f), vRotate), pSongPyeon, CDielectric::Create(vec3(0.76f, 0.63f, 0.95f), 1.1f)));

	vRotate = vec3(0.f, -XMConvertToRadians(225.f), 0.f);
	vecFieldObjects.push_back(CFieldObject::Create(CTransform::Create(vec3(-2.5f, 0.5f, -2.5f), vRotate), pSongPyeon, CLambertain::Create(vec3(0.94f, 0.94f, 0.64f))));

	vRotate = vec3(0.f, -XMConvertToRadians(270.f), 0.f);
	vecFieldObjects.push_back(CFieldObject::Create(CTransform::Create(vec3(0.f, 0.5f, -3.5f), vRotate), pSongPyeon, CLambertain::Create(vec3(0.96f, 0.62f, 0.62f))));

	vRotate = vec3(0.f, -XMConvertToRadians(315.f), 0.f);
	vecFieldObjects.push_back(CFieldObject::Create(CTransform::Create(vec3(2.5f, 0.5f, -2.5f), vRotate), pSongPyeon, CDielectric::Create(vec3(0.f, 0.75f, 1.f), 1.2f)));

	// ¹Ù´Ú
	CBox* pBox = CBox::Create(vec3(-0.5f, 0.f, -0.5f), vec3(0.5f, 1.f, 0.5f));
	CLambertain* pLambertain = CLambertain::Create(vec3(0.48f, 0.83f, 0.53f));
	for (int x = -7; x < 15; ++x)
	{
		for (int z = -7; z < 15; ++z)
		{
			float fY = (CMathUtility::frand0to1()) + 1.f;
			CTransform* pTransform = CTransform::Create(vec3((float)x - 4.f, -fY, (float)z - 2.f), vec3(), vec3(1.f, 1.f, 1.f));

			vecFieldObjects.push_back(CFieldObject::Create(pTransform, pBox, pLambertain));
		}
	}

	// ¾È°³
	/*vecFieldObjects.push_back(CFieldObject::Create(CTransform::Create(vec3(0.f, 5.f, 0.f), vec3(), vec3(8.f, 6.f, 8.f)), CConstantMedium::Create(CSphere::Create(vec3(), 1.f), 0.03f), CIsotropic::Create(vec3(1.f, 0.9f, 0.f))));

	vecFieldObjects.push_back(CFieldObject::Create(CTransform::Create(vec3(0.f, 7.f, 0.f), vec3(), vec3(13.f, 7.f, 13.f)), CConstantMedium::Create(CSphere::Create(vec3(), 1.f), 0.01f), CIsotropic::Create(vec3(1.f, 1.f, 1.f))));*/
}
void TestScene(CCamera** ppCamera, std::vector<CFieldObject*>& vecFieldObjects)
{
	*ppCamera = CMemoryPool::New<CCamera>(CMemoryPool::OBJECT, vec3(10.f, 5.f, -18.f), vec3(0.f, 0.f, -1.f), vec3(0.f, 1.f, 0.f), g_iScreenX / (float)g_iScreenY, 20.f);

	vecFieldObjects.push_back(CFieldObject::Create(CTransform::Create(vec3(0.f, 3.f, 0.f), vec3(), vec3(3.f, 3.f, 3.f)), CConstantMedium::Create(CSphere::Create(vec3(), 1.f), 0.01f), CIsotropic::Create(vec3(1.f, 0.f, 0.f)) ));

	vecFieldObjects.push_back(CFieldObject::Create(CTransform::Create(vec3(0.f, -501.f, 0.f), vec3(0.f, 0.f, 0.f), vec3(500.f, 500.f, 500.f))));
}
void ConrnellBox(CCamera** ppCamera, std::vector<CFieldObject*>& vecFieldObjects)
{
	*ppCamera = CMemoryPool::New<CCamera>(CMemoryPool::OBJECT, 
		vec3(0.f, 277.5f, -800.f), 
		vec3(0.f, 277.5f, 0.f),
		vec3(0.f, 1.f, 0.f) , g_iScreenX / (float)g_iScreenY, 40.f);


	CRectangleXY* pRectangle = CRectangleXY::Create(1.f, 1.f);

	CFieldObject* pFieldObject = nullptr;
	vec3 vRectangleScale(555.f, 555.f, 555.f);
	vec3 vWhite(0.73f, 0.73f, 0.73f);
	// Back
	pFieldObject = CFieldObject::Create(
		CTransform::Create(
		vec3(0.f, 277.5f, 555.f), vec3(0.f , XM_PI, 0.f), vRectangleScale),
		pRectangle,	CLambertain::Create(vWhite));

	vecFieldObjects.push_back(pFieldObject);

	// Bottom
	pFieldObject = CFieldObject::Create(
		CTransform::Create(
			vec3(0.f, 0.f, 277.5f), vec3(-XM_PI * 0.5f, XM_PI, 0.f), vRectangleScale),
		pRectangle, CLambertain::Create(vWhite));

	vecFieldObjects.push_back(pFieldObject);

	// Top
	pFieldObject = CFieldObject::Create(
		CTransform::Create(
			vec3(0.f, 555.f, 277.5f), vec3(XM_PI * 0.5f, XM_PI, 0.f), vRectangleScale),
		pRectangle, CLambertain::Create(vWhite));

	vecFieldObjects.push_back(pFieldObject);

	// Right
	pFieldObject = CFieldObject::Create(
		CTransform::Create(
			vec3(277.5f, 277.5, 277.5f), vec3(0.f, -XM_PI * 0.5f, 0.f), vRectangleScale),
		pRectangle, CLambertain::Create(vec3(0.65f, 0.05f, 0.05f)));

	vecFieldObjects.push_back(pFieldObject);
	// Left
	pFieldObject = CFieldObject::Create(
		CTransform::Create(
			vec3(-277.5f, 277.5, 277.5f), vec3(0.f, XM_PI * 0.5f, 0.f), vRectangleScale),
		pRectangle, CLambertain::Create(vec3(0.12f, 0.45f, 0.15f)));

	vecFieldObjects.push_back(pFieldObject);

	// Light
	pFieldObject = CFieldObject::Create(
		CTransform::Create(
			vec3(0.f, 554.f, 277.5f), vec3(-XM_PI * 0.5f, XM_PI, 0.f), vec3(122.f, 122.f, 122.f)),
		pRectangle, CDiffuseLight::Create(vec3(15.f, 15.f, 15.f)));

	vecFieldObjects.push_back(pFieldObject);

	CBox* pBox = CBox::Create(vec3(-0.5f, -0.5f, -0.5f), vec3(0.5f, 0.5f, 0.5f));


	// Rhight Short Box
	pFieldObject = CFieldObject::Create(
		CTransform::Create(
			vec3(65.f, 87.5f, 147.5f),
			vec3(0.f, XMConvertToRadians(15.f), 0.f),
			vec3(165.f, 165.f, 165.f)),
		pBox, CLambertain::Create(vWhite));

	vecFieldObjects.push_back(pFieldObject);

	// Left Tall Box
	
	pFieldObject = CFieldObject::Create(
		CTransform::Create(
			vec3(-70.f, 165.f, 407.5f),
			vec3(0.f, XMConvertToRadians(-18.f), 0.f),
			vec3(165.f, 330.f, 165.f)),
		pBox, CLambertain::Create(vWhite));

	vecFieldObjects.push_back(pFieldObject);

	
#ifdef MEMORYPOOLUSE
	SAFE_RELEASE(pRectangle);
	//SAFE_RELEASE(pBox);
#endif

}
void CMain::Initialize(void)
{
	CMemoryPool::GetInstance()->ReadyMemoryPool();
	CLogMgr::GetInstance()->ReadyLogMgr();
	

	CThreadPool::InitThreads();

	//MakeSongPyeongMap(&m_pCamera, m_vecObjects);
	//TestScene(&m_pCamera, m_vecObjects);
	ConrnellBox(&m_pCamera, m_vecObjects);

	m_pBVHTree = CBVHTree::Create(0, (UINT)m_vecObjects.size(), m_vecObjects);

	m_pScreenBuffers = new BYTE[g_iScreenX * g_iScreenY * 4];

}

void CMain::Render(void)
{


	HBITMAP backBuffer = CreateBitmap(g_iScreenX, g_iScreenY, 1, 32, m_pScreenBuffers);

	HDC hdcMem = CreateCompatibleDC(m_hdc);
	HGDIOBJ oldBitmap = SelectObject(hdcMem, backBuffer);

	BitBlt(m_hdc, 0, 0, g_iScreenX, g_iScreenY, hdcMem, 0, 0, SRCCOPY);

	SelectObject(hdcMem, oldBitmap);

	DeleteDC(hdcMem);
	DeleteObject(backBuffer);
}
bool CMain::RenderCall(void)
{
	if (CThreadPool::CheckNowRunning())
	{
		return false;
	}
	
	for (int i = 0; i < g_iScreenX / 50; ++i)
	{
		for (int j = 0; j < g_iScreenY / 50; ++j)
		{
			THREAD_DATA* pThreadData = new THREAD_DATA(this, i, j);
			CThreadPool::Push(TASK(&RenderByThread, pThreadData));
		}
	}

	CThreadPool::WakeThreads();


	return true;
}

void CMain::ExitProgram(void)
{
	m_bThreadExit = true;

	CThreadPool::MainWait();
}

bool CMain::CheckNowRendering(void)
{
	return CThreadPool::CheckNowRunning();
}

void CMain::RenderPixel(const int& iIdxX, const int& iIdxY)
{
	// Thread ÇÑ¼¿´ç °¡·Î ¼¼·Î 100ÇÈ¼¿¾¿

	int iCoreCnt = std::thread::hardware_concurrency();

	int iXCnt = 50; //g_iScreenX / iCoreCnt;
	int iXStart = iXCnt * iIdxX;//iXCnt * iThreadIdx;

	int iYCnt = 50;//g_iScreenY / iCoreCnt;
	int iYStart = iYCnt * iIdxY;//iYCnt * iThreadIdx;

	vec3 vColor = vec3(0.f, 0.f, 0.f);
	CRay ray;

	float fSample = 1.f/(float)g_iSample;

	float fTexelX = 1.f / (float)g_iScreenX;
	float fTexelY = 1.f / (float)g_iScreenY;
	int x, y, s;
	float u = 0.f;
	float v = 0.f;

	int iScreenBufferIdx = 0;

	for (x = iXStart; x < iXStart + iXCnt; ++x)
	{
		for (y = iYStart; y < iYStart + iYCnt; ++y)
		{
			memset(&vColor, 0, sizeof(vec3));

			for (s = 0; s < g_iSample; ++s)
			{
				if (m_bThreadExit)
					return;

				u = ((float)x + CMathUtility::frand0to1()) * fTexelX;
				v = ((float)y + CMathUtility::frand0to1()) * fTexelY;
				ray = m_pCamera->GetRay(u, v);
				g_vCamDir = ray.GetDirection();

				//CUtility::GetPixelColor(ray, m_vecObjects, vColor);
				CUtility::GetPixelColor(ray, m_pBVHTree, vColor);
			}

			vColor *= fSample;
			
			iScreenBufferIdx = (x + y * g_iScreenX) * 4;

			m_pScreenBuffers[iScreenBufferIdx] = int(fmin(sqrtf(vColor.z), 1.f) * 255.f);
			m_pScreenBuffers[iScreenBufferIdx + 1] = int(fmin(sqrtf(vColor.y), 1.f) * 255.f);
			m_pScreenBuffers[iScreenBufferIdx + 2] = int(fmin(sqrtf(vColor.x), 1.f) * 255.f);

		}
	}
}


void CMain::RenderByThread(void * pMain)
{
	THREAD_DATA* pThreadData = (THREAD_DATA*)pMain;

	pThreadData->pMain->RenderPixel(pThreadData->iIdxX, pThreadData->iIdxY);

	SAFE_DELETE(pThreadData);
}
