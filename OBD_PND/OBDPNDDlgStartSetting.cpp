// OBDPNDDlgStartSetting.cpp : 实现文件
//

#include "stdafx.h"
#include "OBD_PND.h"
#include "OBDPNDDlgStartSetting.h"
#include "OBD_PNDDlg.h"
#include "OBDPNDDlgSelFile.h"

// COBDPNDDlgStartSetting 对话框

IMPLEMENT_DYNAMIC(COBDPNDDlgStartSetting, CDialog)

COBDPNDDlgStartSetting::COBDPNDDlgStartSetting(CWnd* pParent /*=NULL*/)
	: CDialog(COBDPNDDlgStartSetting::IDD, pParent)
{

}

COBDPNDDlgStartSetting::~COBDPNDDlgStartSetting()
{
}

void COBDPNDDlgStartSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(COBDPNDDlgStartSetting, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// COBDPNDDlgStartSetting 消息处理程序

BOOL COBDPNDDlgStartSetting::OnInitDialog()
{
	CDialog::OnInitDialog();

	fInitUI();

	return TRUE;
}

vector<TagAppInfo> COBDPNDDlgStartSetting::fGetStartList()
{
	return m_vecAppList;
}

void COBDPNDDlgStartSetting::fInitUI()
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

			fSetBkImg(this->GetDC(),IDR_PNG_GRID_800X480);
			m_rectGrid[0][0] = CRect(30,75,310,130);
			m_rectGrid[0][1] = CRect(320,75,650,130);
			m_rectGrid[0][2] = CRect(660,75,760,130);
			for (int i=1;i<5;i++)
			{
				m_rectGrid[i][0] = CRect(30,130 + 55*(i-1) ,310,180 + 55*(i-1));
				m_rectGrid[i][1] = CRect(320,130 + 55*(i-1),650,180 + 55*(i-1));
				m_rectGrid[i][2] = CRect(670,140 + 55*(i-1),750,180 + 55*(i-1));
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

	m_vecAppList = theMainDlg->m_pConfigReadWrite->fGetStartList();

	m_iCurrSelectedIndex= 0;
	m_iCurrRowIndex= 0;					//当前行索引
	m_iCurrPageIndex= 0;
	if (m_vecAppList.size() > 4)
	{
		fSetFootText(0,theMainDlg->fGetBinText(DS_BTN_PAGE_UP));	//上一页
		fSetFootText(1,theMainDlg->fGetBinText(DS_BTN_PAGE_DOWN));	//下一页
		m_iPageCount = m_vecAppList.size()/4+1;
	}
	else
	{
		fSetFootText(0,_T(""));	//上一页
		fSetFootText(1,_T(""));	//下一页
		m_iPageCount = 1;
	}

	fSetTitleText(theMainDlg->fGetBinText(DS_SETUP_AUTO_START_TITLE));

	m_strGrid[0][0] = theMainDlg->fGetBinText(DS_SETUP_AUTO_START_NAME);
	m_strGrid[0][1] = theMainDlg->fGetBinText(DS_SETUP_AUTO_START_PATH);
	m_strGrid[0][2] = theMainDlg->fGetBinText(DS_SETUP_AUTO_START_SEL);
	for (int i=1;i<5;i++)
	{
		m_strGrid[i][0] = _T("");
		m_strGrid[i][1] = _T("");
	}
	for (int i=0;i<m_vecAppList.size();i++)
	{
		m_strGrid[i+1][0] = m_vecAppList[i].strAppName;
		m_strGrid[i+1][1] = m_vecAppList[i].strAppPath;
	}

	fSetFootText(0,theMainDlg->fGetBinText(DS_BTN_MODIFY));	//返回

	fSetFootText(MAX_FOOT_INDEX-2,theMainDlg->fGetBinText(DS_BTN_OK));	//返回
	fSetFootText(MAX_FOOT_INDEX-1,theMainDlg->fGetBinText(DS_BTN_RETURN));	//返回

	this->Invalidate();
}
void COBDPNDDlgStartSetting::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDialog::OnLButtonDown(nFlags, point);
}

void COBDPNDDlgStartSetting::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	for (int i=1;i<5;i++)
	{
		if (point.y> m_rectGrid[i][0].top && point.y <  m_rectGrid[i][0].bottom)
		{
			if (point.x>m_rectGrid[i][1].left && point.x< m_rectGrid[i][1].right)
			{
				COBDPNDDlgSelFile dlgSelFile;
				if(dlgSelFile.DoModal() ==IDOK)
				{					
					m_vecAppList[i-1].strAppPath = dlgSelFile.m_setSelFilePathName;
					m_strGrid[i][1] = dlgSelFile.m_setSelFilePathName;
				}
			}
			if (point.x>m_rectGrid[i][2].left && point.x< m_rectGrid[i][2].right)
			{
				m_vecAppList[i-1].bAutoRun = !m_vecAppList[i-1].bAutoRun;

			}
			m_iCurrSelectedIndex = i-1;
			this->Invalidate();
			break; 
		}
	}

	switch (fGetCurrFootIndex(point))
	{
	case 0:
		{
			COBDPNDDlgSelFile dlgSelFile;
			if(dlgSelFile.DoModal() ==IDOK)
			{					
				m_vecAppList[m_iCurrSelectedIndex].strAppPath = dlgSelFile.m_setSelFilePathName;
				m_strGrid[m_iCurrSelectedIndex+1][1] = dlgSelFile.m_setSelFilePathName;
			}
		}
		break;
	case MAX_FOOT_INDEX-2:
		CDialog::OnOK();
		break;
	case MAX_FOOT_INDEX-1:
		CDialog::OnCancel();
		break;
	}
	this->Invalidate();

	CDialog::OnLButtonUp(nFlags, point);
}

BOOL COBDPNDDlgStartSetting::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;

	return CDialog::OnEraseBkgnd(pDC);
}

void COBDPNDDlgStartSetting::OnPaint()
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
			if (m_strGrid[i][1] == _T(""))
			{
				m_strGrid[i][1] = _T("点击此处修改软件路径...");
			}
			memDC.DrawText(m_strGrid[i][1], -1, &m_rectGrid[i][1], DT_LEFT|DT_VCENTER);

			if (i<=m_vecAppList.size() && m_vecAppList[i-1].bAutoRun)
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

void COBDPNDDlgStartSetting::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值	


	CDialog::OnTimer(nIDEvent);
}
