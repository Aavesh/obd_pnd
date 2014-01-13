// OBDPNDDlgConfigMain.cpp : 实现文件
//

#include "stdafx.h"
#include "OBD_PND.h"
#include "OBDPNDDlgConfigMain.h"
#include "OBD_PNDDlg.h"

#include "OBDPNDDlgVersion.h"
#include "OBDPNDDlgLanguage.h"
#include "OBDPNDDlgFactorySettings.h"
#include "OBDPNDDlgErrLog.h"

#include "OBDPNDDlgStartSetting.h"
#include "OBDPNDDlgAlarmTrouble.h"
#include "OBDPNDDlgAlarmValue.h"

#include "OBDPNDDlgBacklight.h"
// COBDPNDDlgConfigMain 对话框

IMPLEMENT_DYNAMIC(COBDPNDDlgConfigMain, CDialog)

COBDPNDDlgConfigMain::COBDPNDDlgConfigMain(CWnd* pParent /*=NULL*/)
	: CDialog(COBDPNDDlgConfigMain::IDD, pParent)
{
	m_dwPageCount = 1;
	m_dwCurrPageIndex = 0;

	m_iCurrFunIndex = -1;

	m_iCheckIndex = 0;

}

COBDPNDDlgConfigMain::~COBDPNDDlgConfigMain()
{
}

void COBDPNDDlgConfigMain::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(COBDPNDDlgConfigMain, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// COBDPNDDlgConfigMain 消息处理程序

BOOL COBDPNDDlgConfigMain::OnInitDialog()
{
	CDialog::OnInitDialog();

	fInitUI();

	return TRUE;
}

void COBDPNDDlgConfigMain::fSetPage(int iPageIndex)
{
	DWORD dwTitleID= 0;
	//
	for (int i=0;i<10;i++)
	{
		m_strText[i] = _T("");
		m_dwTextImgID[i] = NULL;
	}

	switch (iPageIndex)
	{
	case 0:
		{
			dwTitleID = DS_CONFIG_DEVICES_TITLE;

			fSetTextImg(0,DS_CONFIG_ALARM_OVERSPEED,IDR_PNG_CONFIG_ALARM_OVERSPEED_800X480);
			fSetTextImg(1,DS_CONFIG_ALARM_WATER_TEMP,IDR_PNG_CONFIG_ALARM_WATER_TEMP_800X480);
			fSetTextImg(2,DS_CONFIG_ALARM_FATIGUE_DRIVING,IDR_PNG_CONFIG_ALARM_FATIGUE_DRIVING_800X480);
			fSetTextImg(3,DS_CONFIG_ALARM_BATTERY_VOLTAGE,IDR_PNG_CONFIG_ALARM_BATTERY_VOLTAGE_800X480);
			fSetTextImg(4,DS_CONFIG_ALARM_TROUBLE,IDR_PNG_CONFIG_ALARM_TROUBLE_800X480);

			fSetTextImg(5,DS_CONFIG_START_SET,IDR_PNG_CONFIG_DEVICES_PROTOCOL_800X480);
			fSetTextImg(6,DS_CONFIG_FUEL_CALIBRATION,IDR_PNG_CONFIG_FUEL_CALIBRATION_800X480);
			fSetTextImg(7,DS_CONFIG_FUEL_PRICE,IDR_PNG_CONFIG_FUEL_PRICE_800X480);
			fSetTextImg(8,DS_CONFIG_DEVICES_FACTORY_SET,IDR_PNG_CONFIG_DEVICES_FACTORY_SET_800X480);
			fSetTextImg(9,DS_CONFIG_DEVICES_VERSION,IDR_PNG_CONFIG_DEVICES_VERSION_800X480);

		}
		break;
	}
	fSetTitleText(theMainDlg->fGetBinText(dwTitleID));	//

	this->Invalidate();
}

// 设置该页的图标
void COBDPNDDlgConfigMain::fSetTextImg(DWORD dwIndex,DWORD dwTextResID,DWORD dwImgResID)
{
	if (dwTextResID == 0 || dwImgResID == 0)
		return;

	m_strText[dwIndex] = theMainDlg->fGetBinText(dwTextResID);
	m_dwTextImgID[dwIndex] = dwImgResID;


	//如果未载入图标，则载入图标
	BOOL bIsExist = FALSE;
	for (int i=0;i<40;i++)
	{
		if (m_tagImg[i].dwResID == dwImgResID)
		{
			bIsExist = TRUE;
			break;
		}
	}
	if (!bIsExist)
	{
		for (int i=0;i<40;i++)
		{
			if (m_tagImg[i].dwResID == NULL)
			{
				m_tagImg[i].dwResID = dwImgResID;

				fCommDrawImage(&m_tagImg[i].memdcUnit,&m_tagImg[i].cBmpUnit,&m_memdcBkBmp,m_rectImg[dwIndex],AfxGetResourceHandle(),dwImgResID);

				break;
			}
		}
	}
}


void COBDPNDDlgConfigMain::fInitUI()
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

			m_rectCheck[0] = CRect(0,0,50,50);
			m_rectCheck[1] = CRect(750,0,800,50);
			m_rectCheck[2] = CRect(0,435-50,50,435);
			m_rectCheck[3] = CRect(750,435-50,800,435);

			VERIFY(m_fontText.CreateFont(
				25,                        // nHeight
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


			m_rectImg[0] = CRect(40,105,40+126,105+122);
			m_rectImg[1] = CRect(185,105,185+126,105+122);
			m_rectImg[2] = CRect(330,105,330+126,105+122);
			m_rectImg[3] = CRect(475,105,475+126,105+122);
			m_rectImg[4] = CRect(620,105,620+126,105+122);

			m_rectImg[5] = CRect(40,250,40+126,250+122);
			m_rectImg[6] = CRect(185,250,185+126,250+122);
			m_rectImg[7] = CRect(330,250,330+126,250+122);
			m_rectImg[8] = CRect(475,250,475+126,250+122);
			m_rectImg[9] = CRect(620,250,620+126,250+122);

			
			for (int i=0;i<10;i++)
			{
				m_rectText[i] = m_rectImg[i];
				m_rectText[i].bottom = m_rectText[i].bottom - 10;
			}

		}

		break;
	}

	fSetPage(0);

	//fSetFootText(0,theMainDlg->fGetBinText(DS_BTN_PAGE_UP));					//上一页
	//fSetFootText(1,theMainDlg->fGetBinText(DS_BTN_PAGE_DOWN));					//下一页
	//fSetFootText(0,theMainDlg->fGetBinText(DS_SETUP_BACKLIGHT_TITLE));	//返回
	fSetFootText(MAX_FOOT_INDEX-1,theMainDlg->fGetBinText(DS_BTN_RETURN));	//返回

	this->Invalidate();
}

void COBDPNDDlgConfigMain::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDialog::OnLButtonDown(nFlags, point);
}

void COBDPNDDlgConfigMain::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	switch (fGetCurrFootIndex(point))
	{
	case MAX_FOOT_INDEX-1:
		CDialog::OnCancel();
		break;
	case  0:
		{
			//COBDPNDDlgBacklight dlgBacklight;
			//dlgBacklight.DoModal();
			m_dwCurrPageIndex--;
			if (m_dwCurrPageIndex<0)
				m_dwCurrPageIndex = m_dwPageCount-1;

			fSetPage(m_dwCurrPageIndex);
		}
		break;
	case 1:
		{
			m_dwCurrPageIndex++;
			if (m_dwCurrPageIndex>(m_dwPageCount-1))
				m_dwCurrPageIndex = 0;

			fSetPage(m_dwCurrPageIndex);

		}
		break;
	case 2:
		{
		}
		break;
	}

	m_iCurrFunIndex = -1;
	for (int i=0;i<10;i++)
	{
		if(point.x>m_rectImg[i].left && point.x<m_rectImg[i].right && point.y>m_rectImg[i].top && point.y<m_rectImg[i].bottom)
		{
			m_iCurrFunIndex = i;
			break;
		}
	}
	this->Invalidate();

	if(point.x>m_rectCheck[m_iCheckIndex].left && point.x<m_rectCheck[m_iCheckIndex].right && 
		point.y>m_rectCheck[m_iCheckIndex].top && point.y<m_rectCheck[m_iCheckIndex].bottom)
	{
		m_iCheckIndex++;
		if (m_iCheckIndex == 4)
		{
			theMainDlg->m_pConfigReadWrite->fSetSaveErrLogStatus(TRUE);
			theMainDlg->fSetCurrPanelIndex(OBDPND_PANEL_LOG);		
			CDialog::OnCancel();	
		}
	}
	else
		m_iCheckIndex = 0;




	////语言选择
	//COBDPNDDlgLanguage dlgLanguage;
	//if(dlgLanguage.DoModal() == IDOK)
	//{
	//	AfxMessageBox(dlgLanguage.fGetSelectedLanID());
	//}
	//
	////错误日志
	//COBDPNDDlgErrLog dlgErrLog;
	//if (dlgErrLog.DoModal() == IDOK)
	//{
	//	if (dlgErrLog.fIsOpenErrLog())
	//	{
	//		theMainDlg->m_pConfigReadWrite->fSetSaveErrLogStatus(TRUE);
	//	}
	//	else
	//	{
	//		theMainDlg->m_pConfigReadWrite->fSetSaveErrLogStatus(FALSE);
	//	}

	//}
	//
	////退出
	//theMainDlg->fAppExit();

	switch (m_dwCurrPageIndex)
	{
	case 0:	//设备设置
		{	
			switch (m_iCurrFunIndex)
			{
			case 0:	//超速报警
				{
					COBDPNDDlgAlarmValue dlgAlarmOverSpeed;
					dlgAlarmOverSpeed.fSetAlarmType(ALARM_VALUE_OVERSPEED);
					if (dlgAlarmOverSpeed.DoModal() == IDOK)
					{
						theMainDlg->m_pConfigReadWrite->fSetOverSpeedAlarmValue(dlgAlarmOverSpeed.fGetValue2());
					}
				}
				break;
			case 1:	//水温报警
				{
					COBDPNDDlgAlarmValue dlgAlarmCoolant;
					dlgAlarmCoolant.fSetAlarmType(ALARM_VALUE_COOLANT);
					if (dlgAlarmCoolant.DoModal() == IDOK)
					{
						theMainDlg->m_pConfigReadWrite->fSetWaterTemperatureAlarmValue(dlgAlarmCoolant.fGetValue2());
					}
				}
				break;
			case 2: //疲劳驾驶报警
				{
					COBDPNDDlgAlarmValue dlgAlarmFatigueDriving;
					dlgAlarmFatigueDriving.fSetAlarmType(ALARM_VALUE_FATIGUE_DRIVING);
					if (dlgAlarmFatigueDriving.DoModal() == IDOK)
					{
						theMainDlg->m_pConfigReadWrite->fSetFatigueDrivingAlarmValue(dlgAlarmFatigueDriving.fGetValue2());
					}
				}
				break;
			case 3:	//电瓶电压报警
				{
					COBDPNDDlgAlarmValue dlgAlarmBatteryVoltage;
					dlgAlarmBatteryVoltage.fSetAlarmType(ALARM_VALUE_BATTERY_VOLTAGE);
					dlgAlarmBatteryVoltage.fSetDecLen(1);
					if (dlgAlarmBatteryVoltage.DoModal() == IDOK)
					{
						theMainDlg->m_pConfigReadWrite->fSetVoltMinAlarm(dlgAlarmBatteryVoltage.fGetValue1());
						theMainDlg->m_pConfigReadWrite->fSetVoltMaxAlarm(dlgAlarmBatteryVoltage.fGetValue2());
					}
				}
				break;
			case 4://故障报警
				{
					COBDPNDDlgAlarmTrouble dlgAlarmTrouble;
					if (dlgAlarmTrouble.DoModal() == IDOK)
					{
						if (dlgAlarmTrouble.fIsOpenAlarmTourble())
						{
							theMainDlg->m_pConfigReadWrite->fSetTroubleCodeAlarmStatus(TRUE);
						}
						else
						{
							theMainDlg->m_pConfigReadWrite->fSetTroubleCodeAlarmStatus(FALSE);
						}
					}
				}
				break;
			case 5: //
				{
					COBDPNDDlgStartSetting dlgStart;
					if (dlgStart.DoModal() == IDOK)
					{
						theMainDlg->m_pConfigReadWrite->fSetStartList(dlgStart.fGetStartList());
					}
				}
				break;
			case 6:	//油耗校准
				{
					COBDPNDDlgAlarmValue dlgConfigFuelCalibration;
					dlgConfigFuelCalibration.fSetAlarmType(CONFIG_FUEL_CALIBRATION);
					if (dlgConfigFuelCalibration.DoModal() == IDOK)
					{
						theMainDlg->m_pConfigReadWrite->fSetNowFCC(dlgConfigFuelCalibration.fGetValue1());
						theMainDlg->m_pConfigReadWrite->fSetAvgFCC(dlgConfigFuelCalibration.fGetValue2()); 
					}
				}
				break;
			case 7:	//调整油价
				{
					COBDPNDDlgAlarmValue dlgFuelPrice;
					dlgFuelPrice.fSetAlarmType(CONFIT_FUEL_PRICE);
					dlgFuelPrice.fSetDecLen(2);
					if (dlgFuelPrice.DoModal() == IDOK)
					{
						theMainDlg->m_pConfigReadWrite->fSetFuelPrice(dlgFuelPrice.fGetValue2()/1000.0);
					} 
				}
				break;
			case 8://出厂设置
				{
					COBDPNDDlgFactorySettings dlgFactorySet;
					if (dlgFactorySet.DoModal() == IDOK)
					{
						theMainDlg->m_pConfigReadWrite->fResetAppConfig();

						theMainDlg->fAppExit();
					}
				}
				break;
			case 9://版本信息
				{
					COBDPNDDlgVersion dlgVersion;
					dlgVersion.DoModal();

				}
				break;
			}

		}
		break;
	}

	CDialog::OnLButtonUp(nFlags, point);
}

BOOL COBDPNDDlgConfigMain::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;

	return CDialog::OnEraseBkgnd(pDC);
}

void COBDPNDDlgConfigMain::OnPaint()
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

		memDC.SelectObject(&m_fontText);
		memDC.SetTextColor(theMainDlg->m_clrTitle);

		for (int i=0;i<10;i++)
		{	
			if (m_dwTextImgID[i] != NULL)
			{
				for (int j=0;j<40;j++)
				{
					if (m_tagImg[j].dwResID == m_dwTextImgID[i])
					{
						memDC.BitBlt(m_rectImg[i].left,m_rectImg[i].top,m_rectImg[i].Width(),m_rectImg[i].Height(),	
							&m_tagImg[j].memdcUnit,0,0,SRCCOPY);
						break;
					}
				}
			}

			if (m_iCurrFunIndex == i)
				memDC.SetTextColor(theMainDlg->m_clrFootFocus);   //设置字体颜色白色  
			else
				memDC.SetTextColor(theMainDlg->m_clrFootNormal);   //设置字体颜色白色  					

			memDC.DrawText(m_strText[i],-1,m_rectText[i],DT_BOTTOM|DT_CENTER);
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

void COBDPNDDlgConfigMain::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值	


	CDialog::OnTimer(nIDEvent);
}
