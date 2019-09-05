#include "stdafx.h"
#include "Main.h"
#include "Camera.h"
#include "Utility.h"
#include "ThreadPool.h"

#include "MemoryPool.h"

CMain::CMain(HWND hWnd)
{
	m_hWnd = hWnd;
	m_hdc = GetDC(hWnd);

}
CMain::~CMain(void)
{
	ReleaseDC(m_hWnd, m_hdc);

	CMemoryPool::GetInstance()->DestroyInstance();
	//SAFE_DELETE(m_pCamera);
}

void CMain::Initialize(void)
{
	CMemoryPool::GetInstance()->DefaultSetting();

	//int* pNew = CMemoryPool::Allocate<int>::NEW(CMemoryPool::OBJECT);

	m_pCamera = CMemoryPool::Allocate<CCamera>::NEW(CMemoryPool::OBJECT, vec3(0.f, 1.f, 3.f), vec3(0.f, 0.f, -1.f), vec3(0.f, 1.f, 0.f), g_iScreenX / (float)g_iScreenY, 20.f);
	//m_pCamera = new CCamera(vec3(0.f, 1.f, 3.f), vec3(0.f, 0.f, -1.f), vec3(0.f, 1.f, 0.f), g_iScreenX / (float) g_iScreenY, 20.f);

	auto Test = &CMain::RenderByThread;

	CThreadPool::InitThreads();
}

void CMain::Render(void)
{
	int iCoreCnt = std::thread::hardware_concurrency();

	for (int i = 0; i < g_iScreenX / 50; ++i)
	{
		for (int j = 0; j < g_iScreenY / 50; ++j)
		{
			THREAD_DATA* pThreadData = new THREAD_DATA(this, i, j);
			CThreadPool::Push(TASK(&RenderByThread, pThreadData));
		}
	}

	CThreadPool::WakeThreads();

	while (CThreadPool::RunMain()) {}
	//RenderByThread(new THREAD_DATA(this, iCoreCnt - 1));

	CThreadPool::MainWait();

	MessageBox(m_hWnd, _T("Render End"), _T("Alert"),0);
}

void CMain::RenderPixel(const int& iIdxX, const int& iIdxY)
{
	// Thread ÇÑ¼¿´ç °¡·Î ¼¼·Î 100ÇÈ¼¿¾¿

	int iCoreCnt = std::thread::hardware_concurrency();

	int iXCnt = 50; //g_iScreenX / iCoreCnt;
	int iXStart = iXCnt * iIdxX;//iXCnt * iThreadIdx;

	int iYCnt = 50;//g_iScreenY / iCoreCnt;
	int iYStart = iYCnt * iIdxY;//iYCnt * iThreadIdx;

	for (int x = iXStart; x < iXStart + iXCnt; ++x)
	{
		for (int y = iYStart; y < iYStart + iYCnt; ++y)
		{
			float u = x / (float)g_iScreenX;
			float v = y / (float)g_iScreenY;
			CRay ray = m_pCamera->GetRay(u, v);

			float fDirectionY = ray.GetDirection().y;
			vec3 vColor;
			bool bReturn = CUtility::CalculateColor(ray);
			if (false == bReturn)
				CUtility::GetSkyColor(fDirectionY, vColor);
			else
				vColor.x = 1.f;

			
			CThreadPool::g_PublicMutex.lock();
			SetPixel(m_hdc, x, y, RGB(int(vColor.x * 255.f), int(vColor.y * 255.f), int(vColor.z * 255.f)));
			CThreadPool::g_PublicMutex.unlock();
		}
	}
}

void CMain::RenderByThread(void * pMain)
{
	THREAD_DATA* pThreadData = (THREAD_DATA*)pMain;

	pThreadData->pMain->RenderPixel(pThreadData->iIdxX, pThreadData->iIdxY);

	SAFE_DELETE(pThreadData);
}
