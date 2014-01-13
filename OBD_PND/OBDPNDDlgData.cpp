// OBDPNDDlgData.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OBD_PND.h"
#include "OBDPNDDlgData.h"
#include "OBD_PNDDlg.h"


// COBDPNDDlgData �Ի���

IMPLEMENT_DYNAMIC(COBDPNDDlgData, CDialog)

COBDPNDDlgData::COBDPNDDlgData(CWnd* pParent /*=NULL*/)
	: CDialog(COBDPNDDlgData::IDD, pParent)
{
	m_dwBkResID = 0;
}

COBDPNDDlgData::~COBDPNDDlgData()
{
}

void COBDPNDDlgData::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(COBDPNDDlgData, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// COBDPNDDlgData ��Ϣ�������

BOOL COBDPNDDlgData::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_dwBkResID = NULL;

	fInitUI();
	this->ShowWindow(SW_HIDE);

	return TRUE;
}

void COBDPNDDlgData::fSetBkResID(DWORD dwResID)
{
	if (dwResID != NULL)
	{
		m_dwBkResID = dwResID;

		fCommDrawImage(&m_memdcBkBmp,&m_cBitmapBkBmp,this->GetDC(),m_rectWin,AfxGetResourceHandle(),m_dwBkResID);
		this->Invalidate();
		this->ShowWindow(SW_SHOW);
	}
	else
		this->ShowWindow(SW_HIDE);

}
void COBDPNDDlgData::fInitUI(DWORD dwResID )
{

	switch (theMainDlg->m_dwScreenResolution)
	{
	case RESOLUTION_480X272:
		{

		}
		break;
	case RESOLUTION_800X480:
		{
			m_rectWin = CRect(0,0,61,55);
		}

		break;
	}

	this->MoveWindow(m_rectWin);
	this->Invalidate();
}
void COBDPNDDlgData::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CDialog::OnLButtonDown(nFlags, point);
}

void COBDPNDDlgData::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ


	CDialog::OnLButtonUp(nFlags, point);
}

BOOL COBDPNDDlgData::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	return TRUE;

	return CDialog::OnEraseBkgnd(pDC);
}

void COBDPNDDlgData::OnPaint()
{
	if (m_dwBkResID !=NULL && m_memdcBkBmp != NULL)
	{
		CClientDC pDC(this);

		CRect rect; 
		CDC   memDC; 

		GetWindowRect(&rect); 	
		memDC.CreateCompatibleDC(&pDC); 	

		CBitmap* pOldMemBmp = NULL; 
		pOldMemBmp = memDC.SelectObject(&m_cBitmapBkBmp);
		memDC.BitBlt(0,0,rect.Width(),rect.Height(),&m_memdcBkBmp,0,0,SRCCOPY);

		memDC.SetBkMode(TRANSPARENT);	
		memDC.SetTextColor(theMainDlg->m_clrTitle);

		//������λͼ���Ƶ����ڿͻ��� 
		pDC.BitBlt(0,0,rect.Width(),rect.Height(),&memDC,0,0,SRCCOPY); 

		if(pOldMemBmp)   
			memDC.SelectObject(pOldMemBmp); 

		DeleteObject(pOldMemBmp);	
		DeleteDC(memDC);

	}
	CDialog::OnPaint();
}

void COBDPNDDlgData::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ	

	CDialog::OnTimer(nIDEvent);
}
