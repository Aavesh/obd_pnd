// OBDPNDDlgConnect.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OBD_PND.h"
#include "OBDPNDDlgConnect.h"

#include "OBD_PNDDlg.h"

// COBDPNDDlgConnect �Ի���

IMPLEMENT_DYNAMIC(COBDPNDDlgConnect, CDialog)

COBDPNDDlgConnect::COBDPNDDlgConnect(CWnd* pParent /*=NULL*/)
	: CDialog(COBDPNDDlgConnect::IDD, pParent)
{

}

COBDPNDDlgConnect::~COBDPNDDlgConnect()
{
}

void COBDPNDDlgConnect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(COBDPNDDlgConnect, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// COBDPNDDlgConnect ��Ϣ�������

BOOL COBDPNDDlgConnect::OnInitDialog()
{
	CDialog::OnInitDialog();

	fInitUI();

	return TRUE;
}

void COBDPNDDlgConnect::fInitUI()
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
			fSetBkImg(this->GetDC(),IDR_PNG_DEVICE_SETUP_800X480);
		}

		break;
	}

	fSetTitleText(theMainDlg->fGetBinText(DS_SETUP_GUIDE_TITLE));
	fSetFootText(MAX_FOOT_INDEX-2,theMainDlg->fGetBinText(DS_BTN_RETRY));	//����
	fSetFootText(MAX_FOOT_INDEX-1,theMainDlg->fGetBinText(DS_BTN_CLOSE));	//����

	m_rectText = CRect(50,370,750,425);
	m_strText = theMainDlg->fGetBinText(DS_SETUP_DEVICES_TIP);
	m_clrText = RGB(255,255,255);

	this->Invalidate();
}
void COBDPNDDlgConnect::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CDialog::OnLButtonDown(nFlags, point);
}

void COBDPNDDlgConnect::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	switch (fGetCurrFootIndex(point))
	{
	case MAX_FOOT_INDEX-1:
		{
			CDialog::OnCancel();
		}
		break;
	case MAX_FOOT_INDEX-2:
		{
			CDialog::OnOK();
		}
		break;
	}

	CDialog::OnLButtonUp(nFlags, point);
}

BOOL COBDPNDDlgConnect::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	return TRUE;

	return CDialog::OnEraseBkgnd(pDC);
}

void COBDPNDDlgConnect::OnPaint()
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

		memDC.SetBkMode(TRANSPARENT); 

		fDrawTitleText(&memDC);

		memDC.SelectObject(&m_fontText); 
		memDC.SetTextColor(m_clrText);   

		CRect temp = m_rectText;
		int height = memDC.DrawText(m_strText,temp,DT_CENTER| DT_TOP| DT_WORDBREAK | DT_CALCRECT | DT_EDITCONTROL); // ����ı��߶�
		m_rectText.DeflateRect(0,(m_rectText.Height() - height) / 2); // �ı�rect
		memDC.DrawText(m_strText,m_rectText, DT_CENTER| DT_TOP | DT_EDITCONTROL | DT_WORDBREAK);

		fDrawFootText(&memDC);
		
		//������λͼ���Ƶ����ڿͻ��� 
		pDC.BitBlt(0,0,rect.Width(),rect.Height(),&memDC,0,0,SRCCOPY); 

		

		if(pOldMemBmp)   
			memDC.SelectObject(pOldMemBmp); 

		DeleteObject(pOldMemBmp);	
		DeleteDC(memDC);

	}
	CDialog::OnPaint();
}

void COBDPNDDlgConnect::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ	

	CDialog::OnTimer(nIDEvent);
}
