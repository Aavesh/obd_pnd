// OBDPNDDlgLanguage.cpp : 实现文件
//

#include "stdafx.h"
#include "OBD_PND.h"
#include "OBDPNDDlgLanguage.h"
#include "OBD_PNDDlg.h"


// COBDPNDDlgLanguage 对话框

IMPLEMENT_DYNAMIC(COBDPNDDlgLanguage, CDialog)

COBDPNDDlgLanguage::COBDPNDDlgLanguage(CWnd* pParent /*=NULL*/)
	: CDialog(COBDPNDDlgLanguage::IDD, pParent)
{

}

COBDPNDDlgLanguage::~COBDPNDDlgLanguage()
{
}

void COBDPNDDlgLanguage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(COBDPNDDlgLanguage, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// COBDPNDDlgLanguage 消息处理程序


BOOL COBDPNDDlgLanguage::OnInitDialog()
{
	CDialog::OnInitDialog();

	fInitUI();

	return TRUE;
}

void COBDPNDDlgLanguage::fInitUI()
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
			fSetBkImg(this->GetDC(),IDR_PNG_GRID_800X480);

			m_rectGrid[0][0] = CRect(30,75,310,130);
			m_rectGrid[0][1] = CRect(320,75,650,130);
			m_rectGrid[0][2] = CRect(665,75,720,130);
			for (int i=1;i<5;i++)
			{
				m_rectGrid[i][0] = CRect(30,130 + 55*(i-1) ,310,180 + 55*(i-1));
				m_rectGrid[i][1] = CRect(320,130 + 55*(i-1),650,180 + 55*(i-1));
				m_rectGrid[i][2] = CRect(665,140 + 55*(i-1),720,180 + 55*(i-1));
			}	

			VERIFY(m_fontInfoTitle.CreateFont(
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

			VERIFY(m_fontInfoText.CreateFont(
				25,                        // nHeight
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

			VERIFY(m_fontInfoCheck.CreateFont(
				35,                        // nHeight
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
	m_clrInfoTitle = RGB(255,255,255);
	m_clrInfoText= RGB(255,255,255);

	//fSetTitleText(theMainDlg->fGetBinText(DS_CONFIG_DEVICES_LANGUAGE));

	if (theMainDlg->m_pConfigReadWrite->fGetLanguage().size() > 4)
	{
		fSetFootText(0,theMainDlg->fGetBinText(DS_BTN_PAGE_UP));	//上一页
		fSetFootText(1,theMainDlg->fGetBinText(DS_BTN_PAGE_DOWN));	//下一页
	}
	fSetFootText(MAX_FOOT_INDEX-2,theMainDlg->fGetBinText(DS_BTN_OK));	//确定
	fSetFootText(MAX_FOOT_INDEX-1,theMainDlg->fGetBinText(DS_BTN_RETURN));	//返回

	m_strGrid[0][0] = theMainDlg->fGetBinText(DS_SETUP_LAN_ID);
	m_strGrid[0][1] = theMainDlg->fGetBinText(DS_SETUP_LAN_NAME);
	m_strGrid[0][2] = theMainDlg->fGetBinText(DS_SETUP_LAN_SEL);

	fUpdataByPage();

	this->Invalidate();
}

void COBDPNDDlgLanguage::fUpdataByPage()
{
	vector<_TagLanguage> vecLan = theMainDlg->m_pConfigReadWrite->fGetLanguage();
	int j=m_iCurrPageIndex*4;
	for (int i=1;i<5;i++,j++)
	{
		if (j>=vecLan.size())
		{
			m_strGrid[i][0] = _T("");
			m_strGrid[i][1] = _T("");
			m_strGrid[i][2] = _T("");
		}
		else
		{
			m_strGrid[i][0] = vecLan[j].strLanID;
			m_strGrid[i][1] = vecLan[j].strLanName;
		}
	}
}

CString COBDPNDDlgLanguage::fGetSelectedLanID()
{
	vector<_TagLanguage> vecLan = theMainDlg->m_pConfigReadWrite->fGetLanguage();

	if (m_iCurrSelectedIndex >=0 && m_iCurrSelectedIndex<vecLan.size())
	{
		return vecLan[m_iCurrSelectedIndex].strLanID;
	}

	return theMainDlg->m_pConfigReadWrite->fGetActiveLanguage();
}

void COBDPNDDlgLanguage::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDialog::OnLButtonDown(nFlags, point);
}

void COBDPNDDlgLanguage::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	switch (fGetCurrFootIndex(point))
	{
	case MAX_FOOT_INDEX-2:
		CDialog::OnOK();
		break;
	case MAX_FOOT_INDEX-1:
		CDialog::OnCancel();
		break;
	}

	CDialog::OnLButtonUp(nFlags, point);
}

BOOL COBDPNDDlgLanguage::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;

	return CDialog::OnEraseBkgnd(pDC);
}

void COBDPNDDlgLanguage::OnPaint()
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

		memDC.SelectObject(m_fontInfoTitle); 
		memDC.SetTextColor(m_clrInfoTitle);

		memDC.DrawText(m_strGrid[0][0], -1, m_rectGrid[0][0], DT_CENTER|DT_VCENTER);
		memDC.DrawText(m_strGrid[0][1], -1, m_rectGrid[0][1], DT_CENTER|DT_VCENTER);
		memDC.DrawText(m_strGrid[0][2], -1, m_rectGrid[0][2], DT_CENTER|DT_VCENTER);

		for (int i=1;i<5;i++)
		{	
			if (m_strGrid[i][0] == _T(""))
				continue;

			memDC.SelectObject(m_fontInfoText); 

			memDC.SelectStockObject(NULL_BRUSH);
			if (m_iCurrSelectedIndex == m_iCurrPageIndex*4+i-1)
			{
				memDC.SetTextColor(theMainDlg->m_clrFootFocus);  
				CPen pen(PS_SOLID,2,theMainDlg->m_clrFootFocus);
				memDC.SelectObject(pen);
				pen.DeleteObject();				
			}
			else
			{
				memDC.SetTextColor(theMainDlg->m_clrFootNormal); 
				CPen pen(PS_SOLID,2,theMainDlg->m_clrFootNormal);
				memDC.SelectObject(pen);
				pen.DeleteObject();
			}
			memDC.Rectangle(m_rectGrid[i][2].left,m_rectGrid[i][2].top ,m_rectGrid[i][2].right,m_rectGrid[i][2].bottom  );

			memDC.DrawText(m_strGrid[i][0], -1, &m_rectGrid[i][0], DT_CENTER|DT_VCENTER);
			memDC.DrawText(m_strGrid[i][1], -1, &m_rectGrid[i][1], DT_CENTER|DT_VCENTER);

			if (m_iCurrSelectedIndex == m_iCurrPageIndex*4+i-1)
			{				
				memDC.SelectObject(m_fontInfoCheck);
				memDC.DrawText(_T("√"), -1, &CRect(m_rectGrid[i][2].left,m_rectGrid[i][2].top-5,m_rectGrid[i][2].right,m_rectGrid[i][2].bottom+ 5),DT_CENTER|DT_VCENTER);
			}

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

void COBDPNDDlgLanguage::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值	


	CDialog::OnTimer(nIDEvent);
}
