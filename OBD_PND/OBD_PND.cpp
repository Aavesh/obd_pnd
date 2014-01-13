// OBD_PND.cpp : ����Ӧ�ó��������Ϊ��
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


// COBD_PNDApp ����
COBD_PNDApp::COBD_PNDApp()
	: CWinApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� COBD_PNDApp ����
COBD_PNDApp theApp;
// COBD_PNDApp ��ʼ��

BOOL COBD_PNDApp::InitInstance()
{

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	// ����һ��������,ȷ������ֻ��һ��ʵ��������   
	HANDLE hMutex=CreateMutex(NULL, FALSE, _T("OBD_PND"));  

	if(!hMutex)  
	{  
		AfxMessageBox(_T("�����������ʧ��!"));
		return FALSE;  
	}  

	if(ERROR_ALREADY_EXISTS == GetLastError())  
	{  
		//AfxMessageBox(_T("Ӧ�ó����Ѿ�����!"));
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
		// TODO: �ڴ˴����ô����ʱ�á�ȷ�������ر�
		//  �Ի���Ĵ���
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}
