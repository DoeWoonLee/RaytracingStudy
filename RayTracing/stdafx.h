// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include "Public.h"
#include "LogMgr.h"

#define SAFE_DELETE(p) if(p != nullptr) {delete p; p = nullptr;}
#define SAFE_RELEASE(p) if(p != nullptr) { if(0 == p->Release()) {p = nullptr;}}
// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.

