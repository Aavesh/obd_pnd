// OBDPNDDlgMini.cpp : 实现文件
//

#include "stdafx.h"
#include "OBD_PND.h"
#include "OBDPNDDlgMini.h"
#include "OBD_PNDDlg.h"


// COBDPNDDlgMini 对话框

IMPLEMENT_DYNAMIC(COBDPNDDlgMini, CDialog)

COBDPNDDlgMini::COBDPNDDlgMini(CWnd* pParent /*=NULL*/)
	: CDialog(COBDPNDDlgMini::IDD, pParent)
{
	m_keyDownTime1 = 0;
	m_keyDownTime2 = 0;
}

COBDPNDDlgMini::~COBDPNDDlgMini()
{
}

void COBDPNDDlgMini::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(COBDPNDDlgMini, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()


// COBDPNDDlgMini 消息处理程序

BOOL COBDPNDDlgMini::OnInitDialog()
{
	CDialog::OnInitDialog();

	fInitUI();

	return TRUE;
}

void COBDPNDDlgMini::fInitUI()
{
	switch (theMainDlg->m_dwScreenResolution)
	{
	case RESOLUTION_480X272:
		{

		}
		break;
	case RESOLUTION_800X480:
		{
			CPoint point = theMainDlg->m_pConfigReadWrite->fGetMiniDlgPoint();
			m_rectWin = CRect(point.x,point.y,point.x + 95,point.y + 90);

			fCommDrawImage(&m_memdcBkBmp,&m_cBitmapBkBmp,this->GetDC(),m_rectWin,AfxGetResourceHandle(),IDR_PNG_MINISPEEN_800X480);

			m_rectData = CRect(0,0,95,59);
			m_rectText = CRect(0,59,95,90); 

			VERIFY(m_fontData.CreateFont(
				68,                        // nHeight
				0,                         // nWidth
				0,                         // nEscapement
				0,                         // nOrientation
				FW_BOLD, //FW_BOLD,加粗        FW_NORMAL        // nWeight
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
				30,                        // nHeight
				0,                         // nWidth
				0,                         // nEscapement
				0,                         // nOrientation
				FW_BOLD, //FW_BOLD,加粗        FW_NORMAL        // nWeight
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
	this->MoveWindow(theMainDlg->m_rectWin);


	this->Invalidate();
}
void COBDPNDDlgMini::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	
}
void COBDPNDDlgMini::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	static int dwOldTime;  
	int dwTimeElapsed;  
	if(m_wCount == 0)  
	{  
		m_wCount++;  
		dwOldTime = GetTickCount();  
		SetTimer(1,1000,NULL);  
	}  
	else  
	{  
		dwTimeElapsed = GetTickCount() - dwOldTime;  
		if(dwTimeElapsed <1000  &&dwTimeElapsed > 100 && m_wCount == 1)  
		{  
			m_wCount++;  
		}  
		else  
			m_wCount = 0;  
	}  

	if(m_wCount == 2) //双击判定  
	{  
		KillTimer(1);

		theMainDlg->OnMiniHide(0,0);
		
	}  


	CDialog::OnLButtonDown(nFlags, point);

}


void COBDPNDDlgMini::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//if (GetTickCount() - m_keyUpTime<300)
	//{
	//	AfxMessageBox(_T(""));
	//	KillTimer(1); 
	//	theMainDlg->OnMiniHide(0,0);
	//	this->ShowWindow(SW_HIDE);
	//}
		
	CDialog::OnLButtonUp(nFlags, point);
}

void COBDPNDDlgMini::OnMouseMove(UINT nFlags, CPoint point)
{
	if (theMainDlg->fIsBackRunning())
	{
		KillTimer(2);
		ClientToScreen(&point);

		//if (abs(point.x - (m_rectWin.left+m_rectWin.Width()/2))>30 || abs(point.y - (m_rectWin.top+m_rectWin.Height()/2))>30)
		{
			m_rectWin = CRect(point.x-m_rectWin.Width()/2,point.y-m_rectWin.Height()/2,
				point.x+m_rectWin.Width()-m_rectWin.Width()/2,point.y+m_rectWin.Height()-m_rectWin.Height()/2);
			
			theMainDlg->m_pConfigReadWrite->fSetMiniDlgPoint(CPoint(m_rectWin.left,m_rectWin.top));

			this->MoveWindow(m_rectWin);

			this->Invalidate(); 
		}
	}

	CDialog::OnMouseMove(nFlags,point);
}
BOOL COBDPNDDlgMini::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;

	return CDialog::OnEraseBkgnd(pDC);
}

void COBDPNDDlgMini::OnPaint()
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

		//速度
		if (m_strData == _T("------"))
			m_strData = _T("---");
		DWORD dSSPecent =_ttoi(m_strData);
		CString str; 
		str.Format(_T("%0.0f"),dSSPecent);
		if (dSSPecent>=100)
			memDC.SetTextColor(theMainDlg->m_clrTitle);	//RGB(255,0,0)
		else if (dSSPecent>=80)
			memDC.SetTextColor(theMainDlg->m_clrTitle);	//RGB(244,78,6)
		else 
			memDC.SetTextColor(RGB(0,255,0));	
		memDC.SelectObject(&m_fontData);
		memDC.DrawText(m_strData, -1, &m_rectData, DT_CENTER|DT_VCENTER);	

		memDC.SetTextColor(theMainDlg->m_clrTitle);
		memDC.SelectObject(&m_fontText);
		memDC.DrawText(m_strText, -1, &m_rectText, DT_CENTER|DT_VCENTER);	

		//将背景位图复制到窗口客户区 
		pDC.BitBlt(0,0,rect.Width(),rect.Height(),&memDC,0,0,SRCCOPY); 

		if(pOldMemBmp)   
			memDC.SelectObject(pOldMemBmp); 

		DeleteObject(pOldMemBmp);	
		DeleteDC(memDC);

	}
	CDialog::OnPaint();
}

void COBDPNDDlgMini::fShowDlg(BOOL bShow)
{
	if (bShow )
	{
		m_strData.Format(_T("%s"),theMainDlg->fGetDataStreamValue(0x00000D00));
		m_strText = theMainDlg->fGetDataStreamText(0x00000D00); 
		if (m_strText == INVALID_DATASTREAM_VALUE_STR)
		{
			m_strText = _T("车速");
		}
		this->Invalidate();

		this->MoveWindow(m_rectWin);

		this->ShowWindow(SW_SHOW);
		//this->BringWindowToTop();
		::SetWindowPos(GetSafeHwnd(),HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
		SetTimer(2,1000,NULL);
	}
	else
	{
		
		this->ShowWindow(SW_HIDE);
		KillTimer(2);
	}
	this->Invalidate();
}

void COBDPNDDlgMini::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值	
	switch (nIDEvent)
	{
	case 1:
		{
			m_wCount = 0;  
			KillTimer(1);  
		}
		break;
	case 2:
		{
			m_strData.Format(_T("%s"),theMainDlg->fGetDataStreamValue(0x00000D00));
			this->Invalidate();
		}
		break;
	case 3:
		{

		}
		break;
	}
	CDialog::OnTimer(nIDEvent);
}
