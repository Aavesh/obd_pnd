// OBDPNDDlgMainDetail.cpp : 实现文件
//

#include "stdafx.h"
#include "OBD_PND.h"
#include "OBDPNDDlgMainDetail.h"
#include "OBD_PNDDlg.h"


// COBDPNDDlgMainDetail 对话框

IMPLEMENT_DYNAMIC(COBDPNDDlgMainDetail, CDialog)

COBDPNDDlgMainDetail::COBDPNDDlgMainDetail(CWnd* pParent /*=NULL*/)
	: CDialog(COBDPNDDlgMainDetail::IDD, pParent)
{

}

COBDPNDDlgMainDetail::~COBDPNDDlgMainDetail()
{
}

void COBDPNDDlgMainDetail::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(COBDPNDDlgMainDetail, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// COBDPNDDlgMainDetail 消息处理程序

BOOL COBDPNDDlgMainDetail::OnInitDialog()
{
	CDialog::OnInitDialog();

	fInitUI();

	return TRUE;
}

void COBDPNDDlgMainDetail::fInitUI()
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
			fSetBkImg(this->GetDC(),IDR_PNG_MODE_DETAIL_800X480);
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
			VERIFY(m_fontValue.CreateFont(
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

			m_rectText[0] = CRect(18,17,400,99);
			m_rectText[1] = CRect(18,100,400,175);
			m_rectText[2] = CRect(18,175,400,252);
			m_rectText[3] = CRect(18,252,400,329);
			m_rectText[4] = CRect(18,329,400,406);
			m_rectText[5] = CRect(405,17,788,99);
			m_rectText[6] = CRect(405,100,788,175);
			m_rectText[7] = CRect(405,175,788,252);
			m_rectText[8] = CRect(405,252,788,329);
			m_rectText[9] = CRect(405,329,788,406);

			m_rectValue[0] = CRect(310,17,390,99);
			m_rectValue[1] = CRect(310,100,390,175);
			m_rectValue[2] = CRect(310,175,390,252);
			m_rectValue[3] = CRect(310,252,390,329);
			m_rectValue[4] = CRect(310,329,390,406);
			m_rectValue[5] = CRect(698,17,778,99);
			m_rectValue[6] = CRect(698,100,778,175);
			m_rectValue[7] = CRect(698,175,778,252);
			m_rectValue[8] = CRect(698,252,778,329);
			m_rectValue[9] = CRect(698,329,778,406);
		}

		break;
	}

	for (int i=0;i<10;i++)
	{
		m_strText[i] = theMainDlg->fGetBinText(DS_THE_TOTAL_MILEAGE + i);
		m_strValue[i] = _T("------");
	}

	this->Invalidate();
}

void COBDPNDDlgMainDetail::fRefreshData()
{
	m_strValue[0] = theMainDlg->fGetDataStreamValue(0xFF010008);
	m_strValue[1] = theMainDlg->fGetDataStreamValue(0xFF010006);    
	m_strValue[2] = theMainDlg->fGetDataStreamValue(0xFF01000E);    
	m_strValue[3] = theMainDlg->fGetDataStreamValue(0x00020001);  
	m_strValue[4] = theMainDlg->fGetDataStreamValue(0x00020002);  

	m_strValue[5] = theMainDlg->fGetDataStreamValue(0xFF01000A);    
	m_strValue[6] = theMainDlg->fGetDataStreamValue(0xFF010005);	  
	m_strValue[7] = theMainDlg->fGetDataStreamValue(0xFF01000F);      
	m_strValue[8] = theMainDlg->fGetDataStreamValue(0x00020003);  
	m_strValue[9] = theMainDlg->fGetDataStreamValue(0x00020004);  

	this->Invalidate();

//#define DS_THE_TOTAL_MILEAGE			   			0x0001100F		//本次行车里程(Km)                                                                                                                                                                                                                    
//#define DS_THE_AVG_FUEL_CONSUMPTION		   			0x00011010		//本次平均油耗(L/100Km)                                                                                                                                                                                                               
//#define DS_THE_FUEL_CONSUMPTION			   			0x00011011		//本次油耗量(升)                                                                                                                                                                                                                      
//#define DS_THE_FUEL_COSTS				   			0x00011012		//本次行车油费(元)                                                                                                                                                                                                                    
//#define DS_THE_COSTS_FOR_KM				   			0x00011013		//本次每公里花费(元)                                                                                                                                                                                                                  
//#define DS_TOTAL_MILEAGE				   			0x00011014		//总里程(Km)                                                                                                                                                                                                                          
//#define DS_AVG_FUEL_CONSUMPTION			   			0x00011015		//历史平均油耗(L/100Km)                                                                                                                                                                                                               
//#define DS_FUEL_CONSUMPTION				   			0x00011016		//历史油耗量(升)                                                                                                                                                                                                                      
//#define DS_FUEL_COSTS					   			0x00011017		//总行车油费(元)                                                                                                                                                                                                                      
//#define DS_COSTS_FOR_KM					   			0x00011018		//累计每公里花费(元)       
}

void COBDPNDDlgMainDetail::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDialog::OnLButtonDown(nFlags, point);
}

void COBDPNDDlgMainDetail::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	WORD dwIndex = fGetCurrFootIndex(point);
	theMainDlg->fSetCurrPanelIndex(dwIndex);

	CDialog::OnLButtonUp(nFlags, point);
}

BOOL COBDPNDDlgMainDetail::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;

	return CDialog::OnEraseBkgnd(pDC);
}

void COBDPNDDlgMainDetail::OnPaint()
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

		for (int i=0;i<10;i++)
		{
			memDC.SelectObject(m_fontText);
			memDC.DrawText(m_strText[i], -1, &m_rectText[i], DT_LEFT|DT_VCENTER);
			memDC.SelectObject(m_fontValue);
			memDC.DrawText(m_strValue[i], -1, &m_rectValue[i], DT_RIGHT|DT_VCENTER);
		}


		memDC.SelectObject(&theMainDlg->m_fontFootNormal);
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

void COBDPNDDlgMainDetail::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值	


	CDialog::OnTimer(nIDEvent);
}
