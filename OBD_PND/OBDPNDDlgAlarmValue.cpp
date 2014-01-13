// OBDPNDDlgAlarmCoolant.cpp : 实现文件
//

#include "stdafx.h"
#include "OBD_PND.h"
#include "OBDPNDDlgAlarmValue.h"
#include "OBD_PNDDlg.h"
#include "OBDPNDDlgShowText.h"
#include "OBDPNDDlgConfigNowFCC.h"
#include "OBDPNDDlgConfigAvgFCC.h"

// COBDPNDDlgAlarmValue 对话框

IMPLEMENT_DYNAMIC(COBDPNDDlgAlarmValue, CDialog)

COBDPNDDlgAlarmValue::COBDPNDDlgAlarmValue(CWnd* pParent /*=NULL*/)
	: CDialog(COBDPNDDlgAlarmValue::IDD, pParent)
{
	m_bEnableDec = FALSE;
	m_wDecLen = 0;
	m_iCurrNumberIndex = -1;

	m_bSwitch = TRUE;

	m_dwAlarmType = 0;
}

COBDPNDDlgAlarmValue::~COBDPNDDlgAlarmValue()
{
}

void COBDPNDDlgAlarmValue::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(COBDPNDDlgAlarmValue, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// COBDPNDDlgAlarmValue 消息处理程序

BOOL COBDPNDDlgAlarmValue::OnInitDialog()
{
	CDialog::OnInitDialog();

	fInitUI();

	return TRUE;
}

void COBDPNDDlgAlarmValue::fSetAlarmType(DWORD dwAlarmType)
{
	m_dwAlarmType = dwAlarmType;
}

void  COBDPNDDlgAlarmValue::fSetDecLen(WORD wDecLen )
{
	if (wDecLen == 0)
		m_bEnableDec = FALSE;
	else
	{
		m_bEnableDec = TRUE;
		m_wDecLen = wDecLen+1; //算上小数点
	}
}

DWORD COBDPNDDlgAlarmValue::fGetValue1()
{
	DWORD dwValue = 0;
	switch (m_dwAlarmType)
	{
	case ALARM_VALUE_OVERSPEED:
	case ALARM_VALUE_COOLANT:
	case ALARM_VALUE_FATIGUE_DRIVING:
	case CONFIT_FUEL_PRICE:
	case CONFIG_FUEL_CALIBRATION:
	case CONFIG_FUEL_NOW_SETUP:
		dwValue = wcstod(m_strText[0][1],0);
		break;
	case ALARM_VALUE_BATTERY_VOLTAGE:
		dwValue = wcstod(m_strText[0][1],0)*10; 
		break;
	}
	return dwValue;
}

DWORD COBDPNDDlgAlarmValue::fGetValue2()
{
	DWORD dwValue = 0;
	switch (m_dwAlarmType)
	{
	case ALARM_VALUE_OVERSPEED:
	case ALARM_VALUE_COOLANT:
	case ALARM_VALUE_FATIGUE_DRIVING:
	case CONFIG_FUEL_CALIBRATION:
		dwValue = wcstod(m_strText[1][1],0);
		break;
	case ALARM_VALUE_BATTERY_VOLTAGE:
	case CONFIG_FUEL_NOW_SETUP:
	case CONFIG_FUEL_AVG_SETUP:
		dwValue = wcstod(m_strText[1][1],0)*10.0;
		break;
	case CONFIT_FUEL_PRICE:
		dwValue = wcstod(m_strText[1][1],0)*1000.0;
		break;

	}
	return dwValue;

}

void COBDPNDDlgAlarmValue::fInitUI()
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
			fSetBkImg(this->GetDC(),IDR_PNG_EDIT_VALUE_800X480);

			m_rectText[0][0] = CRect(50,100,750,155);
			m_rectText[1][0] = CRect(50,160,750,215);
			m_rectText[0][1]= CRect(570,100,570+172,100+51);
			m_rectText[1][1] = CRect(570,160,570+172,160+51);

			VERIFY(m_fontText.CreateFont(
				40,                        // nHeight
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

			VERIFY(m_fontNumber.CreateFont(
				53,                        // nHeight
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

			m_rectNumber[1] = CRect(50,230,160,305);
			m_rectNumber[2] = CRect(170,235,270,305);
			m_rectNumber[3] = CRect(290,235,380,305);
			m_rectNumber[4] = CRect(400,235,500,305);
			m_rectNumber[5] = CRect(520,235,610,305);

			m_rectNumber[6] = CRect(50,320,160,400);
			m_rectNumber[7] = CRect(170,320,270,400);
			m_rectNumber[8] = CRect(290,320,380,400);
			m_rectNumber[9] = CRect(400,320,500,400);
			m_rectNumber[0] = CRect(520,320,610,400);

			m_rectNumber[10] = CRect(640,230,745,390);

			fCommDrawImage(&m_memdcEdit[0],&m_cBitmapEdit[0],&m_memdcBkBmp,m_rectText[0][1],AfxGetInstanceHandle(),IDR_PNG_WIDGIT_EDITTEXT_800X480);
			fCommDrawImage(&m_memdcEdit[1],&m_cBitmapEdit[1],&m_memdcBkBmp,m_rectText[1][1],AfxGetInstanceHandle(),IDR_PNG_WIDGIT_EDITTEXT_800X480);
		}

		break;
	}

	m_clrText = RGB(255,255,255);
	m_clrSwitch = RGB(100,100,100);

	switch (m_dwAlarmType)
	{
	case ALARM_VALUE_OVERSPEED:
		{
			fSetTitleText(theMainDlg->fGetBinText(DS_CONFIG_ALARM_OVERSPEED));
			m_strText[1][0] = theMainDlg->fGetBinText(DS_CONFIG_ALARM_OVERSPEED_TEXT2);
			m_strText[1][1].Format(_T("%d"),theMainDlg->m_pConfigReadWrite->fGetOverSpeedAlarmValue());
		}
		break;
	case ALARM_VALUE_COOLANT:
		{
			fSetTitleText(theMainDlg->fGetBinText(DS_CONFIG_ALARM_WATER_TEMP));
			m_strText[1][0] = theMainDlg->fGetBinText(DS_CONFIG_ALARM_WATER_TEMP_TEXT2);
			m_strText[1][1].Format(_T("%d"),theMainDlg->m_pConfigReadWrite->fGetWaterTemperatureAlarmValue());
		}
		break;
	case ALARM_VALUE_FATIGUE_DRIVING:
		{
			fSetTitleText(theMainDlg->fGetBinText(DS_CONFIG_ALARM_FATIGUE_DRIVING));
			m_strText[1][0] = theMainDlg->fGetBinText(DS_CONFIG_ALARM_FATIGUE_DRIVING_TEXT2);
			m_strText[1][1].Format(_T("%d"),theMainDlg->m_pConfigReadWrite->fGetFatigueDrivingAlarmValue());
		}
		break;
	case ALARM_VALUE_BATTERY_VOLTAGE:
		{
			m_bSwitch = FALSE;
			fSetTitleText(theMainDlg->fGetBinText(DS_CONFIG_ALARM_BATTERY_VOLTAGE));
			m_strText[0][0] = theMainDlg->fGetBinText(DS_CONFIG_ALARM_BATTERY_VOLTAGE_TEXT1);
			m_strText[1][0] = theMainDlg->fGetBinText(DS_CONFIG_ALARM_BATTERY_VOLTAGE_TEXT2);
			m_strText[0][1].Format(_T("%d.%d"),theMainDlg->m_pConfigReadWrite->fGetVoltMinAlarm()/10,theMainDlg->m_pConfigReadWrite->fGetVoltMinAlarm()%10);
			m_strText[1][1].Format(_T("%d.%d"),theMainDlg->m_pConfigReadWrite->fGetVoltMaxAlarm()/10,theMainDlg->m_pConfigReadWrite->fGetVoltMaxAlarm()%10);
		}
		break;
	case CONFIG_FUEL_CALIBRATION:
		{
			fSetTitleText(theMainDlg->fGetBinText(DS_CONFIG_FUEL_CALIBRATION));
			m_strText[0][0] = theMainDlg->fGetBinText(DS_CONFIG_FUEL_CALIBRATION_TEXT1);
			m_strText[1][0] = theMainDlg->fGetBinText(DS_CONFIG_FUEL_CALIBRATION_TEXT2);
			m_strText[0][1].Format(_T("%0.0f"),theMainDlg->m_pConfigReadWrite->fGetNowFCC());
			m_strText[1][1].Format(_T("%0.0f"),theMainDlg->m_pConfigReadWrite->fGetAvgFCC());

			m_bSwitch = FALSE;
			fSetFootText(1,theMainDlg->fGetBinText(DS_BTN_CALIBRATE));				//校准			
		}
		break;
	case CONFIT_FUEL_PRICE:
		{
			fSetTitleText(theMainDlg->fGetBinText(DS_CONFIG_FUEL_PRICE));
			m_strText[1][0] = theMainDlg->fGetBinText(DS_CONFIG_FUEL_PRICE_TEXT2);
			m_strText[1][1].Format(_T("%0.2f"),theMainDlg->m_pConfigReadWrite->fGetFuelPrice());
		}
		break;
	case CONFIG_FUEL_NOW_SETUP:
		{
			m_bSwitch = FALSE;
			fSetTitleText(theMainDlg->fGetBinText(DS_CONFIG_FUEL_NOW_SETUP_TITLE));
			m_strText[0][0] = theMainDlg->fGetBinText(DS_CONFIG_FUEL_NOW_SETUP_TEXT1);
			m_strText[0][1].Format(_T("0"));
			m_strText[1][0] = theMainDlg->fGetBinText(DS_CONFIG_FUEL_NOW_SETUP_TEXT2);
			m_strText[1][1].Format(_T("0.0"));
	
		}
		break;
	case CONFIG_FUEL_AVG_SETUP:
		{
			fSetTitleText(theMainDlg->fGetBinText(DS_CONFIG_FUEL_AVG_SETUP_TITLE));
			m_strText[0][0].Format(_T(""));
			m_strText[0][1].Format(_T(""));
			m_strText[1][0] = theMainDlg->fGetBinText(DS_CONFIG_FUEL_AVG_SETUP_TEXT2);
			m_strText[1][1].Format(_T("0.0"));
		}
		break;
	}

	fSetFootText(0,theMainDlg->fGetBinText(DS_BTN_MODIFY));				//修改
	fSetFootText(MAX_FOOT_INDEX-2,theMainDlg->fGetBinText(DS_BTN_OK));	//确定
	fSetFootText(MAX_FOOT_INDEX-1,theMainDlg->fGetBinText(DS_BTN_RETURN));	//返回


	this->Invalidate();
}
void COBDPNDDlgAlarmValue::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDialog::OnLButtonDown(nFlags, point);
}

void COBDPNDDlgAlarmValue::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	// 上下行切换
	int iNumIndex = -1;
	if (m_strText[0][0]!=_T("") && 
		point.x>m_rectText[0][0].left && point.x<m_rectText[0][1].right && point.y>m_rectText[0][0].top && point.y<m_rectText[0][0].bottom)
		m_bSwitch = FALSE;
	if (point.x>m_rectText[1][0].left && point.x<m_rectText[1][1].right && point.y>m_rectText[1][0].top && point.y<m_rectText[1][0].bottom)
		m_bSwitch = TRUE;

	iNumIndex = -1;
	for (int i=0;i<11;i++)
	{
		if (point.x>m_rectNumber[i].left && point.x<m_rectNumber[i].right && point.y>m_rectNumber[i].top && point.y<m_rectNumber[i].bottom)
		{
			m_iCurrNumberIndex = i;
			iNumIndex = i;
			break;
		}
	}
	if (iNumIndex>=0)	//小数点
	{
		if (!m_bSwitch)
		{
			int iPoint = m_strText[0][1].Find(_T("."));//小数点位置
			if (iNumIndex==10 )
			{
				if (m_bEnableDec && iPoint<0 )
					m_strText[0][1] = m_strText[0][1] + _T(".");
			}
			else
			{
				if (wcstod(m_strText[0][1],0) == 0)
					m_strText[0][1] = _T("");

				if (m_strText[0][1].GetLength()<=5)
				{
					if (iNumIndex ==0)
					{
						if (_ttoi(m_strText[0][1])!=0)
						{
							//总长度小于五位，小数位为两位
							if((iPoint>=0 && m_strText[0][1].GetLength() - iPoint >=m_wDecLen))
								m_strText[0][1] = m_strText[0][1];
							else
								m_strText[0][1].Format(_T("%s%d"),m_strText[0][1],iNumIndex);
						}
					}
					else
					{
						//总长度小于五位，小数位为两位
						if((iPoint>=0 && m_strText[0][1].GetLength() - iPoint >=m_wDecLen))
							m_strText[0][1] = m_strText[0][1];
						else
							m_strText[0][1].Format(_T("%s%d"),m_strText[0][1],iNumIndex);
					}



				}
			}
		}
		else
		{
			int iPoint = m_strText[1][1].Find(_T("."));//小数点位置
			if (iNumIndex==10 )
			{
				if (m_bEnableDec && iPoint<0 )
					m_strText[1][1] = m_strText[1][1] + _T(".");
			}
			else
			{
				if (wcstod(m_strText[1][1],0) == 0)
					m_strText[1][1] = _T("");

				if (m_strText[1][1].GetLength()<=5)
				{ 
					if (iNumIndex ==0)
					{
						if (_ttoi(m_strText[1][1])!=0)
						{
							//总长度小于五位，小数位为两位
							if((iPoint>=0 && m_strText[1][1].GetLength() - iPoint >=m_wDecLen))
								m_strText[1][1] = m_strText[1][1];
							else
								m_strText[1][1].Format(_T("%s%d"),m_strText[1][1],iNumIndex);
						}
					}
					else
					{
						//总长度小于五位，小数位为两位
						if((iPoint>=0 && m_strText[1][1].GetLength() - iPoint >=m_wDecLen))
							m_strText[1][1] = m_strText[1][1];
						else
							m_strText[1][1].Format(_T("%s%d"),m_strText[1][1],iNumIndex);
					}



				}
			}

		}
	}

	switch (fGetCurrFootIndex(point))
	{
	case 0:
		{
			if (!m_bSwitch)
				m_strText[0][1] = _T("");
			else
				m_strText[1][1] = _T("");
		}
		break;
	case 1:
		{
			if (m_dwAlarmType == CONFIG_FUEL_CALIBRATION)
			{
				if (!m_bSwitch)
				{
					//AfxMessageBox(_T("即时油耗校准"));
					COBDPNDDlgShowText dlgShowText;
					dlgShowText.fSetTitleText(theMainDlg->fGetBinText(DS_CONFIG_FUEL_NOW_TITLE));
					vector<CString> vecList;
					vecList.push_back(theMainDlg->fGetBinText(DS_CONFIG_FUEL_NOW_TIP1));
					vecList.push_back(theMainDlg->fGetBinText(DS_CONFIG_FUEL_NOW_TIP2));
					dlgShowText.fSetTextList(vecList);
					if (dlgShowText.DoModal() == IDOK)
					{
						 COBDPNDDlgAlarmValue dlgValue;
						 dlgValue.fSetAlarmType(CONFIG_FUEL_NOW_SETUP);
						 dlgValue.fSetDecLen(1);
						 if(dlgValue.DoModal() == IDOK)
						 {
							COBDPNDDlgConfigNowFCC dlgConfigFuelNow;
							WORD wSpeed = dlgValue.fGetValue1();
							double dbFuelCon = dlgValue.fGetValue2()/10.0;
							dlgConfigFuelNow.fSetData(wSpeed,dbFuelCon);
							if (dlgConfigFuelNow.DoModal() == IDOK)
							{					
								m_strText[0][1].Format(_T("%0.0f"),dlgConfigFuelNow.fGetNewFuelConCoeff());
								this->Invalidate();
							}
						 }
					}

				}
				else
				{
					//AfxMessageBox(_T("平均油耗校准"));
					COBDPNDDlgShowText dlgShowText;
					dlgShowText.fSetTitleText(theMainDlg->fGetBinText(DS_CONFIG_FUEL_AVG_TITLE));
					vector<CString> vecList;
					vecList.push_back(theMainDlg->fGetBinText(DS_CONFIG_FUEL_AVG_TIP1));
					vecList.push_back(theMainDlg->fGetBinText(DS_CONFIG_FUEL_AVG_TIP2));
					dlgShowText.fSetTextList(vecList);
					if (dlgShowText.DoModal() == IDOK)
					{
						COBDPNDDlgAlarmValue dlgValue;
						dlgValue.fSetAlarmType(CONFIG_FUEL_AVG_SETUP);
						dlgValue.fSetDecLen(1);
						if(dlgValue.DoModal() == IDOK)
						{									
							double dbNewValue = dlgValue.fGetValue2()/10;
							double dbHisAvgFueCon = theMainDlg->fGetDataStreamValueF(0xFF010005);
							if (dbHisAvgFueCon == INVALID_DATASTREAM_VALUE)
								dbHisAvgFueCon = 0;
							COBDPNDDlgConfigAvgFCC dlgConfigFuelAvg;
							dlgConfigFuelAvg.fSetAvgFuelConmuption(dbNewValue);
							if (dlgConfigFuelAvg.DoModal() == IDOK)
							{
								float fNewAvgOilFuel =theMainDlg->m_pConfigReadWrite->fGetAvgFCC()	* dbNewValue / dbHisAvgFueCon;
								m_strText[1][1].Format(_T("%0.0f"),fNewAvgOilFuel);
								this->Invalidate();
							}
						}
					}
				}
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

BOOL COBDPNDDlgAlarmValue::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;

	return CDialog::OnEraseBkgnd(pDC);
}

void COBDPNDDlgAlarmValue::OnPaint()
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

		if (m_strText[0][0] != _T(""))
		{
			memDC.BitBlt(m_rectText[0][1].left,m_rectText[0][1].top,m_rectText[0][1].Width(),m_rectText[0][1].Height(),	&m_memdcEdit[0],0,0,SRCCOPY);

			memDC.SelectObject(&m_fontText); 
			memDC.SetTextColor(m_clrText);
			memDC.DrawText(m_strText[0][0], -1, m_rectText[0][0], DT_LEFT);		

			if (!m_bSwitch)
				memDC.SetTextColor(theMainDlg->m_clrFootFocus);   //设置字体颜色白色  		
			else
				memDC.SetTextColor(theMainDlg->m_clrFootNormal);   //设置字体颜色白色  
			memDC.DrawText(m_strText[0][1], -1, m_rectText[0][1], DT_CENTER|DT_VCENTER);	
		}

		memDC.BitBlt(m_rectText[1][1].left,m_rectText[1][1].top,m_rectText[1][1].Width(),m_rectText[1][1].Height(),	&m_memdcEdit[1],0,0,SRCCOPY);

		memDC.SelectObject(&m_fontText); 
		memDC.SetTextColor(m_clrText);
		memDC.DrawText(m_strText[1][0], -1, m_rectText[1][0], DT_LEFT);	

		if (m_bSwitch)
			memDC.SetTextColor(theMainDlg->m_clrFootFocus);   //设置字体颜色白色  		
		else
			memDC.SetTextColor(theMainDlg->m_clrFootNormal);   //设置字体颜色白色  	
		memDC.DrawText(m_strText[1][1], -1, m_rectText[1][1], DT_CENTER|DT_VCENTER);	

		memDC.SelectObject(&m_fontNumber); 
		CString strNumber;
		for (int i=0;i<=10;i++)
		{
			if (i == m_iCurrNumberIndex)
				memDC.SetTextColor(theMainDlg->m_clrFootFocus);   //设置字体颜色白色  		
			else
				memDC.SetTextColor(theMainDlg->m_clrFootNormal);   //设置字体颜色白色  
			if (i == 10)
				strNumber.Format(_T("."));
			else
				strNumber.Format(_T("%d"),i);
			memDC.DrawText(strNumber,-1,m_rectNumber[i],DT_CENTER|DT_VCENTER);
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

void COBDPNDDlgAlarmValue::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值	


	CDialog::OnTimer(nIDEvent);
}
