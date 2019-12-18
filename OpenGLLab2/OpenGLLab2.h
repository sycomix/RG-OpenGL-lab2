
// OpenGLLab2.h : main header file for the OpenGLLab2 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// COpenGLLab2App:
// See OpenGLLab2.cpp for the implementation of this class
//

class COpenGLLab2App : public CWinApp
{
public:
	COpenGLLab2App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern COpenGLLab2App theApp;
