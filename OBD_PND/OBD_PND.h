// OBD_PND.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#ifdef STANDARDSHELL_UI_MODEL
#include "resource.h"
#endif

// COBD_PNDApp:
// �йش����ʵ�֣������ OBD_PND.cpp
//

class COBD_PNDApp : public CWinApp
{
public:
	COBD_PNDApp();
	
// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern COBD_PNDApp	theApp;
