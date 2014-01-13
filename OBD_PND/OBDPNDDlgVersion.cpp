// OBDPNDDlgVersion.cpp : 实现文件
//

#include "stdafx.h"
#include "OBD_PND.h"
#include "OBDPNDDlgVersion.h"
#include "OBD_PNDDlg.h"


// COBDPNDDlgVersion 对话框

IMPLEMENT_DYNAMIC(COBDPNDDlgVersion, CDialog)

COBDPNDDlgVersion::COBDPNDDlgVersion(CWnd* pParent /*=NULL*/)
	: CDialog(COBDPNDDlgVersion::IDD, pParent)
{
	m_bShowHideInfo = FALSE;
	m_iCheckIndex = 0;
}

COBDPNDDlgVersion::~COBDPNDDlgVersion()
{
}

void COBDPNDDlgVersion::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(COBDPNDDlgVersion, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// COBDPNDDlgVersion 消息处理程序

BOOL COBDPNDDlgVersion::OnInitDialog()
{
	CDialog::OnInitDialog();

	fInitUI();

	return TRUE;
}

void COBDPNDDlgVersion::fInitUI()
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
			fSetBkImg(this->GetDC(),IDR_PNG_BASE_800X480);

			m_rectCheck[0] = CRect(0,0,50,50);
			m_rectCheck[1] = CRect(750,0,800,50);
			m_rectCheck[2] = CRect(0,435-50,50,435);
			m_rectCheck[3] = CRect(750,435-50,800,435);

			m_rectText[1][0] = CRect(100,145,700,185);
			m_rectText[2][0] = CRect(100,190,700,230);
			m_rectText[3][0] = CRect(100,235,700,275);
			m_rectText[4][0] = CRect(100,280,700,320);
			m_rectText[5][0] = CRect(100,325,700,365);
			m_rectText[0][0] = CRect(100,100,700,140);

			VERIFY(m_fontText.CreateFont(
				28,                        // nHeight
				0,                         // nWidth
				0,                         // nEscapement
				0,                         // nOrientation
				FW_NORMAL, //FW_BOLD,加粗        FW_NORMAL        // nWeight
				FALSE,                     // bItalic
				FALSE,                     // bUnderline
				0,                         // cStrikeOut
				ANSI_CHARSET,              // nCharSet
				OUT_DEFAULT_PRECIS,        // nOutPrecision
				CLIP_DEFAULT_PRECIS,       // nClipPrecision
				DEFAULT_QUALITY,           // nQuality
				DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
				_T("Arial Rounded MT")));              // lpszFacename
		}

		break;
	}

	m_clrText = RGB(255,255,255);

	fSetTitleText(theMainDlg->fGetBinText(DS_CONFIG_DEVICES_VERSION));
	fSetFootText(MAX_FOOT_INDEX-1,theMainDlg->fGetBinText(DS_BTN_RETURN));	//返回

	m_strText[1][0] = theMainDlg->fGetBinText(DS_SOFTWARE_VERSION);

	this->Invalidate();
}
void COBDPNDDlgVersion::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDialog::OnLButtonDown(nFlags, point);
}

void COBDPNDDlgVersion::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if(point.x>m_rectCheck[m_iCheckIndex].left && point.x<m_rectCheck[m_iCheckIndex].right && 
		point.y>m_rectCheck[m_iCheckIndex].top && point.y<m_rectCheck[m_iCheckIndex].bottom)
	{
		m_iCheckIndex++;
		if (m_iCheckIndex == 4)
		{
			m_bShowHideInfo = TRUE;

			m_strText[2][0] = theMainDlg->fGetBinText(DS_COMPANY_WEBSIT);
			m_strText[3][0] = theMainDlg->fGetBinText(DS_COMPANY_TEL);
			m_strText[4][0] = theMainDlg->fGetBinText(DS_COMPANY_NAME);

			this->Invalidate();
		}
	}
	else
		m_iCheckIndex = 0;

	switch (fGetCurrFootIndex(point))
	{
	case MAX_FOOT_INDEX-1:
		CDialog::OnCancel();
		break;
	}

	CDialog::OnLButtonUp(nFlags, point);
}

BOOL COBDPNDDlgVersion::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;

	return CDialog::OnEraseBkgnd(pDC);
}

void COBDPNDDlgVersion::OnPaint()
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
		memDC.SetTextColor(theMainDlg->m_clrTitle);

		fDrawTitleText(&memDC);

		memDC.SelectObject(&m_fontText); 
		memDC.SetTextColor(m_clrText);   

		for (int i=0;i<6;i++)
		{
			memDC.DrawText(m_strText[i][0] +  m_strText[i][1], -1, m_rectText[i][0], DT_LEFT);	
		}

		fDrawFootText(&memDC);

		//将背景位图复制到窗口客户区 
		pDC.BitBlt(0,0,rect.Width(),rect.Height(),&memDC,0,0,SRCCOPY); 

		if(pOldMemBmp)   
			memDC.SelectObject(pOldMemBmp); 

		DeleteObject(pOldMemBmp);	
		DeleteDC(memDC);

	}
	CDialog::OnPaint();
}

void COBDPNDDlgVersion::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值	

	CDialog::OnTimer(nIDEvent);
}
