// OBDPNDDlgMainIdle.cpp : 实现文件
//

#include "stdafx.h"
#include "OBD_PND.h"
#include "OBDPNDDlgMainIdle.h"
#include "OBD_PNDDlg.h"

// COBDPNDDlgMainIdle 对话框
#define M_PI 3.14159265358979323846

IMPLEMENT_DYNAMIC(COBDPNDDlgMainIdle, CDialog)

COBDPNDDlgMainIdle::COBDPNDDlgMainIdle(CWnd* pParent /*=NULL*/)
	: CDialog(COBDPNDDlgMainIdle::IDD, pParent)
{
	m_iKeyDownTime = 0;
	m_iKeyDownIndex = 0;

}

COBDPNDDlgMainIdle::~COBDPNDDlgMainIdle()
{
}

void COBDPNDDlgMainIdle::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(COBDPNDDlgMainIdle, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// COBDPNDDlgMainIdle 消息处理程序

// CDelphiDlgModeIdle 消息处理程序
BOOL COBDPNDDlgMainIdle::OnInitDialog()
{
	CDialog::OnInitDialog();

	fInitUI();

	return TRUE;
}
void COBDPNDDlgMainIdle::fInitUI()
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
			fSetBkImg(this->GetDC(),IDR_PNG_MODE_IDLE_800X480);

			WORD wUnitBmpHeight=55;
			WORD wUnitBmpWidth=61;

			m_tagDsInfo[0].rectData = CRect(37,41,230,102);  
			m_tagDsInfo[0].rectUnit = CRect(230-wUnitBmpWidth,102-wUnitBmpHeight,230,102);
			m_tagDsInfo[0].rectText = CRect(37,102,228,140);  

			m_tagDsInfo[1].rectData = CRect(37,153,230,214); 
			m_tagDsInfo[1].rectUnit = CRect(230-wUnitBmpWidth,214-wUnitBmpHeight,230,214);
			m_tagDsInfo[1].rectText = CRect(37,214,228,250); 

			m_tagDsInfo[2].rectData = CRect(37,265,230,328); 
			m_tagDsInfo[2].rectUnit = CRect(230-wUnitBmpWidth,328-wUnitBmpHeight,230,328);
			m_tagDsInfo[2].rectText = CRect(37,330,228,360); 

			m_tagDsInfo[3].rectData = CRect(570,41,762,102); 
			m_tagDsInfo[3].rectUnit = CRect(762-wUnitBmpWidth,102-wUnitBmpHeight,762,102);
			m_tagDsInfo[3].rectText = CRect(570,102,760,140); 

			m_tagDsInfo[4].rectData = CRect(570,153,762,214);
			m_tagDsInfo[4].rectUnit = CRect(762-wUnitBmpWidth,214-wUnitBmpHeight,762,214);
			m_tagDsInfo[4].rectText = CRect(570,214,760,250);

			m_tagDsInfo[5].rectData = CRect(570,265,762,328);	                                                         
			m_tagDsInfo[5].rectUnit = CRect(762-wUnitBmpWidth,328-wUnitBmpHeight,762,328);	                                                         
			m_tagDsInfo[5].rectText = CRect(570,330,760,360);

			m_rectDate = CRect(12,377,228,426);
			m_rectWeek = CRect(570,377,760,426);
			m_rectTime = CRect(228,360,570,420);

			m_rectWater = CRect(270,78,272+260,78+260);
			m_rectWaterTempValue = CRect(254,50,546,352);
			m_rectWaterTempText	= CRect(290,270,535,333);

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

			VERIFY(m_fontWaterTempText.CreateFont(
				50,                        // nHeight
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

			VERIFY(m_fontWaterTempValue.CreateFont(
				165,                        // nHeight
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

			VERIFY(m_fontDate.CreateFont(
				35,                        // nHeight
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
				54,                        // nHeight
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

			VERIFY(m_fontWeek.CreateFont(
				35,                        // nHeight
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
	_TagUiDataIdle tagUiData= theMainDlg->m_pConfigReadWrite->fGetIdleUiData();
	
	m_tagDsInfo[0].dwDsID = tagUiData.dwLeftUpID;
	CString strText = theMainDlg->fGetBinText(tagUiData.dwLeftUpID);
	m_tagDsInfo[0].strText = strText.Left(strText.Find(_T(" ")));
	m_tagDsInfo[0].strUnit = strText.Right(strText.GetLength() - strText.ReverseFind(' '));

	strText = theMainDlg->fGetBinText(tagUiData.dwLeftMiddleID);
	m_tagDsInfo[1].dwDsID = tagUiData.dwLeftMiddleID;
	m_tagDsInfo[1].strText = strText.Left(strText.Find(_T(" ")));
	m_tagDsInfo[1].strUnit = strText.Right(strText.GetLength() - strText.ReverseFind(' '));

	strText = theMainDlg->fGetBinText(tagUiData.dwLeftDownID);
	m_tagDsInfo[2].dwDsID = tagUiData.dwLeftDownID;
	m_tagDsInfo[2].strText = strText.Left(strText.Find(_T(" ")));
	m_tagDsInfo[2].strUnit = strText.Right(strText.GetLength() - strText.ReverseFind(' '));

	strText = theMainDlg->fGetBinText(tagUiData.dwRightUpID);
	m_tagDsInfo[3].dwDsID = tagUiData.dwRightUpID;
	m_tagDsInfo[3].strText = strText.Left(strText.Find(_T(" ")));
	m_tagDsInfo[3].strUnit = strText.Right(strText.GetLength() - strText.ReverseFind(' '));

	strText = theMainDlg->fGetBinText(tagUiData.dwRightMiddleID);
	m_tagDsInfo[4].dwDsID = tagUiData.dwRightMiddleID;
	m_tagDsInfo[4].strText = strText.Left(strText.Find(_T(" ")));
	m_tagDsInfo[4].strUnit = strText.Right(strText.GetLength() - strText.ReverseFind(' '));

	strText = theMainDlg->fGetBinText(tagUiData.dwRightDownID);
	m_tagDsInfo[5].dwDsID = tagUiData.dwRightDownID;
	m_tagDsInfo[5].strText = strText.Left(strText.Find(_T(" ")));
	m_tagDsInfo[5].strUnit = strText.Right(strText.GetLength() - strText.ReverseFind(' '));

	for (int i=0;i<6;i++)
	{
		m_tagDsInfo[i].strValue = _T("------"); 
		m_tagDsInfo[i].iDecLen = _ttoi(m_tagDsInfo[i].strUnit.Right(1));
		DWORD dwUnitResID = theMainDlg->fGetUnitResID(m_tagDsInfo[i].strUnit);
		fCommDrawImage(&m_tagDsInfo[i].memdcUnit,&m_tagDsInfo[i].cBmpUnit,&m_memdcBkBmp,m_tagDsInfo[i].rectUnit,AfxGetResourceHandle(),dwUnitResID);
	}

	CTime time = CTime::GetCurrentTime();
	m_strTime.Format(_T("%02d:%02d"),time.GetHour(),time.GetMinute());
	m_strDate.Format(_T("%04d-%02d-%02d"),time.GetYear(),time.GetMonth(),time.GetDay());
	m_strWeek = theMainDlg->fGetBinText(0x00011000+time.GetDayOfWeek());

	m_strWaterTempText = theMainDlg->fGetBinText(0x00000500);
	m_strWaterTempText = m_strWaterTempText.Left(strText.Find(_T(" ")));
	m_strWaterTempValue  = _T("---");  

	m_clrTempText = RGB(0,0,0);
	m_clrTempValue = RGB(0,0,0);

	m_clrTempValueAlarm = RGB(255,0,0);

	this->Invalidate();
}

void COBDPNDDlgMainIdle::fRefreshData()
{
	CTime time = CTime::GetCurrentTime();
	m_strTime.Format(_T("%02d:%02d"),time.GetHour(),time.GetMinute());
	m_strDate.Format(_T("%04d-%02d-%02d"),time.GetYear(),time.GetMonth(),time.GetDay());
	m_strWeek = theMainDlg->fGetBinText(0x00011000+time.GetDayOfWeek());

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

				theMainDlg->m_pConfigReadWrite->fSetIdleUiData(i,0xFF010003);
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

				theMainDlg->m_pConfigReadWrite->fSetIdleUiData(i,0xFF010004);
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

				theMainDlg->m_pConfigReadWrite->fSetIdleUiData(i,0xFF010001);
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


				theMainDlg->m_pConfigReadWrite->fSetIdleUiData(i,0xFF010002);
			}
		}	
	}
	m_strWaterTempValue = theMainDlg->fGetDataStreamValue(0x00000500);
	if (m_strWaterTempValue == _T("------"))
		m_strWaterTempValue = _T("---");

	this->Invalidate();
}

void COBDPNDDlgMainIdle::OnLButtonDown(UINT nFlags, CPoint point)
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

void COBDPNDDlgMainIdle::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	KillTimer(1);

	WORD dwIndex = fGetCurrFootIndex(point);
	theMainDlg->fSetCurrPanelIndex(dwIndex);

	m_iKeyDownIndex = 0;
	m_iKeyDownTime =  0;

	CDialog::OnLButtonUp(nFlags, point);
}

BOOL COBDPNDDlgMainIdle::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;

	return CDialog::OnEraseBkgnd(pDC);
}

void COBDPNDDlgMainIdle::OnPaint()
{
	CPaintDC dc(this); // device context for painting

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

		fDrawFill(&memDC,m_rectWater,(_ttoi(m_strWaterTempValue)/180.0)*100);

		memDC.SetTextColor(theMainDlg->m_clrTitle);

		CFont *pOldFont = memDC.SelectObject(&m_fontDate);
		memDC.DrawText(m_strDate,-1,m_rectDate,DT_VCENTER|DT_CENTER);

		pOldFont = memDC.SelectObject(&m_fontWeek);
		memDC.DrawText(m_strWeek,-1,m_rectWeek,DT_VCENTER|DT_CENTER);

		pOldFont = memDC.SelectObject(&m_fontTime);
		memDC.DrawText(m_strTime,-1,m_rectTime,DT_VCENTER|DT_CENTER);

		float fCurrCoolant = theMainDlg->fGetDataStreamValueF(0x00000500);
		if (fCurrCoolant == INVALID_DATASTREAM_VALUE)
			fCurrCoolant = 0;
		DWORD dwAlarmCoolatn = theMainDlg->m_pConfigReadWrite->fGetWaterTemperatureAlarmValue();
		float fPer = fCurrCoolant *100/dwAlarmCoolatn; //报警百分比

		if (fPer>=100)
			memDC.SetTextColor(RGB(255,0,0));	//红色
		else if (fPer>=90)
			memDC.SetTextColor(RGB(244,78,6));	//黄色
		else if (fPer>=45)
			memDC.SetTextColor(RGB(0,0,0));		//黑色
		else
			memDC.SetTextColor(RGB(19,162,10));	//绿色

		pOldFont = memDC.SelectObject(&m_fontWaterTempValue);
		memDC.DrawText(m_strWaterTempValue,-1,m_rectWaterTempValue,DT_VCENTER|DT_CENTER);

		pOldFont = memDC.SelectObject(&m_fontWaterTempText);
		memDC.SetTextColor(RGB(0,0,0));		//黑色
		memDC.DrawText(m_strWaterTempText,-1,m_rectWaterTempText,DT_VCENTER|DT_CENTER);

		memDC.SetTextColor(theMainDlg->m_clrTitle);

		for (int i=0;i<6;i++)
		{
			//没有单位居中显示
			if (theMainDlg->fGetUnitResID(m_tagDsInfo[i].strUnit) == NULL)
			{
				pOldFont = memDC.SelectObject(&m_fontDsDataValue);
				memDC.DrawText(m_tagDsInfo[i].strValue,-1,m_tagDsInfo[i].rectData,DT_BOTTOM|DT_CENTER);
				pOldFont = memDC.SelectObject(&m_fontDsDataText);
				memDC.DrawText(m_tagDsInfo[i].strText,-1,m_tagDsInfo[i].rectText,DT_VCENTER|DT_CENTER);
			}
			else
			{
				memDC.BitBlt(m_tagDsInfo[i].rectUnit.left,m_tagDsInfo[i].rectUnit.top,m_tagDsInfo[i].rectUnit.Width(),m_tagDsInfo[i].rectUnit.Height(),	
					&m_tagDsInfo[i].memdcUnit,0,0,SRCCOPY);

				CRect rectTemp = m_tagDsInfo[i].rectData;
				rectTemp.right =m_tagDsInfo[i].rectUnit.left;
				pOldFont = memDC.SelectObject(&m_fontDsDataValue);
				memDC.DrawText(m_tagDsInfo[i].strValue,-1,rectTemp,DT_BOTTOM|DT_RIGHT);
				pOldFont = memDC.SelectObject(&m_fontDsDataText);
				memDC.DrawText(m_tagDsInfo[i].strText,-1,m_tagDsInfo[i].rectText,DT_VCENTER|DT_CENTER);
			}
		}

		pOldFont = memDC.SelectObject(&theMainDlg->m_fontFootNormal);
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

		if(pOldFont)  
			pDC.SelectObject(pOldFont);
		DeleteObject(pOldFont);

		if(pOldMemBmp)   
			memDC.SelectObject(pOldMemBmp); 
		DeleteObject(pOldMemBmp);

		DeleteDC(memDC);

	}
}

int COBDPNDDlgMainIdle::fDrawFill(CDC * dc,CRect rectCircle,int iPercent)
{
	if (dc == NULL)
		return 0;
	if (iPercent>100)
		iPercent = 100;

	COLORREF clrArc = RGB(0,97,233);
	CPoint pointCircle = CPoint(rectCircle.left + rectCircle.Width()/2,rectCircle.top + rectCircle.Height()/2);

	vector<CPoint> vecPointArc;

	int iRadius = rectCircle.Height()/2;		//半径
	double dStartAngle=90*M_PI/180.0;			//起始角度
	double dEndAngle=180*M_PI/180.0;			//结束角度
	double dRadPerDeg =1.0/(iRadius*M_PI*2.0);	//区分
	int xi = 0,yi = 0;
	int xL = 0,yL = 0;									
	double xd = 0,yd = 0;

	//下半个圆左边弧上的点
	for (double d=dStartAngle;d<dEndAngle;d+=dRadPerDeg) 
	{
		xd=iRadius*cos(d)+pointCircle.x;
		yd=iRadius*sin(d)+pointCircle.y;
		xi=(int)floor(xd);
		yi=(int)floor(yd);
		if (xi!=xL || yi!=yL) //避免重复数据
		{
			vecPointArc.push_back(CPoint((int)xi,(int)yi));

			xL=xi;
			yL=yi;
		}
	}
	//上半个圆左边弧上的点
	for (int i=vecPointArc.size()-1;i>=0;i--)
	{
		vecPointArc.push_back(CPoint(vecPointArc[i].x,pointCircle.y-(vecPointArc[i].y-pointCircle.y)));
	}


	//按比例画线
	CBrush brushFill;
	CBrush *pBrushOld ;
	CPen penDraw ;
	CPen *pPenOld;

	brushFill.DeleteObject();
	brushFill.CreateSolidBrush(clrArc);
	pBrushOld = dc->SelectObject(&brushFill);
	penDraw.DeleteObject();
	penDraw.CreatePen(PS_SOLID, 3, clrArc);
	pPenOld = dc->SelectObject(&penDraw);

	int iPoint = (int)(vecPointArc.size()*(iPercent/100.0)); 
	for (int i=0;i<iPoint;i++)
	{
		dc->MoveTo(vecPointArc[i]);
		dc->LineTo(pointCircle.x+(pointCircle.x-vecPointArc[i].x),vecPointArc[i].y); 

	}

	dc->SelectObject(pBrushOld); 
	dc->SelectObject(pPenOld);
	DeleteObject(pBrushOld);
	DeleteObject(pPenOld);
	DeleteObject(penDraw);
	DeleteObject(pPenOld);


	return 0;
}

void COBDPNDDlgMainIdle::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值	
	switch (nIDEvent)
	{
	case 1:
		{
		}
		break;
	}

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

