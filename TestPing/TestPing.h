
// TestPing.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// основные символы


// CTestPingApp:
// Сведения о реализации этого класса: TestPing.cpp
//

class CTestPingApp : public CWinApp
{
public:
	CTestPingApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CTestPingApp theApp;
