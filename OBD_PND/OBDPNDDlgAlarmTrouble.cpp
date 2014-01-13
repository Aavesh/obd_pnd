// OBDPNDDlgAlarmTrouble.cpp : 实现文件
//

#include "stdafx.h"
#include "OBD_PND.h"
#include "OBDPNDDlgAlarmTrouble.h"

#include "OBD_PNDDlg.h"


// COBDPNDDlgAlarmTrouble 对话框

IMPLEMENT_DYNAMIC(COBDPNDDlgAlarmTrouble, CDialog)

COBDPNDDlgAlarmTrouble::COBDPNDDlgAlarmTrouble(CWnd* pParent /*=NULL*/)
: CDialog(COBDPNDDlgAlarmTrouble::IDD, pParent)
{

}

COBDPNDDlgAlarmTrouble::~COBDPNDDlgAlarmTrouble()
{
}

void COBDPNDDlgAlarmTrouble::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(COBDPNDDlgAlarmTrouble, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// COBDPNDDlgAlarmTrouble 消息处理程序

BOOL COBDPNDDlgAlarmTrouble::OnInitDialog()
{
	CDialog::OnInitDialog();

	fInitUI();

	return TRUE;
}

BOOL COBDPNDDlgAlarmTrouble::fIsOpenAlarmTourble()
{
	return m_bIsOpenAlarmTourble;
}

void COBDPNDDlgAlarmTrouble::fInitUI()
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

			m_rectStatus = CRect(50,150,750,180);
			m_rectText = CRect(50,100,750,400);

			VERIFY(m_fontStatus.CreateFont(
				38,                        // nHeight
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

	m_bIsOpenAlarmTourble = theMainDlg->m_pConfigReadWrite->fGetTroubleCodeAlarmStatus();

	fSetTitleText(theMainDlg->fGetBinText(DS_CONFIG_ALARM_TROUBLE));

	fSetFootText(0,theMainDlg->fGetBinText(DS_BTN_OPEN));	//打开
	fSetFootText(1,theMainDlg->fGetBinText(DS_BTN_CLOSE));	//关闭

	fSetFootText(MAX_FOOT_INDEX-2,theMainDlg->fGetBinText(DS_BTN_OK));		//确定
	fSetFootText(MAX_FOOT_INDEX-1,theMainDlg->fGetBinText(DS_BTN_RETURN));	//返回

	if (m_bIsOpenAlarmTourble)
		fSetCurrFootIndex(0);
	else
		fSetCurrFootIndex(1);

	this->Invalidate();
}
void COBDPNDDlgAlarmTrouble::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDialog::OnLButtonDown(nFlags, point);
}

void COBDPNDDlgAlarmTrouble::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	switch (fGetCurrFootIndex(point))
	{
	case 0:
		m_bIsOpenAlarmTourble = TRUE;
		this->Invalidate();
		break;
	case 1:
		m_bIsOpenAlarmTourble = FALSE;
		this->Invalidate();
		break;
	case MAX_FOOT_INDEX-2:
		CDialog::OnOK();
		break;
	case MAX_FOOT_INDEX-1:
		CDialog::OnCancel();
		break;
	}

	CDialog::OnLButtonUp(nFlags, point);
}

BOOL COBDPNDDlgAlarmTrouble::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;

	return CDialog::OnEraseBkgnd(pDC);
}

void COBDPNDDlgAlarmTrouble::OnPaint()
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


		memDC.SelectObject(&m_fontStatus); 
		memDC.SetTextColor(m_clrText);   
		CString	strText =theMainDlg->fGetBinText(DS_CONFIG_ALARM_TROUBLE_TEXT2);
		if (m_bIsOpenAlarmTourble)
			strText += theMainDlg->fGetBinText(DS_BTN_OPEN);
		else
			strText += theMainDlg->fGetBinText(DS_BTN_CLOSE);
		memDC.DrawText(strText,m_rectStatus, DT_LEFT|DT_VCENTER);

		memDC.SelectObject(&m_fontText); 
		memDC.SetTextColor(m_clrText);   
		strText =theMainDlg->fGetBinText(DS_CONFIG_ALARM_TROUBLE_TEXT1);
		CRect temp = m_rectText;
		int height = memDC.DrawText(strText,temp,DT_LEFT| DT_TOP| DT_WORDBREAK | DT_CALCRECT | DT_EDITCONTROL); // 获得文本高度
		m_rectText.DeflateRect(0,(m_rectText.Height() - height) / 2); // 改变rect
		memDC.DrawText(strText,m_rectText, DT_LEFT| DT_TOP | DT_EDITCONTROL | DT_WORDBREAK);

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

void COBDPNDDlgAlarmTrouble::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值	


	CDialog::OnTimer(nIDEvent);
}
