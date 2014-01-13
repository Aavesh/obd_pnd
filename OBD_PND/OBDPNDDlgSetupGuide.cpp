// OBDPNDDlgSetupGuide.cpp : 实现文件
//

#include "stdafx.h"
#include "OBD_PND.h"
#include "OBDPNDDlgSetupGuide.h"
#include "OBD_PNDDlg.h"
#include "OBDPNDDlgAlarmValue.h"
#include "CeFileFind.h"


// COBDPNDDlgSetupGuide 对话框

IMPLEMENT_DYNAMIC(COBDPNDDlgSetupGuide, CDialog)

COBDPNDDlgSetupGuide::COBDPNDDlgSetupGuide(CWnd* pParent /*=NULL*/)
	: CDialog(COBDPNDDlgSetupGuide::IDD, pParent)
{
	m_dwType = 0;
	m_iCurrSelectedIndex= -1;
	m_iCurrRowIndex= -1;					//当前行索引
	m_iPageCount= 1;
	m_iCurrPageIndex= 0;

	m_wFuelIndex = 0;

}

COBDPNDDlgSetupGuide::~COBDPNDDlgSetupGuide()
{
}

void COBDPNDDlgSetupGuide::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(COBDPNDDlgSetupGuide, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// COBDPNDDlgSetupGuide 消息处理程序

BOOL COBDPNDDlgSetupGuide::OnInitDialog()
{
	CDialog::OnInitDialog();

	fInitUI();

	return TRUE;
}

void COBDPNDDlgSetupGuide::fInitUI()
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
			fSetBkImg(this->GetDC(),IDR_PNG_DEVICE_SETUP_800X480);
		}
		
		break;
	}

	m_rectText = CRect(50,370,750,425);
	m_strText = theMainDlg->fGetBinText(DS_SETUP_DEVICES_TIP);

	m_clrText = RGB(255,255,255);

	fSetTitleText(theMainDlg->fGetBinText(DS_SETUP_GUIDE_TITLE));
	fSetFootText(MAX_FOOT_INDEX-1,theMainDlg->fGetBinText(DS_BTN_NEXT));	//返回


	this->Invalidate();
}
void COBDPNDDlgSetupGuide::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDialog::OnLButtonDown(nFlags, point);
}

void COBDPNDDlgSetupGuide::OnLButtonUp(UINT nFlags, CPoint point)
{
	//CString str;
	//str.Format(_T("%d,%d"),point.x,point.y);
	//AfxMessageBox(str);
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	for (int x=0;x<3;x++) 
	{
		for (int y=0;y<3;y++)
		{
			if (point.x> m_rectFuel[x][y].left && point.y> m_rectFuel[x][y].top && point.x< m_rectFuel[x][y].right && point.y <  m_rectFuel[x][y].bottom)
			{
				m_wFuelIndex = x*3+y;
				this->Invalidate();
				break;
			}
		}
	}

	m_iCurrSelectedIndex = -1;
	for (int i=1;i<5;i++)
	{
		if (point.x> m_rectGrid[i][0].left && point.y> m_rectGrid[i][0].top && point.x< m_rectGrid[i][2].right && point.y <  m_rectGrid[i][2].bottom)
		{
			m_iCurrSelectedIndex = i-1;
			this->Invalidate();
			break; 
		}
	}
	if (m_iCurrSelectedIndex>=0)
	{
		switch (m_dwType)
		{
		case SETUP_GUIDE_DSA:
		case SETUP_GUIDE_GPS:
			{
				for (int i=0;i<m_vecList.size();i++)
				{
					if (i == m_iCurrSelectedIndex)
						m_vecList[i].bAutoRun = !m_vecList[i].bAutoRun;
					else
						m_vecList[i].bAutoRun = FALSE;
				}
			}
			break;
		case SETUP_GUIDE_AUTOSTART:
			{
				for (int i=0;i<m_vecStartList.size();i++)
				{
					if (i == m_iCurrSelectedIndex)
						m_vecStartList[i].bAutoRun = !m_vecStartList[i].bAutoRun;
				}

			}	
			break;
		}
	}

	switch (fGetCurrFootIndex(point))
	{
	case 0:
		{
			switch (m_dwType)
			{
			case SETUP_GUIDE_DEVICE_SET:
			case SETUP_GUIDE_TIP_INIT:
			case SETUP_GUIDE_TIP_START_CAR:
				break;
			case SETUP_GUIDE_FUEL_PRICE:
				{
					COBDPNDDlgAlarmValue dlgFuelPrice;
					dlgFuelPrice.fSetAlarmType(CONFIT_FUEL_PRICE);
					dlgFuelPrice.fSetDecLen(2);
					if (dlgFuelPrice.DoModal() == IDOK)
					{
						theMainDlg->m_pConfigReadWrite->fSetFuelPrice(dlgFuelPrice.fGetValue2()/1000.0);
						m_strEditValue.Format(_T("%0.2f"),dlgFuelPrice.fGetValue2()/1000.0);
					} 
				}
				break;
			case SETUP_GUIDE_SET_ENGINE:	//高温
				{
					theMainDlg->m_pConfigReadWrite->fSetWaterTemperatureAlarmValue(110);

					m_strText = theMainDlg->fGetBinText(DS_SETUP_GUIDE_TIP_FULE_COEFFICIENT);
					m_rectText = CRect(50,70,700,220);

					for (int x=0;x<3;x++)
					{
						for (int y=0;y<3;y++)
						{
							m_rectFuel[x][y] = CRect(50+(x*235),225+(y*60),40+((x+1)*235) ,215+((y+1)*60) );

						}
					}

					fSetFootText(0,_T(""));	//
					fSetFootText(1,_T(""));	//

					fSetFootText(MAX_FOOT_INDEX-1,theMainDlg->fGetBinText(DS_BTN_NEXT));

					m_dwType = SETUP_GUIDE_FUEL_CONSUMPTION;
				}
				break;
			}

		}
		break;
	case 1:
		{
			switch (m_dwType)
			{
			case SETUP_GUIDE_DEVICE_SET:
			case SETUP_GUIDE_TIP_INIT:
			case SETUP_GUIDE_TIP_START_CAR:
			case SETUP_GUIDE_FUEL_PRICE:
				break;
			case SETUP_GUIDE_SET_ENGINE:	//普通
				{
					theMainDlg->m_pConfigReadWrite->fSetWaterTemperatureAlarmValue(100);

					m_strText = theMainDlg->fGetBinText(DS_SETUP_GUIDE_TIP_FULE_COEFFICIENT);
					m_rectText = CRect(50,70,700,220);

					for (int x=0;x<3;x++)
					{
						for (int y=0;y<3;y++)
						{
							m_rectFuel[x][y] = CRect(50+(x*235),225+(y*60),40+((x+1)*235) ,215+((y+1)*60) );

						}
					}

					fSetFootText(0,_T(""));	//
					fSetFootText(1,_T(""));	//

					fSetFootText(MAX_FOOT_INDEX-1,theMainDlg->fGetBinText(DS_BTN_NEXT));

					m_dwType = SETUP_GUIDE_FUEL_CONSUMPTION;
				}
				break;
			}

		}
		break;
	case MAX_FOOT_INDEX-1:
		{
			switch (m_dwType)
			{
			case SETUP_GUIDE_DEVICE_SET:
				{
					fSetBkImg(this->GetDC(),IDR_PNG_BASE_800X480);
					m_rectText = CRect(50,70,750,400);
					VERIFY(m_fontText.CreateFont(
						32,                        // nHeight
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

					m_strText = theMainDlg->fGetBinText(DS_SETUP_GUIDE_TIP_INIT);
					m_dwType = SETUP_GUIDE_TIP_INIT;
				}
				break;
			case SETUP_GUIDE_TIP_INIT:
				{
					//m_strText = theMainDlg->fGetBinText(DS_SETUP_GUIDE_TIP_START_CAR);
					//m_dwType = SETUP_GUIDE_TIP_START_CAR;

					// 读取数据，检查是否可用


					VERIFY(m_fontEditValue.CreateFont(
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

					m_strEditText = theMainDlg->fGetBinText(DS_CONFIG_FUEL_PRICE_TEXT2);
					m_strEditValue.Format(_T("%0.2f"),theMainDlg->m_pConfigReadWrite->fGetFuelPrice());
					m_rectEditText = CRect(50,100,750,155);
					m_rectEditValue = CRect(570,100,570+172,100+51);
					fCommDrawImage(&m_memdcEditValue,&m_cBitmapEditValue,&m_memdcBkBmp,m_rectEditValue,AfxGetInstanceHandle(),IDR_PNG_WIDGIT_EDITTEXT_800X480);

					m_rectText = CRect(50,120,750,400);
					m_strText = theMainDlg->fGetBinText(DS_SETUP_GUIDE_TIP_SET_FUEL_PRICE);
					m_dwType = SETUP_GUIDE_FUEL_PRICE;

					fSetFootText(0,theMainDlg->fGetBinText(DS_BTN_MODIFY));	//返回
				}
				break;
			case SETUP_GUIDE_TIP_START_CAR:
				{
				}
				break;
			case SETUP_GUIDE_FUEL_PRICE:
				{
					m_rectText = CRect(50,70,750,400);
					m_strText =theMainDlg->fGetBinText(DS_SETUP_GUIDE_TIP_SET_ENGINE) + _T("\r\n")  + _T("\r\n") +
						theMainDlg->fGetBinText(DS_SETUP_GUIDE_ENGINE_TEXT1) + _T("\r\n") +
						theMainDlg->fGetBinText(DS_SETUP_GUIDE_ENGINE_TEXT2);

					fSetFootText(0,theMainDlg->fGetBinText(DS_BTN_HIGH));	//返回
					fSetFootText(1,theMainDlg->fGetBinText(DS_BTN_NORMAL));	//返回

					fSetFootText(MAX_FOOT_INDEX-1,theMainDlg->fGetBinText(DS_BTN_UNCLEAR));

					m_dwType = SETUP_GUIDE_SET_ENGINE; 
				}
				break;
			case SETUP_GUIDE_SET_ENGINE:
				{
					// 不清楚发动机类型，默认为普通发动机
					theMainDlg->m_pConfigReadWrite->fSetWaterTemperatureAlarmValue(100);

					m_strText = theMainDlg->fGetBinText(DS_SETUP_GUIDE_TIP_FULE_COEFFICIENT);
					m_rectText = CRect(50,70,750,220);
					for (int x=0;x<3;x++)
					{
						for (int y=0;y<3;y++)
						{
							m_rectFuel[x][y] = CRect(50+(x*235),225+(y*60),40+((x+1)*235) ,215+((y+1)*60) );

						}
					}

					fSetFootText(0,_T(""));	//
					fSetFootText(1,_T(""));	//

					fSetFootText(MAX_FOOT_INDEX-1,theMainDlg->fGetBinText(DS_BTN_NEXT));

					m_dwType = SETUP_GUIDE_FUEL_CONSUMPTION;
				}
				break;			
			case SETUP_GUIDE_FUEL_CONSUMPTION:
				{
					switch (m_wFuelIndex)
					{
					case 0:
						theMainDlg->m_pConfigReadWrite->fSetNowFCC(167.0);
						break;
					case 1:
						theMainDlg->m_pConfigReadWrite->fSetNowFCC(208.0);
						break;
					case 2:
						theMainDlg->m_pConfigReadWrite->fSetNowFCC(250.0);
						break;
					case 3:
						theMainDlg->m_pConfigReadWrite->fSetNowFCC(331.0);
						break;
					case 4:
						theMainDlg->m_pConfigReadWrite->fSetNowFCC(374.0);
						break;
					case 5:
						theMainDlg->m_pConfigReadWrite->fSetNowFCC(416.0);
						break;
					case 6:
						theMainDlg->m_pConfigReadWrite->fSetNowFCC(501.0);
						break;
					case 7:
						theMainDlg->m_pConfigReadWrite->fSetNowFCC(563.0);
						break;
					case 8:
						theMainDlg->m_pConfigReadWrite->fSetNowFCC(624.0);
						break;
					}

					m_vecStartList = theMainDlg->m_pConfigReadWrite->fGetStartList();

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

					m_clrInfoTitle = RGB(255,255,255);
					m_clrInfoText= RGB(255,255,255);

					fSetTitleText(theMainDlg->fGetBinText(DS_SETUP_GUIDE_DSA_TITLE));
					m_vecList = theMainDlg->m_pConfigReadWrite->fGetExistDSA();
					m_strGrid[0][0] = theMainDlg->fGetBinText(DS_SETUP_GUIDE_DSA_NAME);
					m_strGrid[0][1] = theMainDlg->fGetBinText(DS_SETUP_GUIDE_DSA_PATH);
					m_strGrid[0][2] = theMainDlg->fGetBinText(DS_SETUP_GUIDE_DSA_SEL);
					if (m_vecList.size() > 0)
					{
						m_iCurrSelectedIndex = 0;
						m_vecList[0].bAutoRun = TRUE;

						m_iCurrSelectedIndex= 0;
						m_iCurrRowIndex= 0;					//当前行索引
						m_iCurrPageIndex= 0;
						if (m_vecList.size() > 4)
						{
							fSetFootText(0,theMainDlg->fGetBinText(DS_BTN_PAGE_UP));	//上一页
							fSetFootText(1,theMainDlg->fGetBinText(DS_BTN_PAGE_DOWN));	//下一页
							m_iPageCount = m_vecList.size()/4+1;
						}
						else
						{
							fSetFootText(0,_T(""));	//上一页
							fSetFootText(1,_T(""));	//下一页
							m_iPageCount = 1;
						}
						for (int i=1;i<5;i++)
						{
							m_strGrid[i][0] = _T("");
							m_strGrid[i][1] = _T("");
						}

						for (int i=0;i<m_vecList.size();i++)
						{
							m_strGrid[i+1][0] = m_vecList[i].strAppName;
							m_strGrid[i+1][1] = m_vecList[i].strAppPath;
						}

						m_dwType = SETUP_GUIDE_DSA;
					}
					else
					{
						fSetTitleText(theMainDlg->fGetBinText(DS_SETUP_GUIDE_GPS_TITLE));

						m_vecList = theMainDlg->m_pConfigReadWrite->fGetExistGPS();
						m_strGrid[0][0] = theMainDlg->fGetBinText(DS_SETUP_GUIDE_DSA_NAME);
						m_strGrid[0][1] = theMainDlg->fGetBinText(DS_SETUP_GUIDE_DSA_PATH);
						m_strGrid[0][2] = theMainDlg->fGetBinText(DS_SETUP_GUIDE_DSA_SEL);
						if (m_vecList.size()>0)
						{
							m_iCurrSelectedIndex = 0;
							m_vecList[0].bAutoRun = TRUE;
							if (m_vecList.size() > 4)
							{
								fSetFootText(0,theMainDlg->fGetBinText(DS_BTN_PAGE_UP));	//上一页
								fSetFootText(1,theMainDlg->fGetBinText(DS_BTN_PAGE_DOWN));	//下一页
								m_iPageCount = m_vecList.size()/4+1;
							}
							else
							{
								fSetFootText(0,_T(""));	//上一页
								fSetFootText(1,_T(""));	//下一页
								m_iPageCount = 1;
							}
							for (int i=1;i<5;i++)
							{
								m_strGrid[i][0] = _T("");
								m_strGrid[i][1] = _T("");
							}

							for (int i=0;i<m_vecList.size();i++)
							{
								m_strGrid[i+1][0] = m_vecList[i].strAppName;
								m_strGrid[i+1][1] = m_vecList[i].strAppPath;
							}

							m_dwType = SETUP_GUIDE_GPS;
						}
						else
						{
							fSetBkImg(this->GetDC(),IDR_PNG_BASE_800X480);
							fSetTitleText(theMainDlg->fGetBinText(DS_SETUP_GUIDE_TITLE));

							m_rectText = CRect(100,70,700,400);
							m_strText = theMainDlg->fGetBinText(DS_SETUP_GUIDE_OK);

							fSetFootText(0,_T(""));	//
							fSetFootText(1,_T(""));	//

							fSetFootText(MAX_FOOT_INDEX-1,theMainDlg->fGetBinText(DS_BTN_OK));
							m_dwType = SETUP_GUIDE_OK;
						}
					}
				}
				break;
			case  SETUP_GUIDE_DSA:
				{
					for (int i=0;i<m_vecList.size();i++)
					{
						if (m_vecList[i].bAutoRun)
						{
							m_vecStartList[0].strAppPath = m_vecList[i].strAppPath;
							break;
						}
					}
					fSetTitleText(theMainDlg->fGetBinText(DS_SETUP_GUIDE_GPS_TITLE));
					m_strGrid[0][0] = theMainDlg->fGetBinText(DS_SETUP_GUIDE_DSA_NAME);
					m_strGrid[0][1] = theMainDlg->fGetBinText(DS_SETUP_GUIDE_DSA_PATH);
					m_strGrid[0][2] = theMainDlg->fGetBinText(DS_SETUP_GUIDE_DSA_SEL);

					m_vecList = theMainDlg->m_pConfigReadWrite->fGetExistGPS();
					if (m_vecList.size()>0)
					{
						m_iCurrSelectedIndex = 0;
						m_vecList[0].bAutoRun = TRUE;
						if (m_vecList.size() > 4)
						{ 
							fSetFootText(0,theMainDlg->fGetBinText(DS_BTN_PAGE_UP));	//上一页
							fSetFootText(1,theMainDlg->fGetBinText(DS_BTN_PAGE_DOWN));	//下一页
							m_iPageCount = m_vecList.size()/4+1;
						}
						else
						{
							fSetFootText(0,_T(""));	//上一页
							fSetFootText(1,_T(""));	//下一页
							m_iPageCount = 1;
						}
						for (int i=1;i<5;i++)
						{
							m_strGrid[i][0] = _T("");
							m_strGrid[i][1] = _T("");
						}

						for (int i=0;i<m_vecList.size();i++)
						{
							m_strGrid[i+1][0] = m_vecList[i].strAppName;
							m_strGrid[i+1][1] = m_vecList[i].strAppPath;
						}


						m_dwType = SETUP_GUIDE_GPS;
					}
					else
					{
						fSetBkImg(this->GetDC(),IDR_PNG_BASE_800X480);
						fSetTitleText(theMainDlg->fGetBinText(DS_SETUP_GUIDE_TITLE));

						m_rectText = CRect(100,70,700,400);
						m_strText = theMainDlg->fGetBinText(DS_SETUP_GUIDE_OK);

						fSetFootText(0,_T(""));	//
						fSetFootText(1,_T(""));	//

						fSetFootText(MAX_FOOT_INDEX-1,theMainDlg->fGetBinText(DS_BTN_OK));
						m_dwType = SETUP_GUIDE_OK;
					}
				}
				break;
			case SETUP_GUIDE_GPS:
				{
					for (int i=0;i<m_vecList.size();i++)
					{
						if (m_vecList[i].bAutoRun)
						{
							m_vecStartList[1].strAppPath = m_vecList[i].strAppPath;
							break;
						}
					}

					fSetTitleText(theMainDlg->fGetBinText(DS_SETUP_AUTO_START_TITLE));
					m_strGrid[0][0] = theMainDlg->fGetBinText(DS_SETUP_AUTO_START_NAME);
					m_strGrid[0][1] = theMainDlg->fGetBinText(DS_SETUP_AUTO_START_PATH);
					m_strGrid[0][2] = theMainDlg->fGetBinText(DS_SETUP_AUTO_START_SEL);
					if (m_vecStartList.size()>0)
					{
						m_rectGrid[0][2] = CRect(660,75,760,130);

						for (int i=1;i<5;i++)
						{
							m_strGrid[i][0] = _T("");
							m_strGrid[i][1] = _T("");
							m_rectGrid[i][2].left = 670;
							m_rectGrid[i][2].right = 750;
						}
						for (int i=0;i<m_vecStartList.size();i++)
						{
							m_vecStartList[i].bAutoRun = FALSE;
							m_strGrid[i+1][0] = m_vecStartList[i].strAppName;
							m_strGrid[i+1][1] = m_vecStartList[i].strAppPath;
						}

						fSetFootText(0,_T(""));	//上一页
						fSetFootText(1,_T(""));	//下一页

						m_dwType = SETUP_GUIDE_AUTOSTART;
					}
					else
					{
						fSetBkImg(this->GetDC(),IDR_PNG_BASE_800X480);
						fSetTitleText(theMainDlg->fGetBinText(DS_SETUP_GUIDE_TITLE));

						m_rectText = CRect(50,70,750,400);
						m_strText = theMainDlg->fGetBinText(DS_SETUP_GUIDE_OK);

						fSetFootText(0,_T(""));	//
						fSetFootText(1,_T(""));	//

						fSetFootText(MAX_FOOT_INDEX-1,theMainDlg->fGetBinText(DS_BTN_OK));
						m_dwType = SETUP_GUIDE_OK;
					}
				}
				break;
			case SETUP_GUIDE_AUTOSTART:
				{
					theMainDlg->m_pConfigReadWrite->fSetStartList(m_vecStartList);

					fSetBkImg(this->GetDC(),IDR_PNG_BASE_800X480);
					fSetTitleText(theMainDlg->fGetBinText(DS_SETUP_GUIDE_TITLE));

					m_rectText = CRect(50,70,750,400);
					m_strText = theMainDlg->fGetBinText(DS_SETUP_GUIDE_OK);

					fSetFootText(0,_T(""));	//
					fSetFootText(1,_T(""));	//

					fSetFootText(MAX_FOOT_INDEX-1,theMainDlg->fGetBinText(DS_BTN_OK));

					m_dwType = SETUP_GUIDE_OK;

				}
				break;
			case SETUP_GUIDE_OK:
				{
					theMainDlg->m_pConfigReadWrite->fSaveAppConfig();
					CDialog::OnOK();
				}
				break;
			}
		}
		break;
	}
	this->Invalidate();

	CDialog::OnLButtonUp(nFlags, point);
}

BOOL COBDPNDDlgSetupGuide::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;

	return CDialog::OnEraseBkgnd(pDC);
}

void COBDPNDDlgSetupGuide::OnPaint()
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

		switch (m_dwType)
		{
		case SETUP_GUIDE_DEVICE_SET:
			{
				memDC.SelectObject(&m_fontText); 
				memDC.SetTextColor(m_clrText);   

				CRect temp = m_rectText;
				int height = memDC.DrawText(m_strText,temp,DT_CENTER| DT_TOP| DT_WORDBREAK | DT_CALCRECT | DT_EDITCONTROL); // 获得文本高度
				m_rectText.DeflateRect(0,(m_rectText.Height() - height) / 2); // 改变rect
				memDC.DrawText(m_strText,m_rectText, DT_CENTER| DT_TOP | DT_EDITCONTROL | DT_WORDBREAK);
			}
			break;
		case SETUP_GUIDE_TIP_INIT:
		case SETUP_GUIDE_TIP_START_CAR:
			{
				memDC.SelectObject(&m_fontText); 
				memDC.SetTextColor(m_clrText);   

				CRect temp = m_rectText;
				int height = memDC.DrawText(m_strText,temp,DT_LEFT| DT_TOP| DT_WORDBREAK | DT_CALCRECT | DT_EDITCONTROL); // 获得文本高度
				m_rectText.DeflateRect(0,(m_rectText.Height() - height) / 2); // 改变rect
				memDC.DrawText(m_strText,m_rectText, DT_LEFT| DT_TOP | DT_EDITCONTROL | DT_WORDBREAK);

			}
			break;
		case SETUP_GUIDE_FUEL_PRICE:
			{
				memDC.BitBlt(m_rectEditValue.left,m_rectEditValue.top,m_rectEditValue.Width(),m_rectEditValue.Height(),	&m_memdcEditValue,0,0,SRCCOPY);

				memDC.SelectObject(&m_fontEditValue);  
				memDC.DrawText(m_strEditText,m_rectEditText, DT_LEFT| DT_VCENTER);
				memDC.SetTextColor(theMainDlg->m_clrFootFocus);  
				memDC.DrawText(m_strEditValue,m_rectEditValue, DT_CENTER| DT_VCENTER);

				memDC.SetTextColor(m_clrText);   
				memDC.SelectObject(&m_fontText); 
				CRect temp = m_rectText;
				int height = memDC.DrawText(m_strText,temp,DT_LEFT| DT_TOP| DT_WORDBREAK | DT_CALCRECT | DT_EDITCONTROL); // 获得文本高度
				m_rectText.DeflateRect(0,(m_rectText.Height() - height) / 2); // 改变rect
				memDC.DrawText(m_strText,m_rectText, DT_LEFT| DT_TOP | DT_EDITCONTROL | DT_WORDBREAK);

			}
			break;
		case SETUP_GUIDE_SET_ENGINE:
			{
				memDC.SetTextColor(m_clrText);   
				memDC.SelectObject(&m_fontText); 
				CRect temp = m_rectText;
				int height = memDC.DrawText(m_strText,temp,DT_LEFT| DT_TOP| DT_WORDBREAK | DT_CALCRECT | DT_EDITCONTROL); // 获得文本高度
				m_rectText.DeflateRect(0,(m_rectText.Height() - height) / 2); // 改变rect
				memDC.DrawText(m_strText,m_rectText, DT_LEFT| DT_TOP | DT_EDITCONTROL | DT_WORDBREAK);

			}
			break;
		case SETUP_GUIDE_FUEL_CONSUMPTION:
			{
				memDC.SetTextColor(m_clrText);   
				memDC.SelectObject(&m_fontText); 
				CRect temp = m_rectText;
				int height = memDC.DrawText(m_strText,temp,DT_LEFT| DT_TOP| DT_WORDBREAK | DT_CALCRECT | DT_EDITCONTROL); // 获得文本高度
				m_rectText.DeflateRect(0,(m_rectText.Height() - height) / 2); // 改变rect
				memDC.DrawText(m_strText,m_rectText, DT_LEFT| DT_TOP | DT_EDITCONTROL | DT_WORDBREAK);

				CString strText;
				for (int x=0;x<3;x++)
				{
					for (int y=0;y<3;y++)
					{
						CBrush *pBrush = NULL;
						if ((x*3+y) == m_wFuelIndex)
						{
							pBrush= new CBrush(theMainDlg->m_clrFootFocus);
						}
						else
							pBrush=CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
						CBrush *pOldBrush=memDC.SelectObject(pBrush);//选择画刷

						CPen pen(PS_SOLID,1,theMainDlg->m_clrFootFocus);
						memDC.SelectObject(pen);
						pen.DeleteObject();		
						memDC.Rectangle(m_rectFuel[x][y]);

						memDC.FillRect(m_rectFuel[x][y],pBrush);//用指定的画刷去填充矩形区域
						memDC.SelectObject(pOldBrush);

						strText =theMainDlg->fGetBinText(DS_SETUP_GUIDE_FULE_COEFFICIENT_TEXT1 + (x*3+y));
						memDC.DrawText(strText,m_rectFuel[x][y], DT_CENTER |DT_VCENTER);

					}
				}
			}
			break;
		case SETUP_GUIDE_DSA:
		case SETUP_GUIDE_GPS:
		case SETUP_GUIDE_AUTOSTART:
			{
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
					memDC.DrawText(m_strGrid[i][1], -1, &m_rectGrid[i][1], DT_LEFT|DT_VCENTER);

					if (m_dwType == SETUP_GUIDE_AUTOSTART)
					{
						if ((i-1)<m_vecStartList.size() && m_vecStartList[i-1].bAutoRun)
						{				
							memDC.SelectObject(m_fontInfoCheck);
							memDC.DrawText(_T("√"), -1, &CRect(m_rectGrid[i][2].left,m_rectGrid[i][2].top-5,m_rectGrid[i][2].right,m_rectGrid[i][2].bottom+ 5),DT_CENTER|DT_VCENTER);
						}
					}
					else
					{
						if ((i-1)<m_vecList.size() && m_vecList[i-1].bAutoRun)
						{				
							memDC.SelectObject(m_fontInfoCheck);
							memDC.DrawText(_T("√"), -1, &CRect(m_rectGrid[i][2].left,m_rectGrid[i][2].top-5,m_rectGrid[i][2].right,m_rectGrid[i][2].bottom+ 5),DT_CENTER|DT_VCENTER);
						}

					}

				}	

			}
			break;
		case SETUP_GUIDE_OK:
			{
				memDC.SelectObject(&m_fontText); 
				memDC.SetTextColor(m_clrText);   

				CRect temp = m_rectText;
				int height = memDC.DrawText(m_strText,temp,DT_LEFT| DT_TOP| DT_WORDBREAK | DT_CALCRECT | DT_EDITCONTROL); // 获得文本高度
				m_rectText.DeflateRect(0,(m_rectText.Height() - height) / 2); // 改变rect
				memDC.DrawText(m_strText,m_rectText, DT_LEFT| DT_TOP | DT_EDITCONTROL | DT_WORDBREAK);
			}
			break;
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

void COBDPNDDlgSetupGuide::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值	

	CDialog::OnTimer(nIDEvent);
}