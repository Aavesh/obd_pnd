// OBDPNDDlgBacklight.cpp : 实现文件
//

#include "stdafx.h"
#include "OBD_PND.h"
#include "OBDPNDDlgBacklight.h"

#include "OBD_PNDDlg.h"

extern "C"
BOOL SetEventData( 
				  HANDLE hEvent,
				  DWORD dwData
				  );

#define REG_BACKLIGHT				_T("ControlPanel\\Backlight")
#define VALUE_AC_TIMEOUT			_T("ACTimeOut")
#define VALUE_USE_EXT				_T("UseExt")
#define VALUE_BATTERY_TIMEOUT		_T("BatteryTimeOut")
#define VALUE_USE_BATTERY			_T("UseBattery")
#define VALUE_BATTERY_PRESCALE		_T("BatteryPrescale")
#define VALUE_AC_PRESCALE			_T("ACPrescale")

/**************************************************************
Function: YFGetBacklightInfo
Description: 
Get backlight value from system registry.
Parameters:
Backlight:system registry value of backlight.

Return： 
Nonzero indicates that successs.Zero indicates get failture.
Remarks：
**************************************************************/
BOOL COBDPNDDlgBacklight::YFGetBacklightInfo(BACKLIGHTINFO* Backlight)
{
	BOOL bAllRead = TRUE;
	HKEY hKey = NULL;
	DWORD dwType = 0;
	DWORD cbData = 0;

	if(ERROR_SUCCESS == RegOpenKeyEx(HKEY_CURRENT_USER,REG_BACKLIGHT,0,0,&hKey))
	{
		//AC Timeout.
		dwType = REG_DWORD;
		cbData = sizeof(DWORD);
		if(ERROR_SUCCESS != RegQueryValueEx(hKey,VALUE_AC_TIMEOUT,NULL,&dwType,
			(LPBYTE)&Backlight->ACTimeOut,&cbData))
		{
			Backlight->ACTimeOut = 0;
			bAllRead = FALSE;
		}
		//Battery Timeout.
		dwType = REG_DWORD;
		cbData = sizeof(DWORD);
		if(ERROR_SUCCESS != RegQueryValueEx(hKey,VALUE_BATTERY_TIMEOUT,NULL,&dwType,
			(LPBYTE)&Backlight->BatteryTimeOut,&cbData))
		{
			Backlight->BatteryTimeOut = 0;
			bAllRead = FALSE;
		}
		//UseExt.
		dwType = REG_DWORD;
		cbData = sizeof(DWORD);
		if(ERROR_SUCCESS != RegQueryValueEx(hKey,VALUE_USE_EXT,NULL,&dwType,
			(LPBYTE)&Backlight->UseExt,&cbData))
		{
			Backlight->UseExt = 0;
			bAllRead = FALSE;
		}
		//UseBattery.
		dwType = REG_DWORD;
		cbData = sizeof(DWORD);
		if(ERROR_SUCCESS != RegQueryValueEx(hKey,VALUE_USE_BATTERY,NULL,&dwType,
			(LPBYTE)&Backlight->UseBattery,&cbData))
		{
			Backlight->UseBattery = 0;
			bAllRead = FALSE;
		}
		//Battery Prescale.
		dwType = REG_DWORD;
		cbData = sizeof(DWORD);
		if(ERROR_SUCCESS != RegQueryValueEx(hKey,VALUE_BATTERY_PRESCALE,NULL,&dwType,
			(LPBYTE)&Backlight->BatteryPrescale,&cbData))
		{
			Backlight->BatteryPrescale = 0;
			bAllRead = FALSE;
		}
		//AC Prescale.
		dwType = REG_DWORD;
		cbData = sizeof(DWORD);
		if(ERROR_SUCCESS != RegQueryValueEx(hKey,VALUE_AC_PRESCALE,NULL,&dwType,
			(LPBYTE)&Backlight->ACPrescale,&cbData))
		{
			Backlight->ACPrescale = 0;
			bAllRead = FALSE;
		}

		RegCloseKey(hKey);
	}
	else
		bAllRead = FALSE;

	return bAllRead;
}

BOOL COBDPNDDlgBacklight::YFSetBacklightInfo(const BACKLIGHTINFO* Backlight)
{
	BOOL bAllWrite = TRUE;
	HKEY hKey = NULL;
	HANDLE hChangeEvent = NULL;

	if(ERROR_SUCCESS == RegOpenKeyEx(HKEY_CURRENT_USER,REG_BACKLIGHT,0,0,
		&hKey))
	{
		//AC Timeout.
		if(ERROR_SUCCESS != RegSetValueEx(hKey,VALUE_AC_TIMEOUT,0,REG_DWORD,
			(CONST BYTE*)&Backlight->ACTimeOut,sizeof(DWORD)))
			bAllWrite = FALSE;
		//Battery Timeout.
		if(ERROR_SUCCESS != RegSetValueEx(hKey,VALUE_BATTERY_TIMEOUT,0,REG_DWORD,
			(CONST BYTE*)&Backlight->BatteryTimeOut,sizeof(DWORD)))
			bAllWrite = FALSE;
		//UseExt.
		if(ERROR_SUCCESS != RegSetValueEx(hKey,VALUE_USE_EXT,0,REG_DWORD,
			(CONST BYTE*)&Backlight->UseExt,sizeof(DWORD)))
			bAllWrite = FALSE;
		//UseBattery.
		if(ERROR_SUCCESS != RegSetValueEx(hKey,VALUE_USE_BATTERY,0,REG_DWORD,
			(CONST BYTE*)&Backlight->UseBattery,sizeof(DWORD)))
			bAllWrite = FALSE;
		//Battery Prescale.
		if(ERROR_SUCCESS != RegSetValueEx(hKey,VALUE_BATTERY_PRESCALE,0,REG_DWORD,
			(CONST BYTE*)&Backlight->BatteryPrescale,sizeof(DWORD)))
			bAllWrite = FALSE;
		//AC Prescale.
		if(ERROR_SUCCESS != RegSetValueEx(hKey,VALUE_AC_PRESCALE,0,REG_DWORD,
			(CONST BYTE*)&Backlight->ACPrescale,sizeof(DWORD)))
			bAllWrite = FALSE;

		//Notify system that backlight had changed.
		hChangeEvent = OpenEvent(EVENT_ALL_ACCESS,FALSE,_T("BackLightChangeEvent"));
		if(hChangeEvent)
		{
			SetEvent(hChangeEvent);
			CloseHandle(hChangeEvent);
		}
#if HAVE_REGFLUSHKEY
		RegFlushKey(hKey);
#endif

		RegCloseKey(hKey);
	}
	else
		bAllWrite = FALSE;

	return bAllWrite;
}

void COBDPNDDlgBacklight::fSetBackLight(WORD wValue)
{
	YFGetBacklightInfo(&m_curBackLightInfo);
	if (m_curBackLightInfo.ACPrescale/20 != wValue)
	{
		BACKLIGHTINFO   newBackLightInfo = m_curBackLightInfo;
		newBackLightInfo.ACPrescale =  wValue*20;;
		newBackLightInfo.BatteryPrescale =  wValue*20;;
		YFSetBacklightInfo(&newBackLightInfo);		
	}
}
int COBDPNDDlgBacklight::fGetBackLight()
{
	YFGetBacklightInfo(&m_curBackLightInfo);
	return m_curBackLightInfo.ACPrescale/20;
}

// COBDPNDDlgBacklight 对话框

IMPLEMENT_DYNAMIC(COBDPNDDlgBacklight, CDialog)

COBDPNDDlgBacklight::COBDPNDDlgBacklight(CWnd* pParent /*=NULL*/)
	: CDialog(COBDPNDDlgBacklight::IDD, pParent)
{

}

COBDPNDDlgBacklight::~COBDPNDDlgBacklight()
{
}

void COBDPNDDlgBacklight::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(COBDPNDDlgBacklight, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// COBDPNDDlgBacklight 消息处理程序

BOOL COBDPNDDlgBacklight::OnInitDialog()
{
	CDialog::OnInitDialog();

	fInitUI();

	return TRUE;
}

void COBDPNDDlgBacklight::fInitUI()
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
			m_rectEditStart = CRect(160,320,168+172,320+51);
			m_rectEditEnd	= CRect(400,320,400+172,320+51);

			fCommDrawImage(&m_memdcEditStart,&m_bmpEditStart,&m_memdcBkBmp,m_rectEditStart,AfxGetInstanceHandle(),IDR_PNG_CONTROL_EDIT_NUM_800X480);
			fCommDrawImage(&m_memdcEditEnd,&m_bmpEditEnd,&m_memdcBkBmp,m_rectEditEnd,AfxGetInstanceHandle(),IDR_PNG_CONTROL_EDIT_NUM_800X480);
		}

		break;
	}
	fSetTitleText(theMainDlg->fGetBinText(DS_SETUP_BACKLIGHT_TITLE));
	fSetFootText(MAX_FOOT_INDEX-1,theMainDlg->fGetBinText(DS_BTN_OK));
	fSetFootText(0,_T("+"));
	fSetFootText(1,_T("-"));
	fSetFootText(2,theMainDlg->fGetBinText(DS_BTN_DAY));
	fSetFootText(3,theMainDlg->fGetBinText(DS_BTN_NIGHT));

	m_wBacklightDay = theMainDlg->m_pConfigReadWrite->fGetBacklightDay();
	m_wBacklightNight = theMainDlg->m_pConfigReadWrite->fGetBacklightNight();
	m_wBacklightDayStart = theMainDlg->m_pConfigReadWrite->fGetBacklightDayStart();
	m_wBacklightDayEnd = theMainDlg->m_pConfigReadWrite->fGetBacklightDayEnd();

	//背光设置
	CTime time = CTime::GetCurrentTime();
	DWORD dwNow = time.GetHour()*60+time.GetMinute();
	if (dwNow>theMainDlg->m_pConfigReadWrite->fGetBacklightDayStart() && dwNow<theMainDlg->m_pConfigReadWrite->fGetBacklightDayEnd())
	{
		m_bType == BACKLIGHT_DAY;
	}
	else
	{
		m_bType == BACKLIGHT_NIGHT;
	}

	this->Invalidate();
}
void COBDPNDDlgBacklight::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (!m_bKeyDown)
	{
		m_pointKeyDown = point;
		m_bKeyDown = TRUE;
		Sleep(100);
		SetTimer(1,50,NULL);
	}

	CDialog::OnLButtonDown(nFlags, point);
}

void COBDPNDDlgBacklight::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	m_bKeyDown = FALSE;
	KillTimer(1); 
	if (m_bType == BACKLIGHT_DAY)
	{
		if (m_pointKeyDown.x>m_rectEditStart.left && m_pointKeyDown.x<(m_rectEditStart.left+m_rectEditStart.Width()/4) && 
			m_pointKeyDown.y>m_rectEditStart.top && m_pointKeyDown.y<m_rectEditStart.bottom)
		{
			m_wBacklightDayStart--;
			if (m_wBacklightDayStart<=0)
				m_wBacklightDayStart = 0;
		}
		else if (m_pointKeyDown.x<m_rectEditStart.right && m_pointKeyDown.x>(m_rectEditStart.right-m_rectEditStart.Width()/4) && 
			m_pointKeyDown.y>m_rectEditStart.top && m_pointKeyDown.y<m_rectEditStart.bottom)
		{
			m_wBacklightDayStart++;
			if (m_wBacklightDayStart>=1440)
				m_wBacklightDayStart = 1440;
		}
		else if (m_pointKeyDown.x>m_rectEditEnd.left && m_pointKeyDown.x<(m_rectEditEnd.left+m_rectEditEnd.Width()/4) && 
			m_pointKeyDown.y>m_rectEditEnd.top && m_pointKeyDown.y<m_rectEditEnd.bottom)
		{
			m_wBacklightDayEnd--;
			if (m_wBacklightDayEnd<=0)
				m_wBacklightDayEnd = 0;
		}
		else if (m_pointKeyDown.x<m_rectEditEnd.right && m_pointKeyDown.x>(m_rectEditEnd.right-m_rectEditEnd.Width()/4) && 
			m_pointKeyDown.y>m_rectEditEnd.top && m_pointKeyDown.y<m_rectEditEnd.bottom)
		{
			m_wBacklightDayEnd++;
			if (m_wBacklightDayEnd>=1440)
				m_wBacklightDayEnd = 1440;
		}

	}
	else
	{
		if (m_pointKeyDown.x>m_rectEditStart.left && m_pointKeyDown.x<(m_rectEditStart.left+m_rectEditStart.Width()/4) && 
			m_pointKeyDown.y>m_rectEditStart.top && m_pointKeyDown.y<m_rectEditStart.bottom)
		{
			m_wBacklightDayEnd--;
			if (m_wBacklightDayEnd<=0)
				m_wBacklightDayEnd = 0;
		}
		else if (m_pointKeyDown.x<m_rectEditStart.right && m_pointKeyDown.x>(m_rectEditStart.right-m_rectEditStart.Width()/4) && 
			m_pointKeyDown.y>m_rectEditStart.top && m_pointKeyDown.y<m_rectEditStart.bottom)
		{
			m_wBacklightDayEnd++;
			if (m_wBacklightDayEnd>=1440)
				m_wBacklightDayEnd = 1440;
		}
		else if (m_pointKeyDown.x>m_rectEditEnd.left && m_pointKeyDown.x<(m_rectEditEnd.left+m_rectEditEnd.Width()/4) && 
			m_pointKeyDown.y>m_rectEditEnd.top && m_pointKeyDown.y<m_rectEditEnd.bottom)
		{
			m_wBacklightDayStart--;
			if (m_wBacklightDayStart<=0)
				m_wBacklightDayStart = 0;
		}
		else if (m_pointKeyDown.x<m_rectEditEnd.right && m_pointKeyDown.x>(m_rectEditEnd.right-m_rectEditEnd.Width()/4) && 
			m_pointKeyDown.y>m_rectEditEnd.top && m_pointKeyDown.y<m_rectEditEnd.bottom)
		{
			m_wBacklightDayStart++;
			if (m_wBacklightDayStart>=1440)
				m_wBacklightDayStart = 1440;
		}


	}

	switch (fGetCurrFootIndex(point))
	{
	case 0:
		{
			if (m_wBacklight <5)
				m_wBacklight++;

			if (m_bType == BACKLIGHT_DAY)
				m_wBacklightDay = m_wBacklight;
			else
				m_wBacklightNight = m_wBacklight; 
		}		
		break;
	case 1:
		{
			if (m_wBacklight >0)
				m_wBacklight--;

			if (m_bType == BACKLIGHT_DAY)
				m_wBacklightDay = m_wBacklight;
			else
				m_wBacklightNight = m_wBacklight;
		}
		break;
	case 2:
		{
			m_wBacklight = m_wBacklightDay;
			m_bType = BACKLIGHT_DAY;
		}
		break;
	case 3:
		{
			m_wBacklight = m_wBacklightNight;
			m_bType = BACKLIGHT_NIGHT;
		}
		break;
	case MAX_FOOT_INDEX-1:
		{
			theMainDlg->m_pConfigReadWrite->fSetBacklightDay(m_wBacklightDay);
			theMainDlg->m_pConfigReadWrite->fSetBacklightNight(m_wBacklightNight);
			theMainDlg->m_pConfigReadWrite->fSetBacklightDayStart(m_wBacklightDayStart);
			theMainDlg->m_pConfigReadWrite->fSetBacklightDayEnd(m_wBacklightDayEnd);

			CDialog::OnOK();
		}
		break;
	}

	fSetBackLight(m_wBacklight);

	this->Invalidate();

	CDialog::OnLButtonUp(nFlags, point);
}

BOOL COBDPNDDlgBacklight::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;

	return CDialog::OnEraseBkgnd(pDC);
}

void COBDPNDDlgBacklight::OnPaint()
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

		CRect rcText;
		for (int i=0;i<5;i++)
		{
			CPen pen(PS_SOLID,1,RGB(0,255,255));
			CPen * oldPen = memDC.SelectObject(&pen);
			CBrush *pNullBrush=CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
			CBrush *pFullBrush = new CBrush(RGB(0,255,255));
			CBrush *pOldBrush;
			if (i<m_wBacklight)
				pOldBrush = memDC.SelectObject(pFullBrush);
			else
				pOldBrush = memDC.SelectObject(pNullBrush);

			rcText.left = 160 + i*90;
			rcText.right = rcText.left + 50;
			rcText.top = 160 - i*20;
			rcText.bottom = 200 + i*20;

			memDC.Rectangle(rcText);

			memDC.SelectObject(pOldBrush);
			memDC.SelectObject(oldPen);

			DeleteObject(pen);
			DeleteObject(oldPen);
			DeleteObject(pNullBrush);
			DeleteObject(pFullBrush);
			DeleteObject(pOldBrush);
		}

		CString strText;
		memDC.BitBlt(m_rectEditStart.left,m_rectEditStart.top,m_rectEditStart.Width(),m_rectEditStart.Height(),	
			&m_memdcEditStart,0,0,SRCCOPY);
		if (m_bType ==BACKLIGHT_DAY)
			strText.Format(_T("%02d:%02d"),m_wBacklightDayStart/60,m_wBacklightDayStart%60); 
		else
			strText.Format(_T("%02d:%02d"),m_wBacklightDayEnd/60,m_wBacklightDayEnd%60);
		memDC.DrawText(strText, -1, &m_rectEditStart, DT_CENTER|DT_VCENTER);

		strText.Format(_T("-"));
		CRect rectTemp = CRect(m_rectEditStart.right,m_rectEditStart.top,m_rectEditEnd.left,m_rectEditEnd.bottom);
		memDC.DrawText(strText, -1, &rectTemp, DT_CENTER|DT_VCENTER);

		memDC.BitBlt(m_rectEditEnd.left,m_rectEditEnd.top,m_rectEditEnd.Width(),m_rectEditEnd.Height(),	
			&m_memdcEditEnd,0,0,SRCCOPY);
		if (m_bType ==BACKLIGHT_DAY)
			strText.Format(_T("%02d:%02d"),m_wBacklightDayEnd/60,m_wBacklightDayEnd%60);
		else
			strText.Format(_T("%02d:%02d"),m_wBacklightDayStart/60,m_wBacklightDayStart%60);
		memDC.DrawText(strText, -1, &m_rectEditEnd, DT_CENTER|DT_VCENTER);


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

void COBDPNDDlgBacklight::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值	
	if (nIDEvent == 1 && m_bKeyDown)
	{
		if (m_bType == BACKLIGHT_DAY)
		{
			if (m_pointKeyDown.x>m_rectEditStart.left && m_pointKeyDown.x<(m_rectEditStart.left+m_rectEditStart.Width()/4) && 
				m_pointKeyDown.y>m_rectEditStart.top && m_pointKeyDown.y<m_rectEditStart.bottom)
			{
				m_wBacklightDayStart--;
				if (m_wBacklightDayStart<=0)
					m_wBacklightDayStart = 0;
			}
			else if (m_pointKeyDown.x<m_rectEditStart.right && m_pointKeyDown.x>(m_rectEditStart.right-m_rectEditStart.Width()/4) && 
				m_pointKeyDown.y>m_rectEditStart.top && m_pointKeyDown.y<m_rectEditStart.bottom)
			{
				m_wBacklightDayStart++;
				if (m_wBacklightDayStart>=1440)
					m_wBacklightDayStart = 1440;
			}
			else if (m_pointKeyDown.x>m_rectEditEnd.left && m_pointKeyDown.x<(m_rectEditEnd.left+m_rectEditEnd.Width()/4) && 
				m_pointKeyDown.y>m_rectEditEnd.top && m_pointKeyDown.y<m_rectEditEnd.bottom)
			{
				m_wBacklightDayEnd--;
				if (m_wBacklightDayEnd<=0)
					m_wBacklightDayEnd = 0;
			}
			else if (m_pointKeyDown.x<m_rectEditEnd.right && m_pointKeyDown.x>(m_rectEditEnd.right-m_rectEditEnd.Width()/4) && 
				m_pointKeyDown.y>m_rectEditEnd.top && m_pointKeyDown.y<m_rectEditEnd.bottom)
			{
				m_wBacklightDayEnd++;
				if (m_wBacklightDayEnd>=1440)
					m_wBacklightDayEnd = 1440;
			}

		}
		else
		{
			if (m_pointKeyDown.x>m_rectEditStart.left && m_pointKeyDown.x<(m_rectEditStart.left+m_rectEditStart.Width()/4) && 
				m_pointKeyDown.y>m_rectEditStart.top && m_pointKeyDown.y<m_rectEditStart.bottom)
			{
				m_wBacklightDayEnd--;
				if (m_wBacklightDayEnd<=0)
					m_wBacklightDayEnd = 0;
			}
			else if (m_pointKeyDown.x<m_rectEditStart.right && m_pointKeyDown.x>(m_rectEditStart.right-m_rectEditStart.Width()/4) && 
				m_pointKeyDown.y>m_rectEditStart.top && m_pointKeyDown.y<m_rectEditStart.bottom)
			{
				m_wBacklightDayEnd++;
				if (m_wBacklightDayEnd>=1440)
					m_wBacklightDayEnd = 1440;
			}
			else if (m_pointKeyDown.x>m_rectEditEnd.left && m_pointKeyDown.x<(m_rectEditEnd.left+m_rectEditEnd.Width()/4) && 
				m_pointKeyDown.y>m_rectEditEnd.top && m_pointKeyDown.y<m_rectEditEnd.bottom)
			{
				m_wBacklightDayStart--;
				if (m_wBacklightDayStart<=0)
					m_wBacklightDayStart = 0;
			}
			else if (m_pointKeyDown.x<m_rectEditEnd.right && m_pointKeyDown.x>(m_rectEditEnd.right-m_rectEditEnd.Width()/4) && 
				m_pointKeyDown.y>m_rectEditEnd.top && m_pointKeyDown.y<m_rectEditEnd.bottom)
			{
				m_wBacklightDayStart++;
				if (m_wBacklightDayStart>=1440)
					m_wBacklightDayStart = 1440;
			}
		}

		this->Invalidate();
	}
	CDialog::OnTimer(nIDEvent);
}
