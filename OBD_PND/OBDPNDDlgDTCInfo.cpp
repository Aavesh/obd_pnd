// OBDPNDDlgDTCInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "OBD_PND.h"
#include "OBDPNDDlgDTCInfo.h"
#include "OBD_PNDDlg.h"


// COBDPNDDlgDTCInfo 对话框

IMPLEMENT_DYNAMIC(COBDPNDDlgDTCInfo, CDialog)

COBDPNDDlgDTCInfo::COBDPNDDlgDTCInfo(CWnd* pParent /*=NULL*/)
	: CDialog(COBDPNDDlgDTCInfo::IDD, pParent)
{
	m_iIndex = 1;
}

COBDPNDDlgDTCInfo::~COBDPNDDlgDTCInfo()
{
}

void COBDPNDDlgDTCInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(COBDPNDDlgDTCInfo, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// COBDPNDDlgDTCInfo 消息处理程序


BOOL COBDPNDDlgDTCInfo::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowText(_T("OBD_PND_DTC"));

	m_mapDtcList = theMainDlg->fGetDtcList();
	fInitUI();

	return TRUE;
}

void COBDPNDDlgDTCInfo::fInitUI()
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
			m_rectCode = CRect(50,80,200,130);
			m_rectCodeValue = CRect(200,80,750,130);
			m_rectText = CRect(50,140,200,435);
			m_rectTextValue = CRect(200,140,750,435);
 
			VERIFY(m_fontDtcCode.CreateFont(
				35,                        // nHeight
				0,                         // nWidth
				0,                         // nEscapement
				0,                         // nOrientation
				FW_BOLD, //FW_BOLD,加粗                // nWeight
				FALSE,                     // bItalic
				FALSE,                     // bUnderline
				0,                         // cStrikeOut
				ANSI_CHARSET,              // nCharSet
				OUT_DEFAULT_PRECIS,        // nOutPrecision
				CLIP_DEFAULT_PRECIS,       // nClipPrecision
				DEFAULT_QUALITY,           // nQuality
				DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
				_T("Arial Rounded MT")));              // lpszFacename

			VERIFY(m_fontDtcText.CreateFont(
				35,                        // nHeight
				0,                         // nWidth
				0,                         // nEscapement
				0,                         // nOrientation
				FW_BOLD, //FW_BOLD,加粗                // nWeight
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

	fSetTitleText(theMainDlg->fGetBinText(DS_DTC_INFO_TITLE));

	m_strDtcCode = theMainDlg->fGetBinText(DS_DTC_INFO_TEXT1);
	m_strDtcText = theMainDlg->fGetBinText(DS_DTC_INFO_TEXT2);

	if (m_mapDtcList.size()>0)
	{
		m_itDtcInfo = m_mapDtcList.begin();
		m_iIndex = 0;
	}

	if (m_mapDtcList.size()>1)
	{
		fSetFootText(0,theMainDlg->fGetBinText(DS_BTN_ITEM_UP));	//
		fSetFootText(1,theMainDlg->fGetBinText(DS_BTN_ITEM_DOWN));	//返回
	}

	fSetFootText(MAX_FOOT_INDEX-1,theMainDlg->fGetBinText(DS_BTN_RETURN));	//返回


	this->Invalidate();
}
void COBDPNDDlgDTCInfo::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDialog::OnLButtonDown(nFlags, point);
}

void COBDPNDDlgDTCInfo::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	switch (fGetCurrFootIndex(point))
	{
	case 0:
		{
			if (m_mapDtcList.size()>1)
			{
				if (m_itDtcInfo == m_mapDtcList.begin())
				{
					m_itDtcInfo = m_mapDtcList.end();
					m_iIndex = m_mapDtcList.size();
				}
				m_iIndex--;
				m_itDtcInfo--;

			}
		}
		break;
	case 1:
		{
			if (m_mapDtcList.size()>1)
			{
				m_itDtcInfo++;
				m_iIndex++;
				if (m_itDtcInfo == m_mapDtcList.end())
				{
					m_itDtcInfo = m_mapDtcList.begin();
					m_iIndex = 0;
				}
			}
		}
		break;
	case MAX_FOOT_INDEX-1:
		CDialog::OnCancel();
		break;
	}
	this->Invalidate();

	CDialog::OnLButtonUp(nFlags, point);
}

BOOL COBDPNDDlgDTCInfo::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;

	return CDialog::OnEraseBkgnd(pDC);
}

void COBDPNDDlgDTCInfo::OnPaint()
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

		memDC.SetBkColor(m_clrDtcCode);
		memDC.SelectObject(&m_fontDtcCode);
		memDC.DrawText(m_strDtcCode,-1,m_rectCode,DT_VCENTER|DT_LEFT);

		CString strText;
		if (m_mapDtcList.size()>0)
			strText.Format(_T("P%04X"),m_itDtcInfo->second.dwDtcID);
		else
			strText.Format(_T(""));
		memDC.DrawText(strText,-1,m_rectCodeValue,DT_VCENTER|DT_LEFT);

		//故障码数量大于1，显示页码
		if (m_mapDtcList.size() >1)
		{
			strText.Format(_T("%d/%d"),m_iIndex+1,m_mapDtcList.size());
			memDC.DrawText(strText,-1,m_rectCodeValue,DT_VCENTER|DT_RIGHT);
		}

		memDC.SetBkColor(m_clrDtcText);
		memDC.SelectObject(&m_fontDtcText);
		memDC.DrawText(m_strDtcText,-1,m_rectText,DT_TOP|DT_LEFT);
		if (m_mapDtcList.size()>0)
			strText.Format(_T("%s"),m_itDtcInfo->second.strDtcText);
		else
			strText.Format(_T(""));
		memDC.DrawText(strText,-1,m_rectTextValue,DT_LEFT|DT_EDITCONTROL|DT_WORDBREAK);

		CPen pen(PS_SOLID,2,RGB(255,255,255));
		CPen *oldPen = memDC.SelectObject(&pen);
		memDC.MoveTo(m_rectCode.left, m_rectCode.bottom + 2);
		memDC.LineTo( m_rectCodeValue.right, m_rectCode.bottom + 2);
		pen.DeleteObject();
		oldPen->DeleteObject();		

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

void COBDPNDDlgDTCInfo::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值	

	CDialog::OnTimer(nIDEvent);
}
