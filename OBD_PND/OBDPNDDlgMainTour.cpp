// OBDPNDDlgMainTour.cpp : 实现文件
//

#include "stdafx.h"
#include "OBD_PND.h"
#include "OBDPNDDlgMainTour.h"
#include "OBD_PNDDlg.h"

#define		PT_NUM		100 
#define		TABNUM		1
#define		PI			3.1415926535
// COBDPNDDlgMainTour 对话框

IMPLEMENT_DYNAMIC(COBDPNDDlgMainTour, CDialog)

COBDPNDDlgMainTour::COBDPNDDlgMainTour(CWnd* pParent /*=NULL*/)
	: CDialog(COBDPNDDlgMainTour::IDD, pParent)
{
	m_iKeyDownTime = 0;
	m_iKeyDownIndex = 0;

}

COBDPNDDlgMainTour::~COBDPNDDlgMainTour()
{
}

void COBDPNDDlgMainTour::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(COBDPNDDlgMainTour, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// COBDPNDDlgMainTour 消息处理程序

BOOL COBDPNDDlgMainTour::OnInitDialog()
{
	CDialog::OnInitDialog();

	fInitUI();

	return TRUE;
}

void COBDPNDDlgMainTour::fInitUI()
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
			fSetBkImg(this->GetDC(),IDR_PNG_MODE_TOUR_800X480);

			WORD wUnitBmpHeight=55;
			WORD wUnitBmpWidth=61;

			m_tagDsInfo[0].rectData = CRect(35,55,230,120);  
			m_tagDsInfo[0].rectUnit = CRect(230-wUnitBmpWidth,120-wUnitBmpHeight,230,120);
			m_tagDsInfo[0].rectText = CRect(35,123,228,152);  

			m_tagDsInfo[1].rectData = CRect(35,175,230,238); 
			m_tagDsInfo[1].rectUnit = CRect(230-wUnitBmpWidth,235-wUnitBmpHeight,230,238);
			m_tagDsInfo[1].rectText = CRect(35,238,228,270); 

			m_tagDsInfo[2].rectData = CRect(35,290,230,355); 
			m_tagDsInfo[2].rectUnit = CRect(230-wUnitBmpWidth,357-wUnitBmpHeight,230,355);
			m_tagDsInfo[2].rectText = CRect(35,359,228,390); 

			m_tagDsInfo[3].rectData = CRect(570,55,762,120); 
			m_tagDsInfo[3].rectUnit = CRect(762-wUnitBmpWidth,120-wUnitBmpHeight,762,120);
			m_tagDsInfo[3].rectText = CRect(570,123,760,152); 

			m_tagDsInfo[4].rectData = CRect(570,175,762,238);
			m_tagDsInfo[4].rectUnit = CRect(762-wUnitBmpWidth,235-wUnitBmpHeight,762,238);
			m_tagDsInfo[4].rectText = CRect(570,238,760,270);

			m_tagDsInfo[5].rectData = CRect(570,290,762,355);	                                                         
			m_tagDsInfo[5].rectUnit = CRect(762-wUnitBmpWidth,357-wUnitBmpHeight,762,355);	                                                         
			m_tagDsInfo[5].rectText = CRect(570,359,760,390);

			m_rectSpeed = CRect(305,190,490,310);
			m_rectTime = CRect(230,40,570,90);
			m_rectMeterRight = CRect(252,101,252+296,101+296);

			m_pointCompass = CPoint(400,339);
			m_dRaduis = 50;
			m_wPointLen = 35;
			m_wCompassPointLenght = 23;
			m_wCompassPointLenght2 = 3;

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

			VERIFY(m_fontSpeed.CreateFont(
				130,                        // nHeight
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
			
			VERIFY(m_fontTime.CreateFont(
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
		}
		break;
	}
	_TagUiDataTour tagUiData= theMainDlg->m_pConfigReadWrite->fGetTourUiData();
	m_tagDsInfo[0].dwDsID = tagUiData.dwLeftUpID;
	CString strText = theMainDlg->fGetBinText(tagUiData.dwLeftUpID);
	m_tagDsInfo[0].strText = strText.Left(strText.Find(_T(" ")));
	m_tagDsInfo[0].strUnit = strText.Right(strText.GetLength() - strText.ReverseFind(' '));

	m_tagDsInfo[1].dwDsID = tagUiData.dwLeftMiddleID;
	strText = theMainDlg->fGetBinText(tagUiData.dwLeftMiddleID);
	m_tagDsInfo[1].strText = strText.Left(strText.Find(_T(" ")));
	m_tagDsInfo[1].strUnit = strText.Right(strText.GetLength() - strText.ReverseFind(' '));

	m_tagDsInfo[2].dwDsID = tagUiData.dwLeftDownID;
	strText = theMainDlg->fGetBinText(tagUiData.dwLeftDownID);
	m_tagDsInfo[2].strText = strText.Left(strText.Find(_T(" ")));
	m_tagDsInfo[2].strUnit = strText.Right(strText.GetLength() - strText.ReverseFind(' '));

	m_tagDsInfo[3].dwDsID = tagUiData.dwRightUpID;
	strText = theMainDlg->fGetBinText(tagUiData.dwRightUpID);
	m_tagDsInfo[3].strText = strText.Left(strText.Find(_T(" ")));
	m_tagDsInfo[3].strUnit = strText.Right(strText.GetLength() - strText.ReverseFind(' '));

	m_tagDsInfo[4].dwDsID = tagUiData.dwRightMiddleID;
	strText = theMainDlg->fGetBinText(tagUiData.dwRightMiddleID);
	m_tagDsInfo[4].strText = strText.Left(strText.Find(_T(" ")));
	m_tagDsInfo[4].strUnit = strText.Right(strText.GetLength() - strText.ReverseFind(' '));

	m_tagDsInfo[5].dwDsID = tagUiData.dwRightDownID;
	strText = theMainDlg->fGetBinText(tagUiData.dwRightDownID);
	m_tagDsInfo[5].strText = strText.Left(strText.Find(_T(" ")));
	m_tagDsInfo[5].strUnit = strText.Right(strText.GetLength() - strText.ReverseFind(' '));

	for (int i=0;i<6;i++)
	{
		m_tagDsInfo[i].strValue = _T("------"); 
		if (m_tagDsInfo[i].strUnit != _T(""))
		{
			DWORD dwUnitResID = theMainDlg->fGetUnitResID(m_tagDsInfo[i].strUnit);
			fCommDrawImage(&m_tagDsInfo[i].memdcUnit,&m_tagDsInfo[i].cBmpUnit,&m_memdcBkBmp,m_tagDsInfo[i].rectUnit,AfxGetResourceHandle(),dwUnitResID);
		}
	}

	CTime time = CTime::GetCurrentTime();
	m_strTime.Format(_T("%02d:%02d"),time.GetHour(),time.GetMinute());
	m_strSpeed =  _T("---"); 

	m_clrSpeedAlarm = RGB(255,0,0);
	this->Invalidate();
}

void COBDPNDDlgMainTour::fRefreshData()
{
	CTime time = CTime::GetCurrentTime();
	m_strTime.Format(_T("%02d:%02d"),time.GetHour(),time.GetMinute());

	for (int i=0;i<6;i++)
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

				theMainDlg->m_pConfigReadWrite->fSetTourUiData(i,0xFF010003);
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

				theMainDlg->m_pConfigReadWrite->fSetTourUiData(i,0xFF010004);
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

				theMainDlg->m_pConfigReadWrite->fSetTourUiData(i,0xFF010001);
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


				theMainDlg->m_pConfigReadWrite->fSetTourUiData(i,0xFF010002);
			}
		}	
	}
	m_strSpeed = theMainDlg->fGetDataStreamValue(0x00000D00);	
	if (m_strSpeed == _T("------"))
		m_strSpeed = _T("---");

	m_fCompassValue = theMainDlg->fGetCompassValue();

	this->Invalidate();
}

void COBDPNDDlgMainTour::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	for (int i=0;i<6;i++)
	{
		if (point.x>m_tagDsInfo[i].rectData.left && point.x<m_tagDsInfo[i].rectUnit.right &&
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

void COBDPNDDlgMainTour::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	KillTimer(1);

	WORD dwIndex = fGetCurrFootIndex(point);
	theMainDlg->fSetCurrPanelIndex(dwIndex);
	//if (dwIndex<3)
	//	theMainDlg->fPauseCheckAutoSelectPanel();

	m_iKeyDownIndex = 0;
	m_iKeyDownTime =  0;
	CDialog::OnLButtonUp(nFlags, point);
}

BOOL COBDPNDDlgMainTour::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;

	return CDialog::OnEraseBkgnd(pDC);
}

void COBDPNDDlgMainTour::OnPaint()
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

		memDC.SelectObject(m_fontSpeed);
		//速度
		float fValue = theMainDlg->fGetDataStreamValueF(0x00000D00);
		if (fValue == 0)
			fValue = 0;
		double dSSPecent = fValue*100.0/theMainDlg->m_pConfigReadWrite->fGetOverSpeedAlarmValue();
		CString str; 
		str.Format(_T("%0.0f"),dSSPecent);
		if (dSSPecent>=100)
			memDC.SetTextColor(theMainDlg->m_clrTitle);
		else if (dSSPecent>=80)
			memDC.SetTextColor(RGB(244,78,6));	
		else 
			memDC.SetTextColor(RGB(0,255,0));	

		memDC.DrawText(m_strSpeed,-1,m_rectSpeed,DT_BOTTOM|DT_CENTER);

		memDC.SetTextColor(theMainDlg->m_clrTitle);
		memDC.SelectObject(m_fontTime);
		memDC.DrawText(m_strTime,-1,m_rectTime,DT_VCENTER|DT_CENTER);

		for (int i=0;i<6;i++)
		{
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

		////画时速指针
		//fDrawPoint(&memDC,m_rectMeterRight,_ttoi(m_strSpeed));	

		m_fCompassValue = theMainDlg->fGetCompassValue();
		//画指南针指针
		fDrawCompass(&memDC,m_pointCompass,6,m_fCompassValue,m_wCompassPointLenght,m_wCompassPointLenght2);

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

void COBDPNDDlgMainTour::OnTimer(UINT_PTR nIDEvent)
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

//************************************************************************************
//函    数： fDrawPoint(CDC * dc,CPoint pointCircle,int iValue)
//功    能： 根据圆心，半径，角度画指针到DC上
//参    数：dc------------目标DC
//			pointCircle---圆心坐标
//			iValue--------指针值
//返    回: 
//例	如： 
//************************************************************************************
int COBDPNDDlgMainTour::fDrawPoint(CDC * dc,CRect rectCircle,int iValue)
{	
	if (dc == NULL)
		return 0;
	
	int iLen = 50;										//指针长度

	CPoint pointCircle = CPoint(rectCircle.left + rectCircle.Width()/2,rectCircle.top + rectCircle.Height()/2);

	float fIndex = float((iValue)/4);				//刻度索引
	double dAngle = (60+fIndex*4);						//角度索引
	
	COLORREF clrPoint = RGB(231,31,25);					//表盘箭头颜色
	COLORREF clrHand  =  RGB(231,31,25);				//指针颜色
	COLORREF clrArc  =  RGB(0,250,22);					//圆弧颜色
			
	//画指针
	CPoint ptTemp[4];		
	ptTemp[0].x = (int)(pointCircle.x+(80)*cos((double)((dAngle+89)*PI/180.0)));
	ptTemp[0].y = (int)(pointCircle.y+(80)*sin((double)((dAngle+89)*PI/180.0)));	

	ptTemp[1].x = (int)(pointCircle.x+(80 + iLen)*cos((double)((dAngle+90)*PI/180.0)));
	ptTemp[1].y = (int)(pointCircle.y+(80 + iLen)*sin((double)((dAngle+90)*PI/180.0)));			

	ptTemp[2].x = (int)(pointCircle.x+(80 + iLen)*cos((double)((dAngle+91)*PI/180.0)));
	ptTemp[2].y = (int)(pointCircle.y+(80 + iLen)*sin((double)((dAngle+91)*PI/180.0)));	

	ptTemp[3].x = (int)(pointCircle.x+(80)*cos((double)((dAngle+92)*PI/180.0)));
	ptTemp[3].y = (int)(pointCircle.y+(80)*sin((double)((dAngle+92)*PI/180.0)));	

	CPen penBound(0,1,clrHand);
	CPen* pOldPen = dc->SelectObject(&penBound);
	CBrush brushBound(clrHand);
	CBrush* pOldBrush = dc->SelectObject(&brushBound); 

	dc->Polygon(ptTemp,4); 

	dc->SelectObject(pOldPen);
	dc->SelectObject(pOldBrush);
	DeleteObject(penBound);
	DeleteObject(brushBound);

	////画箭头
	//CPoint ptTemp2[5];		 
	//ptTemp2[0].x = (int)(pointCircle.x+(138)*cos((double)((dAngle+91)*PI/180.0)));
	//ptTemp2[0].y = (int)(pointCircle.y+(138)*sin((double)((dAngle+91)*PI/180.0)));	

	//ptTemp2[1].x = (int)(pointCircle.x+(145)*cos((double)((dAngle+90)*PI/180.0)));
	//ptTemp2[1].y = (int)(pointCircle.y+(145)*sin((double)((dAngle+90)*PI/180.0)));	

	//ptTemp2[2].x = (int)(pointCircle.x+(150)*cos((double)((dAngle+90)*PI/180.0)));
	//ptTemp2[2].y = (int)(pointCircle.y+(150)*sin((double)((dAngle+90)*PI/180.0)));	

	//ptTemp2[3].x = (int)(pointCircle.x+(150)*cos((double)((dAngle+92)*PI/180.0)));
	//ptTemp2[3].y = (int)(pointCircle.y+(150)*sin((double)((dAngle+92)*PI/180.0)));

	//ptTemp2[4].x = (int)(pointCircle.x+(145)*cos((double)((dAngle+92)*PI/180.0)));
	//ptTemp2[4].y = (int)(pointCircle.y+(145)*sin((double)((dAngle+92)*PI/180.0)));		
	//
	//CPen penBound2(0,1,clrPoint);
	//CPen* pOldPen2 = dc->SelectObject(&penBound2);
	//dc->SelectObject(penBound2);

	//CBrush brushBound2(clrPoint);
	//CBrush* pOldBrush2 = dc->SelectObject(&brushBound2);
	//dc->SelectObject(brushBound2);

	//dc->Polygon(ptTemp2,5);

	//dc->SelectObject(pOldPen2);
	//dc->SelectObject(pOldBrush2);	
	//DeleteObject(penBound2);
	//DeleteObject(brushBound2);	

	////画圆弧
	//double dRadPerDeg = 4.0*atan(1.0)/180.0;
	//double dStartAngle = 210;							//起始角度
	//double dLen = (iValue)/240.0 * 2.4;				//车速角度偏移参数，240刻度，每4.8

	//CPoint ptGroup1[PT_NUM*TABNUM+1], ptGroup2[PT_NUM*TABNUM+1];
	//CPoint ptRgn[PT_NUM*2+2];
	//CBrush brushFill;
	//CBrush *pBrushOld ;
	//CPen penDraw ;
	//CPen *pPenOld;

	//double dOuter = rectCircle.Width()/2-6.5;			//弧圈外圈
	//double dInner = rectCircle.Width()/2-10.5;			//弧圈内线
	//// 获取点的位置
	//for(int i=0; i<=PT_NUM*TABNUM; i++)
	//{
	//	ptGroup1[i].x = pointCircle.x + (int)(dOuter*cos((dStartAngle-i*dLen)*dRadPerDeg)); 
	//	ptGroup1[i].y = pointCircle.y - (int)(dOuter*sin((dStartAngle-i*dLen)*dRadPerDeg));
	//	ptGroup2[i].x = pointCircle.x + (int)(dInner*cos((dStartAngle-i*dLen)*dRadPerDeg));
	//	ptGroup2[i].y = pointCircle.y - (int)(dInner*sin((dStartAngle-i*dLen)*dRadPerDeg));
	//}

	//// 
	//for(int i=0; i<TABNUM; i++)
	//{
	//	//确定区域
	//	for(int j=0; j<=PT_NUM; j++) 
	//	{
	//		ptRgn[j] = ptGroup1[i*PT_NUM+j];
	//		ptRgn[2*PT_NUM+1-j] = ptGroup2[i*PT_NUM+j];
	//	}
	//	brushFill.DeleteObject();
	//	brushFill.CreateSolidBrush(clrArc);
	//	pBrushOld = dc->SelectObject(&brushFill);
	//	penDraw.DeleteObject();
	//	penDraw.CreatePen(PS_SOLID, 1, clrArc);
	//	pPenOld = dc->SelectObject(&penDraw);
	//	dc->Polygon(ptRgn, 2*PT_NUM+2);
	//	dc->SelectObject(pBrushOld); 
	//	dc->SelectObject(pPenOld);
	//}

	//DeleteObject(pBrushOld);
	//DeleteObject(pPenOld);
	//DeleteObject(penDraw);
	//DeleteObject(pPenOld);

	return 0;   
}



int COBDPNDDlgMainTour::fDrawCompass(CDC * dc,CPoint pointCircle,int iRaduis,double dAngle ,int iLen,int iLen2)
{	
	if (dc == NULL)
		return 0;	

	CPoint ptTemp[3];	
	ptTemp[0].x = (int)(pointCircle.x+(iRaduis-15)*cos((double)((dAngle+90)*PI/180.0)));
	ptTemp[0].y = (int)(pointCircle.y+(iRaduis-15)*sin((double)((dAngle+90)*PI/180.0)));

	ptTemp[1].x = (int)(pointCircle.x+(iRaduis+iLen)*cos((double)((dAngle+90)*PI/180.0)));
	ptTemp[1].y = (int)(pointCircle.y+(iRaduis+iLen)*sin((double)((dAngle+90)*PI/180.0)));

	ptTemp[2].x = (int)(pointCircle.x+(iRaduis+iLen2)*cos((double)((dAngle+180)*PI/180.0)));
	ptTemp[2].y = (int)(pointCircle.y+(iRaduis+iLen2)*sin((double)((dAngle+180)*PI/180.0)));	

	CPen penBound(0,1,RGB(0,255,0));
	CPen* pOldPen = dc->SelectObject(&penBound);
	CBrush brushBound(RGB(0,255,0));
	CBrush* pOldBrush = dc->SelectObject(&brushBound);

	dc->Polygon(ptTemp,3);

	dc->SelectObject(pOldPen);
	DeleteObject(pOldPen);
	dc->SelectObject(pOldBrush);
	DeleteObject(pOldBrush);


	CPoint ptTemp2[3];	
	ptTemp2[0].x = (int)(pointCircle.x+(iRaduis-15)*cos((double)((dAngle+90)*PI/180.0)));
	ptTemp2[0].y = (int)(pointCircle.y+(iRaduis-15)*sin((double)((dAngle+90)*PI/180.0)));

	ptTemp2[1].x = (int)(pointCircle.x+(iRaduis+iLen)*cos((double)((dAngle+90)*PI/180.0)));
	ptTemp2[1].y = (int)(pointCircle.y+(iRaduis+iLen)*sin((double)((dAngle+90)*PI/180.0)));

	ptTemp2[2].x = (int)(pointCircle.x+(iRaduis+iLen2)*cos((double)((dAngle)*PI/180.0)));
	ptTemp2[2].y = (int)(pointCircle.y+(iRaduis+iLen2)*sin((double)((dAngle)*PI/180.0)));	

	CPen penBound2(0,1,RGB(0,255,0));
	CPen* pOldPen2 = dc->SelectObject(&penBound2);

	CBrush brushBound2(RGB(10,255,10));
	CBrush* pOldBrush2 = dc->SelectObject(&brushBound2);

	dc->Polygon(ptTemp2,3);

	dc->SelectObject(pOldPen2);
	dc->SelectObject(pOldBrush2);
	DeleteObject(pOldPen2);
	DeleteObject(pOldBrush2);


	//画两个圆心
	CBrush brushBound3(RGB(161,29,131));
	CBrush* pOldBrush3 = dc->SelectObject(&brushBound3);
	dc->Ellipse(pointCircle.x+5,pointCircle.y+5,pointCircle.x-5,pointCircle.y-5);
	dc->SelectObject(brushBound3);
	DeleteObject(brushBound3);	

	CBrush brushBound4(RGB(55,80,192));
	CBrush* pOldBrush4 = dc->SelectObject(&brushBound4);
	dc->Ellipse(pointCircle.x+3,pointCircle.y+3,pointCircle.x-3,pointCircle.y-3);
	dc->SelectObject(brushBound4);
	DeleteObject(brushBound4);	

	return 0;   
}
