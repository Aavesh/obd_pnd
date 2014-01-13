// OBDPNDDlgLogo.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OBD_PND.h"
#include "OBDPNDDlgLogo.h"
#include "OBD_PNDDlg.h"


// COBDPNDDlgLogo �Ի���

IMPLEMENT_DYNAMIC(COBDPNDDlgLogo, CDialog)

COBDPNDDlgLogo::COBDPNDDlgLogo(CWnd* pParent /*=NULL*/)
	: CDialog(COBDPNDDlgLogo::IDD, pParent)
{

}

COBDPNDDlgLogo::~COBDPNDDlgLogo()
{
}

void COBDPNDDlgLogo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(COBDPNDDlgLogo, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// COBDPNDDlgLogo ��Ϣ�������

BOOL COBDPNDDlgLogo::OnInitDialog()
{
	CDialog::OnInitDialog();

	fInitUI();

	return TRUE;
}

void COBDPNDDlgLogo::fInitUI()
{
	this->MoveWindow(theMainDlg->m_rectWin);
	switch (theMainDlg->m_dwScreenResolution)
	{
	case RESOLUTION_480X272:
		{

		}
		break;
	case RESOLUTION_800X480:
		{
			fSetBkImg(this->GetDC(),IDR_PNG_LOGO_800X480);
		}

		break;
	}
	this->Invalidate();
}
void COBDPNDDlgLogo::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CDialog::OnLButtonDown(nFlags, point);
}

void COBDPNDDlgLogo::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialog::OnLButtonUp(nFlags, point);
}

BOOL COBDPNDDlgLogo::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	return TRUE;

	return CDialog::OnEraseBkgnd(pDC);
}

void COBDPNDDlgLogo::OnPaint()
{
	if (m_memdcBkBmp != NULL)
	{
		CClientDC pDC(this);

		CRect rect; 
		CDC   memDC; 

		GetWindowRect(&rect); 	
		memDC.CreateCompatibleDC(&pDC); 	

		CBitmap* pOldMemBmp = NULL; 
		pOldMemBmp = memDC.SelectObject(&m_cBitmapBkBmp);
		memDC.BitBlt(0,0,rect.Width(),rect.Height(),&m_memdcBkBmp,0,0,SRCCOPY);

		//������λͼ���Ƶ����ڿͻ��� 
		pDC.BitBlt(0,0,rect.Width(),rect.Height(),&memDC,0,0,SRCCOPY); 

		if(pOldMemBmp)   
			memDC.SelectObject(pOldMemBmp); 

		DeleteObject(pOldMemBmp);	
		DeleteDC(memDC);

	}
	CDialog::OnPaint();
}

void COBDPNDDlgLogo::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ	

	CDialog::OnTimer(nIDEvent);
}
