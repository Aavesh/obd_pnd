// OBDPNDDlgConfigNowFCC.cpp : 实现文件
//

#include "stdafx.h"
#include "OBD_PND.h"
#include "OBDPNDDlgConfigNowFCC.h"
#include "OBD_PNDDlg.h"


// COBDPNDDlgConfigNowFCC 对话框

IMPLEMENT_DYNAMIC(COBDPNDDlgConfigNowFCC, CDialog)

COBDPNDDlgConfigNowFCC::COBDPNDDlgConfigNowFCC(CWnd* pParent /*=NULL*/)
	: CDialog(COBDPNDDlgConfigNowFCC::IDD, pParent)
{
	m_bCountDown = FALSE;
	m_dbNewFuelConCoeff = 0;
	m_dbOldFuelConCoeff = 0;
	m_dbSetupFuelCon = 0;

	m_wCountTime = 0;

}

COBDPNDDlgConfigNowFCC::~COBDPNDDlgConfigNowFCC()
{
}

void COBDPNDDlgConfigNowFCC::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(COBDPNDDlgConfigNowFCC, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// COBDPNDDlgConfigNowFCC 消息处理程序

BOOL COBDPNDDlgConfigNowFCC::OnInitDialog()
{
	CDialog::OnInitDialog();

	fInitUI();

	SetTimer(2,1000,NULL);

	return TRUE;
}

void COBDPNDDlgConfigNowFCC::fInitUI()
{
	switch (theMainDlg->m_dwScreenResolution)
	{
	case RESOLUTION_480X272:
		{
		}
		break;
	case RESOLUTION_800X480:
		{
			fSetBkImg(this->GetDC(),IDR_PNG_BASE_800X480);

			m_rectList[0] = CRect(100,95,800,170);
			m_rectList[1] = CRect(100,190,800,265);
			m_rectList[2] = CRect(50,305,750,430);

			VERIFY(m_fontText.CreateFont(
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
			VERIFY(m_fontText2.CreateFont(
				60,                        // nHeight
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
			VERIFY(m_fontText3.CreateFont(
				45,                        // nHeight
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

	fSetTitleText(theMainDlg->fGetBinText(DS_CONFIG_FUEL_NOW_TEST_TITLE));	//

	m_strText[0] = theMainDlg->fGetBinText(DS_CONFIG_FUEL_NOW_TEST_TEXT1);
	m_strText[1] = theMainDlg->fGetBinText(DS_CONFIG_FUEL_NOW_TEST_TEXT2);
	m_strText[2] = theMainDlg->fGetBinText(DS_CONFIG_FUEL_NOW_TIP_KEEP_RUN);
	m_strText[3] = theMainDlg->fGetBinText(DS_CONFIG_FUEL_NOW_TIP_KEPP_TIME);

	fSetFootText(MAX_FOOT_INDEX-2,theMainDlg->fGetBinText(DS_BTN_OK));	//返回
	fSetFootText(MAX_FOOT_INDEX-1,theMainDlg->fGetBinText(DS_BTN_RETURN));	//返回

	this->Invalidate();
}


void COBDPNDDlgConfigNowFCC::fSetData(WORD wSpeed,double dbFuelCon)
{
	m_wSetupSpeed = wSpeed;
	m_dbSetupFuelCon = dbFuelCon;
}

double COBDPNDDlgConfigNowFCC::fGetNewFuelConCoeff()
{
	return m_dbNewFuelConCoeff;
}

void COBDPNDDlgConfigNowFCC::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDialog::OnLButtonDown(nFlags, point);
}

void COBDPNDDlgConfigNowFCC::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	switch (fGetCurrFootIndex(point))
	{
	case 0:
		break;
	case 1:
		break;
	case 2:
		break;
	case 3:
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

BOOL COBDPNDDlgConfigNowFCC::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;

	return CDialog::OnEraseBkgnd(pDC);
}

void COBDPNDDlgConfigNowFCC::OnPaint()
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

		memDC.SelectObject(m_fontText);
		memDC.SetTextColor(theMainDlg->m_clrTitle);
		CString strText;			
		DWORD dwCurrVss = theMainDlg->fGetDataStreamValueF(0x00000D00);
		if (dwCurrVss == INVALID_DATASTREAM_VALUE)
			dwCurrVss = 0;
		strText.Format(_T("%s%d"),m_strText[0],dwCurrVss); 
		memDC.DrawText(strText, -1, &m_rectList[0], DT_LEFT);	
		float fValue = theMainDlg->fGetDataStreamValueF(0xFF010001);
		if (fValue == INVALID_DATASTREAM_VALUE)
			fValue = 0;
		strText.Format(_T("%s%0.1f"),fValue);
		memDC.DrawText(strText, -1, &m_rectList[1], DT_LEFT);	

		CPen pen(PS_SOLID,2,RGB(255,255,255));
		CPen *oldPen = memDC.SelectObject(&pen);
		memDC.MoveTo(30, m_rectList[1].bottom + (m_rectList[2].top - m_rectList[1].bottom)/2); 
		memDC.LineTo(theMainDlg->m_rectWin.right-30, m_rectList[1].bottom + (m_rectList[2].top - m_rectList[1].bottom)/2);

		memDC.SelectObject(m_fontText3);
		if (dwCurrVss > 0 && dwCurrVss > m_wSetupSpeed-2 && dwCurrVss < m_wSetupSpeed+2)
		{
			strText.Format(_T("%s[%d]"),m_strText[3],8-m_wCountTime);

			if (!m_bCountDown)
			{
				SetTimer(1,1000,NULL);
				m_bCountDown = TRUE; 
				m_wCountTime = 0;
			}
		}
		else
		{
			strText.Format(_T("%s"),m_strText[2]);

			KillTimer(1);
			m_bCountDown = FALSE;
			m_wCountTime = 0;
		}

		memDC.DrawText(strText, -1, &m_rectList[2],DT_CENTER );	//DT_LEFT|DT_EDITCONTROL|DT_WORDBREAK

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

void COBDPNDDlgConfigNowFCC::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值	
	switch (nIDEvent)
	{
	case 1:
		m_wCountTime++;

		if (m_wCountTime == 8)
		{
			KillTimer(1);
			KillTimer(2);
			//新油耗系数 = 旧油耗系数 * 设置油耗/真是油耗
			float fValue = theMainDlg->fGetDataStreamValueF(0xFF010001);
			if (fValue != INVALID_DATASTREAM_VALUE)
				m_dbNewFuelConCoeff = theMainDlg->m_pConfigReadWrite->fGetNowFCC() * m_dbSetupFuelCon / fValue;
			else
				m_dbNewFuelConCoeff = 0;

			CDialog::OnOK();
		}
		break;
	case 2:
		this->Invalidate();
		break;
	}

	CDialog::OnTimer(nIDEvent);
}
