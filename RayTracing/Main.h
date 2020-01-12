#pragma once

#include "FieldObject.h"
#include <vector>

class CBVHTree;
class CMain;
struct THREAD_DATA
{
	THREAD_DATA(CMain* _pMain, int _iIdxX, int _iIdxY) : pMain(_pMain), iIdxX(_iIdxX), iIdxY(_iIdxY) {}
	CMain* pMain;
	int iIdxX;
	int iIdxY;
};
class CCamera;
class CMain
{
public:
	CMain(HWND hWnd);
	~CMain();
public:
	void Initialize(void);
	void Render(void);
	void RenderPixel(const int& iIdxX, const int& iIdxY);

	static void RenderByThread(void* pMain);
private:
	int m_iScreenX;
	int m_iScreenY;
	int m_iSampleCnt;

	std::vector<CFieldObject*> m_vecObjects;
	CBVHTree* m_pBVHTree;
private:
	HDC m_hdc;
	HWND m_hWnd;
	CCamera* m_pCamera;

};