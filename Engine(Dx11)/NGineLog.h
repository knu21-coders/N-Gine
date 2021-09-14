#pragma once
#include "framework.h"
#include "Win32_.h"
#define MAX_LOG_COUNT 5120
using namespace std;

class NGineLog {
public:
	NGineLog(){
		
	}
	~NGineLog() {
		
	}
	void AddLog(const WCHAR* _log) {

	}
	void WarnError(const TCHAR* _error) {
		MessageBox(g_hWnd, _error, _T("오류 경고"), MB_ICONERROR);
	}
private:
	
};
extern NGineLog *g_NGineLogger = nullptr;