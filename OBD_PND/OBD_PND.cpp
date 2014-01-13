// OBD_PND.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "OBD_PND.h"
#include "OBD_PNDDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COBD_PNDApp

BEGIN_MESSAGE_MAP(COBD_PNDApp, CWinApp)
END_MESSAGE_MAP()


// COBD_PNDApp 构造
COBD_PNDApp::COBD_PNDApp()
	: CWinApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 COBD_PNDApp 对象
COBD_PNDApp theApp;
// COBD_PNDApp 初始化

BOOL COBD_PNDApp::InitInstance()
{

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	// 定义一个互斥体,确保程序只有一个实例在运行   
	HANDLE hMutex=CreateMutex(NULL, FALSE, _T("OBD_PND"));  

	if(!hMutex)  
	{  
		AfxMessageBox(_T("创建互斥对象失败!"));
		return FALSE;  
	}  

	if(ERROR_ALREADY_EXISTS == GetLastError())  
	{  
		//AfxMessageBox(_T("应用程序已经运行!"));
		CWnd *pWnd = CWnd::FindWindow(NULL,_T( "OBD_PND"));
		if (pWnd != NULL)
		{
			//AfxMessageBox(_T("The software is running!"));
			//::SendMessage(pWnd->m_hWnd,WM_WINDOW_RESTORE,0,0);

		}
		CloseHandle(hMutex);  
		return FALSE;   
	}  

	COBD_PNDDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此处放置处理何时用“确定”来关闭
		//  对话框的代码
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}
