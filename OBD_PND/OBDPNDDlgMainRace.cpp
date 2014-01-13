// OBDPNDDlgMainRace.cpp : 实现文件
//

#include "stdafx.h"
#include "OBD_PND.h"
#include "OBDPNDDlgMainRace.h"
#include "OBD_PNDDlg.h"

// COBDPNDDlgMainRace 对话框

IMPLEMENT_DYNAMIC(COBDPNDDlgMainRace, CDialog)

COBDPNDDlgMainRace::COBDPNDDlgMainRace(CWnd* pParent /*=NULL*/)
	: CDialog(COBDPNDDlgMainRace::IDD, pParent)
{
	m_iKeyDownTime = 0;
	m_iKeyDownIndex = 0;
}

COBDPNDDlgMainRace::~COBDPNDDlgMainRace()
{
}

void COBDPNDDlgMainRace::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(COBDPNDDlgMainRace, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// COBDPNDDlgMainRace 消息处理程序

BOOL COBDPNDDlgMainRace::OnInitDialog()
{
	CDialog::OnInitDialog();

	fInitUI();

	return TRUE;
}

void COBDPNDDlgMainRace::fInitUI()
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
			fSetBkImg(this->GetDC(),IDR_PNG_MODE_RACE_800X480);

			WORD wUnitBmpHeight=55;
			WORD wUnitBmpWidth=61;

			m_tagDsInfo[0].rectData = CRect(55,100,240,180);  
			m_tagDsInfo[0].rectUnit = CRect(0,0,0,0);
			m_tagDsInfo[0].rectText = CRect(55,195,240,245);  

			m_tagDsInfo[1].rectData = CRect(52,290,255,355); 
			m_tagDsInfo[1].rectUnit = CRect(255-wUnitBmpWidth,355-wUnitBmpHeight,255,355);
			m_tagDsInfo[1].rectText = CRect(52,355,245,388); 

			m_tagDsInfo[2].rectData = CRect(300,55,505,120); 
			m_tagDsInfo[2].rectUnit = CRect(505-wUnitBmpWidth,120-wUnitBmpHeight,505,120); 
			m_tagDsInfo[2].rectText = CRect(300,120,500,155); 

			m_tagDsInfo[3].rectData = CRect(300,170,505,235); 
			m_tagDsInfo[3].rectUnit = CRect(505-wUnitBmpWidth,235-wUnitBmpHeight,505,235); 
			m_tagDsInfo[3].rectText = CRect(300,235,495,270); 

			m_tagDsInfo[4].rectData = CRect(300,290,505,357); 
			m_tagDsInfo[4].rectUnit = CRect(505-wUnitBmpWidth,357-wUnitBmpHeight,505,357); 
			m_tagDsInfo[4].rectText = CRect(300,357,495,390); 

			m_tagDsInfo[5].rectData = CRect(555,100,745,180); 
			m_tagDsInfo[5].rectUnit = CRect(0,0,0,0); 
			m_tagDsInfo[5].rectText = CRect(555,195,745,245); 

			m_tagDsInfo[6].rectData = CRect(555,290,755,355);
			m_tagDsInfo[6].rectUnit = CRect(755-wUnitBmpWidth,355-wUnitBmpHeight,755,355);
			m_tagDsInfo[6].rectText = CRect(555,355,745,388);

			VERIFY(m_fontDsDataValue.CreateFont(
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

			VERIFY(m_fontDsDataText.CreateFont(
				19,                        // nHeight
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

			VERIFY(m_fontValue.CreateFont(
				90,                        // nHeight
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
				34,                        // nHeight
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
	_TagUiDataRace tagUiData= theMainDlg->m_pConfigReadWrite->fGetRaceUiData();
	m_tagDsInfo[0].dwDsID = tagUiData.dwLeftUpID;
	CString strText = theMainDlg->fGetBinText(tagUiData.dwLeftUpID);
	m_tagDsInfo[0].strText = strText.Left(strText.Find(_T(" ")));
	m_tagDsInfo[0].strUnit = strText.Right(strText.GetLength() - strText.ReverseFind(' '));
	m_tagDsInfo[1].dwDsID = tagUiData.dwLeftDownID;
	strText = theMainDlg->fGetBinText(tagUiData.dwLeftDownID);
	m_tagDsInfo[1].strText = strText.Left(strText.Find(_T(" ")));
	m_tagDsInfo[1].strUnit = strText.Right(strText.GetLength() - strText.ReverseFind(' '));

	m_tagDsInfo[2].dwDsID = tagUiData.dwMiddleUpID;
	strText = theMainDlg->fGetBinText(tagUiData.dwMiddleUpID);
	m_tagDsInfo[2].strText = strText.Left(strText.Find(_T(" ")));
	m_tagDsInfo[2].strUnit = strText.Right(strText.GetLength() - strText.ReverseFind(' '));
	m_tagDsInfo[3].dwDsID = tagUiData.dwMiddleMiddleID;
	strText = theMainDlg->fGetBinText(tagUiData.dwMiddleMiddleID);
	m_tagDsInfo[3].strText = strText.Left(strText.Find(_T(" ")));
	m_tagDsInfo[3].strUnit = strText.Right(strText.GetLength() - strText.ReverseFind(' '));
	m_tagDsInfo[4].dwDsID = tagUiData.dwMiddleDownID;
	strText = theMainDlg->fGetBinText(tagUiData.dwMiddleDownID);
	m_tagDsInfo[4].strText = strText.Left(strText.Find(_T(" ")));
	m_tagDsInfo[4].strUnit = strText.Right(strText.GetLength() - strText.ReverseFind(' '));

	m_tagDsInfo[5].dwDsID = tagUiData.dwRightUpID;
	strText = theMainDlg->fGetBinText(tagUiData.dwRightUpID);
	m_tagDsInfo[5].strText = strText.Left(strText.Find(_T(" ")));
	m_tagDsInfo[5].strUnit = strText.Right(strText.GetLength() - strText.ReverseFind(' '));
	m_tagDsInfo[6].dwDsID = tagUiData.dwRightDownID;
	strText = theMainDlg->fGetBinText(tagUiData.dwRightDownID);
	m_tagDsInfo[6].strText = strText.Left(strText.Find(_T(" ")));
	m_tagDsInfo[6].strUnit = strText.Right(strText.GetLength() - strText.ReverseFind(' '));

	for (int i=0;i<7;i++)
	{
		m_tagDsInfo[i].strValue = _T("------"); 
		DWORD dwUnitResID = theMainDlg->fGetUnitResID(m_tagDsInfo[i].strUnit);
		fCommDrawImage(&m_tagDsInfo[i].memdcUnit,&m_tagDsInfo[i].cBmpUnit,&m_memdcBkBmp,m_tagDsInfo[i].rectUnit,AfxGetResourceHandle(),dwUnitResID);
	}

	m_tagDsInfo[0].strValue = _T("---");
	m_tagDsInfo[5].strValue = _T("---");

	this->Invalidate();
}

void COBDPNDDlgMainRace::fRefreshData()
{
	for (int i=0;i<7;i++)
	{
		m_tagDsInfo[i].strValue= theMainDlg->fGetDataStreamValue(m_tagDsInfo[i].dwDsID);	
		//即时油耗1(动态)
		if (m_tagDsInfo[i].dwDsID == 0xFF010001 &&
			m_tagDsInfo[i].strValue == INVALID_DATASTREAM_VALUE_STR)
		{
			if (theMainDlg->fGetDataStreamValue(0xFF010003)!=INVALID_DATASTREAM_VALUE_STR)
			{
				CString strText = theMainDlg->fGetBinText(0xFF010003);
				m_tagDsInfo[i].dwDsID = 0xFF010003;
				m_tagDsInfo[i].strText = strText.Left(strText.Find(_T(" ")));
				m_tagDsInfo[i].strUnit = strText.Right(strText.GetLength() - strText.ReverseFind(' '));

				theMainDlg->m_pConfigReadWrite->fSetRaceUiData(i,0xFF010003);
			}
		}	
		//即时油耗1(静态)
		if (m_tagDsInfo[i].dwDsID == 0xFF010002 &&
			m_tagDsInfo[i].strValue == INVALID_DATASTREAM_VALUE_STR)
		{
			if (theMainDlg->fGetDataStreamValue(0xFF010004)!=INVALID_DATASTREAM_VALUE_STR)
			{
				CString strText = theMainDlg->fGetBinText(0xFF010004);
				m_tagDsInfo[i].dwDsID = 0xFF010004;
				m_tagDsInfo[i].strText = strText.Left(strText.Find(_T(" ")));
				m_tagDsInfo[i].strUnit = strText.Right(strText.GetLength() - strText.ReverseFind(' '));

				theMainDlg->m_pConfigReadWrite->fSetRaceUiData(i,0xFF010004);
			}
		}	
		//即时油耗2(动态)
		if (m_tagDsInfo[i].dwDsID == 0xFF010003 &&
			m_tagDsInfo[i].strValue == INVALID_DATASTREAM_VALUE_STR)
		{
			if (theMainDlg->fGetDataStreamValue(0xFF010001)!=INVALID_DATASTREAM_VALUE_STR)
			{
				CString strText = theMainDlg->fGetBinText(0xFF010001);
				m_tagDsInfo[i].dwDsID = 0xFF010001;
				m_tagDsInfo[i].strText = strText.Left(strText.Find(_T(" ")));
				m_tagDsInfo[i].strUnit = strText.Right(strText.GetLength() - strText.ReverseFind(' '));

				theMainDlg->m_pConfigReadWrite->fSetRaceUiData(i,0xFF010001);
			}
		}	
		//即时油耗2(静态)
		if (m_tagDsInfo[i].dwDsID == 0xFF010004 &&
			m_tagDsInfo[i].strValue == INVALID_DATASTREAM_VALUE_STR)
		{
			if (theMainDlg->fGetDataStreamValue(0xFF010002)!=INVALID_DATASTREAM_VALUE_STR)
			{
				CString strText = theMainDlg->fGetBinText(0xFF010002);
				m_tagDsInfo[i].dwDsID = 0xFF010002;
				m_tagDsInfo[i].strText = strText.Left(strText.Find(_T(" ")));
				m_tagDsInfo[i].strUnit = strText.Right(strText.GetLength() - strText.ReverseFind(' '));


				theMainDlg->m_pConfigReadWrite->fSetRaceUiData(i,0xFF010002);
			}
		}	
		//空气流量
		if (m_tagDsInfo[i].dwDsID == 0x00001000 &&
			m_tagDsInfo[i].strValue == INVALID_DATASTREAM_VALUE_STR)
		{
			if (theMainDlg->fGetDataStreamValue(0x00000B00)!=INVALID_DATASTREAM_VALUE_STR)
			{
				CString strText = theMainDlg->fGetBinText(0x00000B00);
				m_tagDsInfo[i].dwDsID = 0x00000B00;
				m_tagDsInfo[i].strText = strText.Left(strText.Find(_T(" ")));
				m_tagDsInfo[i].strUnit = strText.Right(strText.GetLength() - strText.ReverseFind(' '));

				DWORD dwUnitResID = theMainDlg->fGetUnitResID(m_tagDsInfo[i].strUnit);
				fCommDrawImage(&m_tagDsInfo[i].memdcUnit,&m_tagDsInfo[i].cBmpUnit,&m_memdcBkBmp,m_tagDsInfo[i].rectUnit,AfxGetResourceHandle(),dwUnitResID);
				theMainDlg->m_pConfigReadWrite->fSetRaceUiData(i,0x00000B00);
			}
		}	
		//进气压力
		if (m_tagDsInfo[i].dwDsID == 0x00000B00 &&
			m_tagDsInfo[i].strValue == INVALID_DATASTREAM_VALUE_STR)
		{
			if (theMainDlg->fGetDataStreamValue(0x00001000)!=INVALID_DATASTREAM_VALUE_STR)
			{
				CString strText = theMainDlg->fGetBinText(0x00001000);
				m_tagDsInfo[i].dwDsID = 0x00001000;
				m_tagDsInfo[i].strText = strText.Left(strText.Find(_T(" ")));
				m_tagDsInfo[i].strUnit = strText.Right(strText.GetLength() - strText.ReverseFind(' '));

				DWORD dwUnitResID = theMainDlg->fGetUnitResID(m_tagDsInfo[i].strUnit);
				fCommDrawImage(&m_tagDsInfo[i].memdcUnit,&m_tagDsInfo[i].cBmpUnit,&m_memdcBkBmp,m_tagDsInfo[i].rectUnit,AfxGetResourceHandle(),dwUnitResID);
				theMainDlg->m_pConfigReadWrite->fSetRaceUiData(i,0x00001000);
			}
		}	
	}
	if (m_tagDsInfo[0].strValue == _T("------"))
		m_tagDsInfo[0].strValue = _T("---");
	if (m_tagDsInfo[5].strValue == _T("------"))
		m_tagDsInfo[5].strValue = _T("---");

	this->Invalidate();
}

void COBDPNDDlgMainRace::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	for (int i=0;i<7;i++)
	{
		if (point.x>m_tagDsInfo[i].rectText.left && point.x<m_tagDsInfo[i].rectText.right &&
			point.y>m_tagDsInfo[i].rectData.top && point.y<m_tagDsInfo[i].rectText.bottom)
		{
			m_iKeyDownIndex = i;
			m_iKeyDownTime =  GetTickCount();
			SetTimer(1,200,NULL);
			break;
		}
	}
	CDialog::OnLButtonDown(nFlags, point);
}

void COBDPNDDlgMainRace::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	KillTimer(1);

	WORD dwIndex = fGetCurrFootIndex(point);
	theMainDlg->fSetCurrPanelIndex(dwIndex);

	m_iKeyDownIndex = 0;
	m_iKeyDownTime =  0;
	CDialog::OnLButtonUp(nFlags, point);
}

BOOL COBDPNDDlgMainRace::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;

	return CDialog::OnEraseBkgnd(pDC);
}

void COBDPNDDlgMainRace::OnPaint()
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

		for (int i=0;i<7;i++)
		{
			if (i==0 || i==5)
			{
				memDC.SetTextColor(RGB(0,0,0));
				memDC.SelectObject(m_fontValue);
				memDC.DrawText(m_tagDsInfo[i].strValue,-1,m_tagDsInfo[i].rectData,DT_VCENTER|DT_CENTER);
				memDC.SelectObject(m_fontText);
				memDC.DrawText(m_tagDsInfo[i].strText,-1,m_tagDsInfo[i].rectText,DT_VCENTER|DT_CENTER);
			}
			else
			{
				memDC.SetTextColor(theMainDlg->m_clrTitle);
				//没有单位居中显示
				if (theMainDlg->fGetUnitResID(m_tagDsInfo[i].strUnit) == NULL)
				{
					memDC.SelectObject(m_fontDsDataValue);
					memDC.DrawText(m_tagDsInfo[i].strValue,-1,m_tagDsInfo[i].rectData,DT_BOTTOM|DT_CENTER);
					memDC.SelectObject(m_fontDsDataText);
					memDC.DrawText(m_tagDsInfo[i].strText,-1,m_tagDsInfo[i].rectText,DT_VCENTER|DT_CENTER);
				}
				else
				{
					CRect rectTemp = m_tagDsInfo[i].rectData;
					rectTemp.right =m_tagDsInfo[i].rectUnit.left; 
					memDC.SelectObject(m_fontDsDataValue);
					memDC.DrawText(m_tagDsInfo[i].strValue,-1,rectTemp,DT_BOTTOM|DT_RIGHT);
					memDC.SelectObject(m_fontDsDataText);
					memDC.DrawText(m_tagDsInfo[i].strText,-1,m_tagDsInfo[i].rectText,DT_VCENTER|DT_CENTER);

					memDC.BitBlt(m_tagDsInfo[i].rectUnit.left,m_tagDsInfo[i].rectUnit.top,m_tagDsInfo[i].rectUnit.Width(),m_tagDsInfo[i].rectUnit.Height(),	
						&m_tagDsInfo[i].memdcUnit,0,0,SRCCOPY);
				}
			}

		}

		//
		memDC.SelectObject(theMainDlg->m_fontFootNormal);
		for (int i=0;i<MAX_FOOT_INDEX;i++)
		{
			if (theMainDlg->fGetCurrPanelIndex()== i)
				memDC.SetTextColor(theMainDlg->m_clrFootFocus);   
			else
				memDC.SetTextColor(theMainDlg->m_clrFootNormal); 		
			memDC.DrawText(theMainDlg->m_strFoot[i], -1, &theMainDlg->m_rectFoot[i], DT_CENTER);
		}

		//将背景位图复制到窗口客户区 
		pDC.BitBlt(0,0,rect.Width(),rect.Height(),&memDC,0,0,SRCCOPY); 

		if(pOldMemBmp)   
			memDC.SelectObject(pOldMemBmp); 

		DeleteObject(pOldMemBmp);	
		DeleteDC(memDC);

	}
	CDialog::OnPaint();
}

void COBDPNDDlgMainRace::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值	

	if (GetTickCount() - m_iKeyDownTime > 1500)
	{
		KillTimer(1);
		//CDelphiDlgConfigShowData dlgConfigShowData;
		//dlgConfigShowData.fSetSelectedDataID(m_tagDsInfo[m_iKeyDownIndex].dwDsID );
		//if(dlgConfigShowData.DoModal() == IDOK)
		//{
		//	DWORD dwNewDataID = dlgConfigShowData.fGetSelectedDataID();
		//	CString strText = theMainDlg->fGetBinText(dwNewDataID);
		//	m_tagDsInfo[m_iKeyDownIndex].dwDsID = dwNewDataID;
		//	m_tagDsInfo[m_iKeyDownIndex].strText = strText.Left(strText.Find(_T(" ")));
		//	m_tagDsInfo[m_iKeyDownIndex].strUnit = strText.Right(strText.GetLength() - strText.ReverseFind(' '));

		//	m_tagDsInfo[m_iKeyDownIndex].memdcUnit.DeleteDC();
		//	m_tagDsInfo[m_iKeyDownIndex].cBmpUnit.DeleteObject();

		//	DWORD dwUnitResID = theMainDlg->fGetUnitResID(m_tagDsInfo[m_iKeyDownIndex].strUnit);
		//	fCommDrawImage(&m_tagDsInfo[m_iKeyDownIndex].memdcUnit,&m_tagDsInfo[m_iKeyDownIndex].cBmpUnit,&m_memdcBk,
		//		m_tagDsInfo[m_iKeyDownIndex].rectUnit,g_hResDll,dwUnitResID);
		//}
	}


	CDialog::OnTimer(nIDEvent);
}
