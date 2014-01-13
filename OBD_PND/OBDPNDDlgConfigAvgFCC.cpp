// OBDPNDDlgConfigAvgFCC.cpp : 实现文件
//

#include "stdafx.h"
#include "OBD_PND.h"
#include "OBDPNDDlgConfigAvgFCC.h"
#include "OBD_PNDDlg.h"


// COBDPNDDlgConfigAvgFCC 对话框

IMPLEMENT_DYNAMIC(COBDPNDDlgConfigAvgFCC, CDialog)

COBDPNDDlgConfigAvgFCC::COBDPNDDlgConfigAvgFCC(CWnd* pParent /*=NULL*/)
	: CDialog(COBDPNDDlgConfigAvgFCC::IDD, pParent)
{

}

COBDPNDDlgConfigAvgFCC::~COBDPNDDlgConfigAvgFCC()
{
}

void COBDPNDDlgConfigAvgFCC::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(COBDPNDDlgConfigAvgFCC, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// COBDPNDDlgConfigAvgFCC 消息处理程序

BOOL COBDPNDDlgConfigAvgFCC::OnInitDialog()
{
	CDialog::OnInitDialog();

	fInitUI();

	return TRUE;
}

void COBDPNDDlgConfigAvgFCC::fInitUI()
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
			m_rectList[0] = CRect(60,95,800,170);
			m_rectList[1] = CRect(60,190,800,265);
			m_rectList[2] = CRect(50,305,750,430);

			VERIFY(m_fontText.CreateFont(
				48,                        // nHeight
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
				100,                        // nHeight
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
				65,                        // nHeight
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

	fSetTitleText(theMainDlg->fGetBinText(DS_CONFIG_FUEL_AVG_TEST_TITLE));	//

	m_strText[0] = theMainDlg->fGetBinText(DS_CONFIG_FUEL_AVG_TEST_TEXT1);
	m_strText[1] = theMainDlg->fGetBinText(DS_CONFIG_FUEL_AVG_TEST_TEXT2);
	m_strText[2] = theMainDlg->fGetBinText(DS_CONFIG_FUEL_AVG_TIP_UPDATE);

	fSetFootText(MAX_FOOT_INDEX-2,theMainDlg->fGetBinText(DS_BTN_OK));	//返回
	fSetFootText(MAX_FOOT_INDEX-1,theMainDlg->fGetBinText(DS_BTN_RETURN));	//返回

	this->Invalidate();
}


void COBDPNDDlgConfigAvgFCC::fSetAvgFuelConmuption(float fSetAvgFuelCon)
{
	m_fSettingAvgFuelCon = fSetAvgFuelCon;
	this->Invalidate();
}


void COBDPNDDlgConfigAvgFCC::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDialog::OnLButtonDown(nFlags, point);
}

void COBDPNDDlgConfigAvgFCC::OnLButtonUp(UINT nFlags, CPoint point)
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

BOOL COBDPNDDlgConfigAvgFCC::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;

	return CDialog::OnEraseBkgnd(pDC);
}

void COBDPNDDlgConfigAvgFCC::OnPaint()
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
		strText.Format(_T("%s%0.1f"),m_strText[0],m_fSettingAvgFuelCon); 
		memDC.DrawText(strText, -1, &m_rectList[0], DT_LEFT);	
		float fValue = theMainDlg->fGetDataStreamValueF(0xFF010005);
		if (fValue == INVALID_DATASTREAM_VALUE)
			fValue = 0;
		strText.Format(_T("%s%0.1f"),m_strText[1],fValue);
		memDC.DrawText(strText, -1, &m_rectList[1], DT_LEFT);	

		CPen pen(PS_SOLID,2,RGB(255,255,255));
		CPen *oldPen = memDC.SelectObject(&pen);
		memDC.MoveTo(30, m_rectList[1].bottom + (m_rectList[2].top - m_rectList[1].bottom)/2); 
		memDC.LineTo(theMainDlg->m_rectWin.right-30, m_rectList[1].bottom + (m_rectList[2].top - m_rectList[1].bottom)/2);

		memDC.SelectObject(m_fontText3);
		strText.Format(_T("%s"),m_strText[2]);
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

void COBDPNDDlgConfigAvgFCC::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值	


	CDialog::OnTimer(nIDEvent);
}
