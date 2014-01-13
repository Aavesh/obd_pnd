// OBD_PNDDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OBD_PND.h"
#include "OBD_PNDDlg.h"
#include "OBDPNDDlgConfigMain.h"

#include "OBDPNDDlgSetupGuide.h"
#include "OBDPNDDlgConnect.h"
#include "OBDPNDDlgBacklight.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// COBD_PNDDlg �Ի���

COBD_PNDDlg * theMainDlg = NULL;

COBD_PNDDlg::COBD_PNDDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COBD_PNDDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pSplashWindow = NULL;
	m_pDlgLog = NULL;
	m_pDlgTip = NULL;

	m_pDlgAlarmOverSpeed = NULL;
	m_pDlgAlarmCoolant = NULL;
	m_pDlgAlarmFatigueDriving = NULL;

	m_pConfigReadWrite = NULL;
	m_pCommunication = NULL;

	m_pDlgMainIdle = NULL;
	m_pDlgMainTour = NULL;
	m_pDlgMainRace = NULL;
	m_pDlgMainDetail = NULL;

	m_pSeriesGps = NULL;

	m_wCurrPanelIndex = 0;
	m_wBeforPanelIndex = 0;

	m_bBackRunning = FALSE;
	m_bSetting = FALSE;
	m_bVaildComm = FALSE;

	m_fCompassValue = 0;

	m_bUpdateDateTimeByGps = FALSE;


	m_bShowLog = FALSE;
}

void COBD_PNDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(COBD_PNDDlg, CDialog)
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	ON_WM_SIZE()
#endif
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SERIES_RECV,OnSerialRecvData) 
	ON_MESSAGE(WM_SERIES_RESTART,OnSerialRestart)
	ON_MESSAGE(WM_OBDPND_MINI_SHOW,OnMiniShow)
	ON_MESSAGE(WM_OBDPND_MINI_HIDE,OnMiniHide)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// ��ȡָ����λ��
float COBD_PNDDlg::fGetCompassValue()
{
	return m_pSeriesGps->fGetComPassValue();
}
//
map<DWORD,TagDtcInfo> COBD_PNDDlg::fGetDtcList()
{
	return m_pCommunication->fGetDtcList();
}
// ��ȡ����������
CString COBD_PNDDlg::fGetDataStreamValue(DWORD dwDsID)
{
	return m_pCommunication->fGetDataStreamValue(dwDsID);
}
float COBD_PNDDlg::fGetDataStreamValueF(DWORD dwDsID)
{
	return m_pCommunication->fGetDataStreamValueF(dwDsID);

}
CString COBD_PNDDlg::fGetDataStreamText(DWORD dwDsID)
{
	return m_pCommunication->fGetDataStreamText(dwDsID);

}


void COBD_PNDDlg::fShowDtcInfo()
{
	//�й����룬
	KillTimer(OBDPND_TIMER_GET_DTC_INFO);
	SetTimer(OBDPND_TIMER_GET_DTC_INFO,1000,NULL);
}

// ��ȡ����
BOOL COBD_PNDDlg::fStartSeries(int iPort)
{
	return m_pCommunication->fStartSeries(iPort);
}

// ֹͣ��ȡ
void COBD_PNDDlg::fStopSerial()
{
	m_pCommunication->fStopSerial();
}

void COBD_PNDDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ	
	switch (nIDEvent)
	{
	case OBDPND_TIMER:
		break;
	case OBDPND_TIMER_GET_DTC_INFO:
		{
			KillTimer(OBDPND_TIMER_GET_DTC_INFO);

			if (m_pConfigReadWrite->fGetTroubleCodeAlarmStatus() &&
				m_pCommunication->fGetDtcList().size()>0)
			{
				fCommPlaySound(m_pConfigReadWrite->fGetSoundFile(SOUND_FILE_AlarmTrouble));
				CWnd * pWnd = CWnd::FindWindow(NULL,_T("OBD_PND_DTC"));
				if (pWnd == NULL)
				{
					COBDPNDDlgDTCInfo dlgDtcInfo;
					dlgDtcInfo.DoModal();
				}
				else
				{
					//
				}
			}

		}
		break;
	case OBDPND_TIMER_START_COMM:
		{
			KillTimer(OBDPND_TIMER_START_COMM);
			//��������ɱ��ռ�ô��ڳ���
			vector<CString> vecAppList = m_pConfigReadWrite->fGetFilterAppList();
			for (int i=0;i<vecAppList.size();i++)
			{
				fAddLog(_T("�������:")+vecAppList[i]);
				fCommKillProcess(vecAppList[i]);
			}
			if (m_pConfigReadWrite->fGetOBDComPortNo() == 0)
			{
				fAddLog(_T("�����ô���..."));

				int iPortNo = m_pCommunication->fCheckVaildSerialPort();
				if(iPortNo >= 0)
				{
					m_pConfigReadWrite->fSetOBDComPortNo(iPortNo);
					m_pCommunication->fStartSeries(iPortNo);
				}
				else
				{
					fShowTipInfo(fGetBinText(DS_ECU_COM_FAILURE));
					COBDPNDDlgConnect dlgConnet;					
					if (dlgConnet.DoModal() == IDOK)
					{
						fShowTipInfo(fGetBinText(DS_ECU_COM_INIT));
						fCommPlaySound(m_pConfigReadWrite->fGetSoundFile(SOUND_FILE_GetData));
						SetTimer(OBDPND_TIMER_START_COMM,10,NULL);
					}
					else
					{
						fAppExit();
					}
					return;
				}
			}
			else
			{
				if(!m_pCommunication->fStartSeries(m_pConfigReadWrite->fGetOBDComPortNo()) ) 
				{
					fShowTipInfo(fGetBinText(DS_ECU_COM_FAILURE));
					COBDPNDDlgConnect dlgConnet;					
					if (dlgConnet.DoModal() == IDOK)
					{
						fShowTipInfo(fGetBinText(DS_ECU_COM_INIT));
						fCommPlaySound(m_pConfigReadWrite->fGetSoundFile(SOUND_FILE_GetData));
						SetTimer(OBDPND_TIMER_START_COMM,10,NULL);
					}
					else
					{
						fAppExit();
					}
					return;
				}
			}

			SetTimer(OBDPND_TIMER_REFRESH_UI_BEGIN,100,NULL);
			m_bVaildComm = TRUE;
			
			//�ж��Զ�����
			SetTimer(OBDPND_TIMER_AUTO_START_APP,100,NULL);
		}
		break;
	case OBDPND_TIMER_AUTO_START_APP:
		{
			KillTimer(OBDPND_TIMER_AUTO_START_APP);

			vector<TagAppInfo> vecStartApp = m_pConfigReadWrite->fGetStartList();
			for (int i=0;i<vecStartApp.size();i++)
			{
				if (vecStartApp[i].bAutoRun)
				{
					fAddLog(_T("�Զ�����:")+vecStartApp[i].strAppPath);
					fCommRunProcess(vecStartApp[i].strAppPath);
				}
			}

		}
		break;
	case OBDPND_TIMER_REFRESH_UI_BEGIN:
		{
			if (m_bVaildComm)
				fHideTipInfo();
			KillTimer(OBDPND_TIMER_REFRESH_UI_BEGIN);

			SetTimer(OBDPND_TIMER_REFRESH_UI,500,NULL);
		}
		break;
	case OBDPND_TIMER_REFRESH_UI:
		{
			fRefreshUiData();
		}
		break;
	case OBDPND_TIMER_AUTO_SEL_PANEL:
		{
			KillTimer(OBDPND_TIMER_AUTO_SEL_PANEL);

			DWORD dwCurrSpeend = _ttoi(m_pCommunication->fGetDataStreamValue(0x00000D00));
			if ( dwCurrSpeend> 0)	//��ǰ���ٴ�����
			{	
				if ( m_wCurrPanelIndex == OBDPND_PANEL_TOUR || m_wCurrPanelIndex == OBDPND_PANEL_RACE)
				{
					m_wBeforPanelIndex = m_wCurrPanelIndex;					
				}
				else
				{
					if (m_wBeforPanelIndex == OBDPND_PANEL_RACE )
						fSetCurrPanelIndex(OBDPND_PANEL_RACE);
					else
						fSetCurrPanelIndex(OBDPND_PANEL_TOUR);	
				}
			}
			else			//����Ϊ��
			{
				if(m_wCurrPanelIndex != OBDPND_PANEL_IDLE)		//ת�ص���		
					fSetCurrPanelIndex(OBDPND_PANEL_IDLE);			
			}

			SetTimer(OBDPND_TIMER_AUTO_SEL_PANEL,m_pConfigReadWrite->fGetAutoSelectPanelTime()*1000,NULL);
		}
		break;
	case OBDPND_TIMER_EXIT:
		{
			KillTimer(OBDPND_TIMER_EXIT);

			fSaveAppData();

			exit(0);
		}
		break;
	case OBDPND_TIMER_ALARM_FATIGUE_DRIVING:
		{
			SetTimer(OBDPND_TIMER_ALARM_FATIGUE_DRIVING_STAY,15*1000,NULL);
			m_pDlgAlarmFatigueDriving->fShowTipText();
		}
		break;
	case OBDPND_TIMER_ALARM_FATIGUE_DRIVING_STAY:
		{
			KillTimer(OBDPND_TIMER_ALARM_FATIGUE_DRIVING_STAY);
			m_pDlgAlarmFatigueDriving->fHideTipText();
		}
		break;
	case OBDPND_TIMER_ALARM_BATTERY_VOLTAGE:
		{
			SetTimer(OBDPND_TIMER_ALARM_BATTERY_VOLTAGE_STAY,15*1000,NULL);
			m_pDlgAlarmBatteryVoltage->fShowTipText();
		}
		break;
	case OBDPND_TIMER_ALARM_BATTERY_VOLTAGE_STAY:
		{
			KillTimer(OBDPND_TIMER_ALARM_BATTERY_VOLTAGE_STAY);
			m_pDlgAlarmBatteryVoltage->fHideTipText();
		}
		break;
	case OBDPND_TIMER_CHECK_GPS_RUNNING:
		{
			if (!fCommFindProcess(m_strGpsExe))
			{
				KillTimer(OBDPND_TIMER_CHECK_GPS_RUNNING);
				OnMiniHide(0,0);
			}
		}
		break;
	case OBDPND_TIMER_CHECK_BACKLIGHT:
		{
			//���ý��治ˢ��
			if (!m_bSetting)
			{
				//�������������6-18��ҹ��18-6��
				CTime time = CTime::GetCurrentTime();
				DWORD dwNow = time.GetHour()*60+time.GetMinute();
				if (dwNow>m_pConfigReadWrite->fGetBacklightDayStart() && dwNow<m_pConfigReadWrite->fGetBacklightDayEnd())
				{
					if (!m_bIsDay)
					{
						m_bIsDay = TRUE;
						COBDPNDDlgBacklight backlight;
						backlight.fSetBackLight(m_pConfigReadWrite->fGetBacklightDay());
						fAddLog(_T("����ģʽ"));
					}
				}
				else
				{
					if (m_bIsDay)
					{
						m_bIsDay = FALSE;
						COBDPNDDlgBacklight backlight;
						backlight.fSetBackLight(m_pConfigReadWrite->fGetBacklightNight());
						fAddLog(_T("ҹ��ģʽ"));
					}				
				}
			}
		}
		break;
	}

	CDialog::OnTimer(nIDEvent);
}


BOOL COBD_PNDDlg::fIsBackRunning()
{
	return m_bBackRunning;
}
void COBD_PNDDlg::fRefreshUiData()
{
	//����ȡ���ݳ�ʱ
	m_pCommunication->fCheckOverTime();

	// ��̨���� �����ý��� ��ˢ������UI����
	if (!m_bBackRunning && !m_bSetting)
	{
		switch (m_wCurrPanelIndex)
		{
		case OBDPND_PANEL_IDLE:
			m_pDlgMainIdle->fRefreshData();
			break;
		case OBDPND_PANEL_TOUR:
			m_pDlgMainTour->fRefreshData();
			break;
		case OBDPND_PANEL_RACE:
			m_pDlgMainRace->fRefreshData();
			break;
		case OBDPND_PANEL_DETAIL:
			m_pDlgMainDetail->fRefreshData();
			break;
		}
	}

	// ���ý��� ����鱨��
	if (!m_bSetting)
	{
		//���ٱ��� һֱ��ʾ
		if (m_pCommunication->fGetDataStreamValueF(0x00000D00)!= INVALID_DATASTREAM_VALUE && m_pCommunication->fGetDataStreamValueF(0x00000D00) > m_pConfigReadWrite->fGetOverSpeedAlarmValue() )
		{
			if (!m_mapAlarmStates[OBDPND_TIMER_ALARM_OVERSPEED])
			{
				m_mapAlarmStates[OBDPND_TIMER_ALARM_OVERSPEED] = TRUE;
				m_pDlgAlarmOverSpeed->fShowTipText();
			}
		}
		else
		{
			if (m_mapAlarmStates[OBDPND_TIMER_ALARM_OVERSPEED])
			{
				m_mapAlarmStates[OBDPND_TIMER_ALARM_OVERSPEED] = FALSE;
				m_pDlgAlarmOverSpeed->fHideTipText();
			}
		}

		//ˮ�� һֱ��ʾ
		if (m_pCommunication->fGetDataStreamValueF(0x00000500)!= INVALID_DATASTREAM_VALUE && m_pCommunication->fGetDataStreamValueF(0x00000500) > m_pConfigReadWrite->fGetWaterTemperatureAlarmValue() )
		{
			if (!m_mapAlarmStates[OBDPND_TIMER_ALARM_COOLANT])
			{
				m_mapAlarmStates[OBDPND_TIMER_ALARM_COOLANT] = TRUE;
				m_pDlgAlarmCoolant->fShowTipText();
			}
		}
		else
		{
			if (m_mapAlarmStates[OBDPND_TIMER_ALARM_COOLANT])
			{
				m_mapAlarmStates[OBDPND_TIMER_ALARM_COOLANT] = FALSE;
				m_pDlgAlarmCoolant->fHideTipText();
			}
		}

		//ƣ�ͼ�ʻ���� ���ʱ��10��������,��ʱ ÿ��ͣ��15��
		if (m_pCommunication->fGetDataStreamValueF(0xFF010007)!= INVALID_DATASTREAM_VALUE && (m_pCommunication->fGetDataStreamValueF(0xFF010007)/60.0) > m_pConfigReadWrite->fGetFatigueDrivingAlarmValue() )
		{
			if (!m_mapAlarmStates[OBDPND_TIMER_ALARM_FATIGUE_DRIVING])
			{
				m_mapAlarmStates[OBDPND_TIMER_ALARM_FATIGUE_DRIVING] = TRUE;
				m_pDlgAlarmFatigueDriving->fShowTipText();
				
				SetTimer(OBDPND_TIMER_ALARM_FATIGUE_DRIVING,10*60*1000,NULL);
				SetTimer(OBDPND_TIMER_ALARM_FATIGUE_DRIVING_STAY,15*1000,NULL);
			}
		}
		else
		{
			if (m_mapAlarmStates[OBDPND_TIMER_ALARM_FATIGUE_DRIVING])
			{
				m_mapAlarmStates[OBDPND_TIMER_ALARM_FATIGUE_DRIVING] = FALSE;
				m_pDlgAlarmFatigueDriving->fHideTipText();
				KillTimer(OBDPND_TIMER_ALARM_FATIGUE_DRIVING);
				KillTimer(OBDPND_TIMER_ALARM_FATIGUE_DRIVING_STAY);
			}
		}
		//��ƿ��ѹ ���ʱ��10��������,��ʱ ÿ��ͣ��15��
		float fCurrVolt= m_pCommunication->fGetDataStreamValueF(0xFF01000B);
		float fMaxVolt = m_pConfigReadWrite->fGetVoltMaxAlarm()/10.0;
		float fMinVolt = m_pConfigReadWrite->fGetVoltMinAlarm()/10.0;
		if (fCurrVolt!= INVALID_DATASTREAM_VALUE && (fCurrVolt > fMaxVolt || fCurrVolt < fMinVolt))
		{
			if (fCurrVolt > fMaxVolt)
			{
				m_pDlgAlarmBatteryVoltage->fSetTipText(fGetBinText(DS_ALARM_VOLT_HIGH));
				m_pDlgAlarmBatteryVoltage->fSetSoundFile(m_pConfigReadWrite->fGetSoundFile(SOUND_FILE_AlarmVoltHigh));
			}
			if (fCurrVolt < fMinVolt)
			{
				m_pDlgAlarmBatteryVoltage->fSetTipText(fGetBinText(DS_ALARM_VOLT_LOW));
				m_pDlgAlarmBatteryVoltage->fSetSoundFile(m_pConfigReadWrite->fGetSoundFile(SOUND_FILE_AlarmVoltLow));
			}

			if (!m_mapAlarmStates[OBDPND_TIMER_ALARM_BATTERY_VOLTAGE])
			{
				m_mapAlarmStates[OBDPND_TIMER_ALARM_BATTERY_VOLTAGE] = TRUE;
				m_pDlgAlarmBatteryVoltage->fShowTipText();

				SetTimer(OBDPND_TIMER_ALARM_BATTERY_VOLTAGE,10*60*1000,NULL);
				SetTimer(OBDPND_TIMER_ALARM_BATTERY_VOLTAGE_STAY,15*1000,NULL);
			}
		}
		else
		{
			if (m_mapAlarmStates[OBDPND_TIMER_ALARM_BATTERY_VOLTAGE])
			{
				m_mapAlarmStates[OBDPND_TIMER_ALARM_BATTERY_VOLTAGE] = FALSE;
				m_pDlgAlarmBatteryVoltage->fHideTipText();
				KillTimer(OBDPND_TIMER_ALARM_BATTERY_VOLTAGE);
				KillTimer(OBDPND_TIMER_ALARM_BATTERY_VOLTAGE_STAY);
			}
		}
	}

	//// ������������־
	//if (m_pConfigReadWrite->fGetSaveErrLogStatus())
	//{
	//	CString str;
	//	for (map<DWORD,TagDataStreamValue>::iterator it = m_mapDataStreamValue.begin();it!=m_mapDataStreamValue.end();it++)
	//	{
	//		str.Format(_T("%08X = %0.7f = %s = %s"),it->first,it->second.fValue,it->second.strValue,it->second.strText);
	//		fAddLog(str);
	//	}
	//	fAddLog(_T(""));
	//}

}

LONG COBD_PNDDlg::OnMiniShow(WPARAM wParam,LPARAM lParam)
{	
	this->ShowWindow(SW_HIDE);
	m_pDlgMini->fShowDlg(TRUE);
	m_bBackRunning = TRUE;

	return 0;
}
LONG COBD_PNDDlg::OnMiniHide(WPARAM wParam,LPARAM lParam)
{	
	m_pDlgMini->ShowWindow(SW_HIDE);
	this->ShowWindow(SW_SHOW);
	//this->BringWindowToTop();
	::SetWindowPos(GetSafeHwnd(),HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);

	m_bBackRunning = FALSE;

	return 0;
}

LONG COBD_PNDDlg::OnSerialRestart(WPARAM wParam,LPARAM lParam)
{
	m_pCommunication->fStopSerial();
	fAddLog(_T("��ȡ���ڳ�ʱ����"));
	m_pCommunication->fStartSeries(m_pConfigReadWrite->fGetOBDComPortNo());

	return TRUE;
}
// ���ڽ������ݴ�����
LONG COBD_PNDDlg::OnSerialRecvData(WPARAM wParam,LPARAM lParam)
{
	//����������־
	if (m_pConfigReadWrite->fGetSaveErrLogStatus())
	{
		BYTE * pBufData = (BYTE *)wParam;
		DWORD dwBufLen = lParam;
		CString strText;
		CString strTemp;
		strText.Format(_T("[R][%d]%d="),GetTickCount(),dwBufLen);
		for (DWORD i=0;i<dwBufLen;i++)
		{
			strTemp.Format(_T("%02X,"),pBufData[i]);
			strText = strText + strTemp;
		}
		fAddLog(strText,LOG_TYPE_GPS_DATA);
	}

	return 0;
}

// COBD_PNDDlg ��Ϣ�������

BOOL COBD_PNDDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	theMainDlg = this;
	this->SetWindowText(_T("OBD_PND"));

	// ��ʼ��Ӧ�ó��������ļ�
	if (m_pConfigReadWrite == NULL)
	{
		m_pConfigReadWrite = new COBDPNDConfigReadWrite();
		if(!m_pConfigReadWrite->fInitAppConfig())
		{
			AfxMessageBox(_T("��ʼ��APP�����ļ���Ч!"));
			CDialog::OnCancel();
		}
	}	

	// ��ʼ��ͨѶģ��
	if (m_pCommunication == NULL)
	{
		m_pCommunication = new COBDPNDCommunication();
	}	

	//IDB_BITMAP1����ӵ�λͼ��Դ��
	m_pSplashWindow = new CSplashWnd();
	m_pSplashWindow->CreateSplash(); 
	m_pSplashWindow->CenterWindow(); 
	m_pSplashWindow->ShowWindow(SW_SHOW);
	m_pSplashWindow->UpdateWindow(); 

	// Ч��Logo�ļ�
	if(!m_pSplashWindow->CheckBMP())
	{
		AfxMessageBox(_T("��Դ�ļ�������ȷ�ϣ�"));
		m_pSplashWindow->DestroyWindow();
		CDialog::OnCancel();

		return FALSE;
	}
	// ��ʼ������������Ϣ
	fInitUiSetting();

	m_pDlgLog = new COBDPNDDlgLog();
	m_pDlgLog->Create(IDD_OBD_PND_DIALOG_LOG,this);
	m_pDlgLog->ShowWindow(SW_HIDE);

	m_pDlgTip = new COBDPNDDlgTipInfo();
	m_pDlgTip->Create(IDD_OBD_PND_DIALOG_CHILD,this);
	m_pDlgTip->ShowWindow(SW_HIDE);


	//��������
	m_pDlgAlarmOverSpeed = new COBDPNDDlgTipInfo();
	m_pDlgAlarmOverSpeed->Create(IDD_OBD_PND_DIALOG,this); 
	m_pDlgAlarmOverSpeed->ShowWindow(SW_HIDE);
	m_pDlgAlarmOverSpeed->fSetTipText(fGetBinText(DS_ALARM_OVERSPEED));
	m_pDlgAlarmOverSpeed->fSetSoundFile(m_pConfigReadWrite->fGetSoundFile(SOUND_FILE_AlarmOverSpeed));

	m_pDlgAlarmCoolant = new COBDPNDDlgTipInfo();
	m_pDlgAlarmCoolant->Create(IDD_OBD_PND_DIALOG,this);
	m_pDlgAlarmCoolant->ShowWindow(SW_HIDE);
	m_pDlgAlarmCoolant->fSetTipText(fGetBinText(DS_ALARM_WATER_TEMP));
	m_pDlgAlarmCoolant->fSetSoundFile(m_pConfigReadWrite->fGetSoundFile(SOUND_FILE_AlarmCoolant));

	m_pDlgAlarmFatigueDriving = new COBDPNDDlgTipInfo();
	m_pDlgAlarmFatigueDriving->Create(IDD_OBD_PND_DIALOG,this);
	m_pDlgAlarmFatigueDriving->ShowWindow(SW_HIDE);
	m_pDlgAlarmFatigueDriving->fSetTipText(fGetBinText(DS_ALARM_FATIGUE_DRIVING));
	m_pDlgAlarmFatigueDriving->fSetSoundFile(m_pConfigReadWrite->fGetSoundFile(SOUND_FILE_AlarmFatigueDriving));

	m_pDlgAlarmBatteryVoltage= new COBDPNDDlgTipInfo();
	m_pDlgAlarmBatteryVoltage->Create(IDD_OBD_PND_DIALOG,this);
	m_pDlgAlarmBatteryVoltage->ShowWindow(SW_HIDE);
	m_pDlgAlarmBatteryVoltage->fSetTipText(fGetBinText(DS_ALARM_VOLT_LOW));
	m_pDlgAlarmBatteryVoltage->fSetSoundFile(m_pConfigReadWrite->fGetSoundFile(SOUND_FILE_AlarmVoltLow));


	////��������
	//CTime time = CTime::GetCurrentTime();
	//DWORD dwNow = time.GetHour()*60+time.GetMinute();
	//if (dwNow>m_pConfigReadWrite->fGetBacklightDayStart() && dwNow<m_pConfigReadWrite->fGetBacklightDayEnd())
	//{
	//	COBDPNDDlgBacklight backlight;
	//	backlight.fSetBackLight(m_pConfigReadWrite->fGetBacklightDay());
	//	fAddLog(_T("����ģʽ"));		
	//	m_bIsDay = TRUE;
	//}
	//else
	//{
	//	COBDPNDDlgBacklight backlight;
	//	backlight.fSetBackLight(m_pConfigReadWrite->fGetBacklightNight());
	//	fAddLog(_T("ҹ��ģʽ"));
	//	m_bIsDay = FALSE;
	//}
	//SetTimer(OBDPND_TIMER_CHECK_BACKLIGHT,1000,NULL);

	m_pSeriesGps = new CSeriesGPS();
	if (m_pSeriesGps->OpenPort(this->m_hWnd,
		m_pConfigReadWrite->fGetGPSComPortNo(),
		m_pConfigReadWrite->fGetGPSComBaun(),
		m_pConfigReadWrite->fGetGPSComParity(),
		m_pConfigReadWrite->fGetGPSComDataBits(),
		m_pConfigReadWrite->fGetGPSComStopBits()))
	{
		fAddLog(_T("��GPS���ݴ��ڳɹ�!"));
	}
	else
	{
		fAddLog(_T("��GPS���ݴ���ʧ��!"));
	}


	m_pDlgMainIdle = new COBDPNDDlgMainIdle();
	m_pDlgMainIdle->Create(IDD_OBD_PND_DIALOG_CHILD,this);
	m_pDlgMainIdle->ShowWindow(SW_HIDE);
	fAddLog(_T("��ʼ�����ٽ�����ɣ�"));

	m_pDlgMainTour = new COBDPNDDlgMainTour();
	m_pDlgMainTour->Create(IDD_OBD_PND_DIALOG_CHILD,this);
	m_pDlgMainTour->ShowWindow(SW_HIDE);
	fAddLog(_T("��ʼ��Ѳ��������ɣ�"));

	m_pDlgMainRace = new COBDPNDDlgMainRace();
	m_pDlgMainRace->Create(IDD_OBD_PND_DIALOG_CHILD,this); 
	m_pDlgMainRace->ShowWindow(SW_HIDE);
	fAddLog(_T("��ʼ������������ɣ�"));

	m_pDlgMainDetail = new COBDPNDDlgMainDetail();
	m_pDlgMainDetail->Create(IDD_OBD_PND_DIALOG_CHILD,this);
	m_pDlgMainDetail->ShowWindow(SW_HIDE);
	fAddLog(_T("��ʼ����ϸ������ɣ�"));

	m_pDlgMini = new COBDPNDDlgMini();
	m_pDlgMini->Create(IDD_OBD_PND_DIALOG);
	m_pDlgMini->ShowWindow(SW_HIDE);
	fAddLog(_T("��ʼ���������ݽ���"));

	//������
	if (m_pConfigReadWrite->fGetActiveLanguage() == _T(""))
	{
		m_pConfigReadWrite->fSetActiveLanguage(_T("CHS"));

		COBDPNDDlgSetupGuide dlgSetupGuide;
		dlgSetupGuide.DoModal(); 
	}

	fSetCurrPanelIndex(OBDPND_PANEL_IDLE);

	m_mapAlarmStates.insert(pair<DWORD,BOOL>(OBDPND_TIMER_ALARM_OVERSPEED,FALSE));
	m_mapAlarmStates.insert(pair<DWORD,BOOL>(OBDPND_TIMER_ALARM_COOLANT,FALSE));
	m_mapAlarmStates.insert(pair<DWORD,BOOL>(OBDPND_TIMER_ALARM_FATIGUE_DRIVING,FALSE));
	m_mapAlarmStates.insert(pair<DWORD,BOOL>(OBDPND_TIMER_ALARM_BATTERY_VOLTAGE,FALSE));


	fShowTipInfo(fGetBinText(DS_ECU_COM_INIT));
	fCommPlaySound(m_pConfigReadWrite->fGetSoundFile(SOUND_FILE_GetData));

	SetTimer(OBDPND_TIMER_START_COMM,100,NULL);

	m_pSplashWindow->DestroyWindow();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
void COBD_PNDDlg::OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/)
{
	if (AfxIsDRAEnabled())
	{
		DRA::RelayoutDialog(
			AfxGetResourceHandle(), 
			this->m_hWnd, 
			DRA::GetDisplayMode() != DRA::Portrait ? 
			MAKEINTRESOURCE(IDD_OBD_PND_DIALOG_WIDE) : 
			MAKEINTRESOURCE(IDD_OBD_PND_DIALOG));
	}
}
#endif



void COBD_PNDDlg::fInitUiSetting()
{
	//��ȡ�ֱ���
	int x = GetSystemMetrics(SM_CXSCREEN); //��Ļ��� 
	int y = GetSystemMetrics(SM_CYSCREEN); //��Ļ�߶�
	if (x == 480 && y == 272) 
		m_dwScreenResolution = RESOLUTION_480X272;
	else if (x == 800 && y == 480) 
		m_dwScreenResolution = RESOLUTION_800X480;

	//���ݷֱ��ʳ�ʼ�����ڶ���
	switch (m_dwScreenResolution)
	{
	case RESOLUTION_480X272:
		{
			m_rectWin = CRect(0,0,480,272);
			m_rectTitle = CRect(0,10,480,40);
			for (int i=0;i<MAX_FOOT_INDEX;i++)
			{
				m_rectFoot[i] = CRect(i*(480/MAX_FOOT_INDEX),242,(i+1)*(480/MAX_FOOT_INDEX),272);
			}
			VERIFY(m_fontTitle.CreateFont(
				30,                        // nHeight
				0,                         // nWidth
				0,                         // nEscapement
				0,                         // nOrientation
				FW_BOLD, //FW_BOLD,�Ӵ�        FW_NORMAL        // nWeight
				FALSE,                     // bItalic
				FALSE,                     // bUnderline
				0,                         // cStrikeOut
				ANSI_CHARSET,              // nCharSet
				OUT_DEFAULT_PRECIS,        // nOutPrecision
				CLIP_DEFAULT_PRECIS,       // nClipPrecision
				DEFAULT_QUALITY,           // nQuality
				DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
				_T("Arial Rounded MT")));              // lpszFacename

			VERIFY(m_fontFootNormal.CreateFont(
				23,                        // nHeight
				0,                         // nWidth
				0,                         // nEscapement
				0,                         // nOrientation
				FW_NORMAL, //FW_BOLD,�Ӵ�        FW_NORMAL        // nWeight
				FALSE,                     // bItalic
				FALSE,                     // bUnderline
				0,                         // cStrikeOut
				ANSI_CHARSET,              // nCharSet
				OUT_DEFAULT_PRECIS,        // nOutPrecision
				CLIP_DEFAULT_PRECIS,       // nClipPrecision
				DEFAULT_QUALITY,           // nQuality
				DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
				_T("Arial Rounded MT")));              // lpszFacename		

			VERIFY(m_fontFootFocus.CreateFont(
				23,                        // nHeight
				0,                         // nWidth
				0,                         // nEscapement
				0,                         // nOrientation
				FW_BOLD, //FW_BOLD,�Ӵ�        FW_NORMAL        // nWeight
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
	case RESOLUTION_800X480:
		{
			m_rectWin = CRect(0,0,800,480);
			m_rectTitle = CRect(0,25,800,70);
			for (int i=0;i<MAX_FOOT_INDEX;i++)
			{
				m_rectFoot[i] = CRect(i*(800/MAX_FOOT_INDEX),435,(i+1)*(800/MAX_FOOT_INDEX),480);
			}

			VERIFY(m_fontTitle.CreateFont(
				40,                        // nHeight
				0,                         // nWidth
				0,                         // nEscapement
				0,                         // nOrientation
				FW_BOLD, //FW_BOLD,�Ӵ�        FW_NORMAL        // nWeight
				FALSE,                     // bItalic
				FALSE,                     // bUnderline
				0,                         // cStrikeOut
				ANSI_CHARSET,              // nCharSet
				OUT_DEFAULT_PRECIS,        // nOutPrecision
				CLIP_DEFAULT_PRECIS,       // nClipPrecision
				DEFAULT_QUALITY,           // nQuality
				DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
				_T("Arial Rounded MT")));              // lpszFacename

			VERIFY(m_fontFootNormal.CreateFont(
				33,                        // nHeight
				0,                         // nWidth
				0,                         // nEscapement
				0,                         // nOrientation
				FW_NORMAL, //FW_BOLD,�Ӵ�        FW_NORMAL        // nWeight
				FALSE,                     // bItalic
				FALSE,                     // bUnderline
				0,                         // cStrikeOut
				ANSI_CHARSET,              // nCharSet
				OUT_DEFAULT_PRECIS,        // nOutPrecision
				CLIP_DEFAULT_PRECIS,       // nClipPrecision
				DEFAULT_QUALITY,           // nQuality
				DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
				_T("Arial Rounded MT")));              // lpszFacename

			VERIFY(m_fontFootFocus.CreateFont(
				33,                        // nHeight
				0,                         // nWidth
				0,                         // nEscapement
				0,                         // nOrientation
				FW_BOLD, //FW_BOLD,�Ӵ�        FW_NORMAL        // nWeight
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


	m_clrTitle = RGB(255,255,255);
	m_clrFootNormal = RGB(255,255,255);
	m_clrFootFocus = RGB(244,78,6);

	//��ʼ�������湦�ܰ�ť
	for (int i=0;i<MAX_FOOT_INDEX;i++)
	{
		m_strFoot[i] = fGetBinText(DS_MAIN_IDEL+i);
	}

	this->MoveWindow(m_rectWin);
	this->Invalidate();

}

CString COBD_PNDDlg::fGetBinText(DWORD dwID)
{
	CString strRet = fCommGetBinFileText(dwID,m_pConfigReadWrite->fGetTextBinFileName());
	return strRet;
}
DWORD COBD_PNDDlg::fGetUnitResID(CString strUnit)
{
	strUnit = strUnit.Left(strUnit.GetLength()-1);
	strUnit.Replace(_T("@"),_T(""));
	strUnit = strUnit.TrimRight();
	strUnit = strUnit.TrimLeft();
	strUnit = strUnit.MakeUpper();
	switch (m_dwScreenResolution)
	{
	case RESOLUTION_480X272:
		{
		}
		break;
	case RESOLUTION_800X480:
		{
			if (strUnit == _T("KPA"))
			{
				return IDR_PNG_UNIT_Kpa_800X480;
			}
			if (strUnit == _T("KM"))
			{
				return IDR_PNG_UNIT_KM_800X480;
			}
			if (strUnit == _T("KM/H"))
				return IDR_PNG_UNIT_KM_H_800X480;
			if (strUnit == _T("L/100KM"))
				return IDR_PNG_UNIT_L_100KM_800X480;
			if (strUnit == _T("L/H"))
				return IDR_PNG_UNIT_L_H_800X480;
			if (strUnit == _T("M"))
				return NULL;
			if (strUnit == _T("MS"))
				return IDR_PNG_UNIT_MS_800X480;
			if (strUnit == _T("%"))
				return IDR_PNG_UNIT_PERCENT_800X480;
			if (strUnit == _T("MV"))
				return IDR_PNG_UNIT_MV_800X480;
			if (strUnit == _T("V"))
				return IDR_PNG_UNIT_V_800X480;
			if (strUnit == _T("��"))
				return IDR_PNG_UNIT_C_800X480;
			if (strUnit == _T("��"))
				return IDR_PNG_UNIT_T_800X480;
			if (strUnit == _T("RPM"))
				return IDR_PNG_UNIT_RPM_800X480;
			if (strUnit == _T("$"))
				return IDR_PNG_UNIT_MONETARY_800X480;
			if (strUnit == _T("G/S"))
				return IDR_PNG_UNIT_G_S_800X480;
		}
	}

	return NULL;
}

int	COBD_PNDDlg::fSaveAppData()
{
	m_pConfigReadWrite->fSaveAppConfig();
	return TRUE;
}

void COBD_PNDDlg::fAppExit()
{
	fShowTipInfo(_T("�����˳����..."));
	SetTimer(OBDPND_TIMER_EXIT,100,NULL);
}

int COBD_PNDDlg::fAddLog(CString strText,WORD wType)
{
	m_pDlgLog->fAddLog(strText,wType);

	return TRUE;
}

// ��ʾ��ʾ��Ϣ
int COBD_PNDDlg::fShowTipInfo(CString strText,WORD wType)
{
	if (strText == _T(""))
	{
		m_pDlgTip->ShowWindow(SW_HIDE);
	}
	else
	{
		m_pDlgTip->fSetTipText(strText,wType);
		m_pDlgTip->ShowWindow(SW_SHOW);
	}

	return TRUE;
}
int COBD_PNDDlg::fHideTipInfo()
{
	m_pDlgTip->ShowWindow(SW_HIDE);

	return TRUE;
}

WORD COBD_PNDDlg::fGetCurrPanelIndex()
{
	return m_wCurrPanelIndex;
}
WORD COBD_PNDDlg::fGetBeforPanelIndex()
{
	return m_wBeforPanelIndex;
}

void COBD_PNDDlg::fSetBeforPanelIndex()
{
	fSetCurrPanelIndex(m_wBeforPanelIndex);
	m_bShowLog = FALSE;
}
void COBD_PNDDlg::fReturnCurrPanelIndex()
{
	fSetCurrPanelIndex(m_wCurrPanelIndex);
	m_bShowLog = FALSE;
}

void COBD_PNDDlg::fSetCurrPanelIndex(WORD wIndex)
{
	switch (wIndex)
	{
	case OBDPND_PANEL_LOGO:
		{
		}
		break;
	case OBDPND_PANEL_IDLE:	//����
		{
			if(m_wCurrPanelIndex != wIndex)
			{
				m_wBeforPanelIndex = m_wCurrPanelIndex;
				m_wCurrPanelIndex = wIndex;

				//���ý�����Զ��л��ض�Ӧ״̬��ʱ����
				KillTimer(OBDPND_TIMER_AUTO_SEL_PANEL);
				SetTimer(OBDPND_TIMER_AUTO_SEL_PANEL,m_pConfigReadWrite->fGetAutoSelectPanelTime()*1000,NULL);	
			}
			if (m_pDlgMainIdle == NULL)
			{
				m_pDlgMainIdle = new COBDPNDDlgMainIdle();
				m_pDlgMainIdle->Create(IDD_OBD_PND_DIALOG,this);				
			}
			m_pDlgMainIdle->ShowWindow(SW_SHOW);
			m_pDlgMainTour->ShowWindow(SW_HIDE);
			m_pDlgMainRace->ShowWindow(SW_HIDE);
			m_pDlgMainDetail->ShowWindow(SW_HIDE);

			m_pDlgLog->ShowWindow(SW_HIDE);
		}
		break;
	case OBDPND_PANEL_TOUR: //Ѳ��
		{
			if(m_wCurrPanelIndex != wIndex)
			{
				m_wBeforPanelIndex = m_wCurrPanelIndex;
				m_wCurrPanelIndex = wIndex;
				//���ý�����Զ��л��ض�Ӧ״̬��ʱ����
				KillTimer(OBDPND_TIMER_AUTO_SEL_PANEL);
				SetTimer(OBDPND_TIMER_AUTO_SEL_PANEL,m_pConfigReadWrite->fGetAutoSelectPanelTime()*1000,NULL);	
			}
			if (m_pDlgMainTour == NULL)
			{
				m_pDlgMainTour = new COBDPNDDlgMainTour();
				m_pDlgMainTour->Create(IDD_OBD_PND_DIALOG,this);
				fAddLog(_T("��ʼ��Ѳ��������ɣ�"));
			}
			m_pDlgMainIdle->ShowWindow(SW_HIDE);
			m_pDlgMainTour->ShowWindow(SW_SHOW);
			m_pDlgMainRace->ShowWindow(SW_HIDE);
			m_pDlgMainDetail->ShowWindow(SW_HIDE);

			m_pDlgLog->ShowWindow(SW_HIDE);
		}
		break; 
	case OBDPND_PANEL_RACE:	//����
		{
			if(m_wCurrPanelIndex != wIndex)
			{
				m_wBeforPanelIndex = m_wCurrPanelIndex;
				m_wCurrPanelIndex = wIndex;
				//���ý�����Զ��л��ض�Ӧ״̬��ʱ����
				KillTimer(OBDPND_TIMER_AUTO_SEL_PANEL);
				SetTimer(OBDPND_TIMER_AUTO_SEL_PANEL,m_pConfigReadWrite->fGetAutoSelectPanelTime()*1000,NULL);	
			}
			if (m_pDlgMainRace == NULL)
			{
				m_pDlgMainRace = new COBDPNDDlgMainRace();
				m_pDlgMainRace->Create(IDD_OBD_PND_DIALOG,this);	
				fAddLog(_T("��ʼ������������ɣ�"));			
			}
			m_pDlgMainIdle->ShowWindow(SW_HIDE);
			m_pDlgMainTour->ShowWindow(SW_HIDE);
			m_pDlgMainRace->ShowWindow(SW_SHOW);
			m_pDlgMainDetail->ShowWindow(SW_HIDE);

			m_pDlgLog->ShowWindow(SW_HIDE);
		}
		break;
	case OBDPND_PANEL_DETAIL:	//��ϸ
		{
			if(m_wCurrPanelIndex != wIndex)
			{
				m_wBeforPanelIndex = m_wCurrPanelIndex;
				m_wCurrPanelIndex = wIndex;
				//���ý�����Զ��л��ض�Ӧ״̬��ʱ����
				KillTimer(OBDPND_TIMER_AUTO_SEL_PANEL);
				SetTimer(OBDPND_TIMER_AUTO_SEL_PANEL,m_pConfigReadWrite->fGetAutoSelectPanelTime()*1000,NULL);	
			}
			if (m_pDlgMainDetail == NULL)
			{
				m_pDlgMainDetail = new COBDPNDDlgMainDetail();
				m_pDlgMainDetail->Create(IDD_OBD_PND_DIALOG,this);		
				fAddLog(_T("��ʼ����ϸ������ɣ�"));		
			}
			m_pDlgMainIdle->ShowWindow(SW_HIDE);
			m_pDlgMainTour->ShowWindow(SW_HIDE);
			m_pDlgMainRace->ShowWindow(SW_HIDE);
			m_pDlgMainDetail->ShowWindow(SW_SHOW);

			m_pDlgLog->ShowWindow(SW_HIDE);
		}
		break;
	case OBDPND_PANEL_SET:	//����
		{
			m_bSetting = TRUE;

			COBDPNDDlgConfigMain dlgConfigMain;
			dlgConfigMain.DoModal();

 
			m_bSetting = FALSE;
		}
		break;
	case OBDPND_PANEL_BACK:	//����
		{
			CString strAppPath = m_pConfigReadWrite->fGetStartList()[1].strAppPath;
			if(fCommRunProcess(strAppPath))
			{
				OnMiniShow(0,0);
				m_strGpsExe = strAppPath.Right(strAppPath.GetLength() - strAppPath.ReverseFind('\\') - 1);
				SetTimer(OBDPND_TIMER_CHECK_GPS_RUNNING,1000,NULL);
			}
		}
		break;
	case OBDPND_PANEL_EXIT: //�˳�
		{
			fAppExit();
		}
		break;
	case OBDPND_PANEL_LOG:
		{
			if (m_pDlgLog == NULL)
			{
				m_pDlgLog = new COBDPNDDlgLog();
				m_pDlgLog->Create(IDD_OBD_PND_DIALOG,this);				
			}
			m_pConfigReadWrite->fSetSaveErrLogStatus(TRUE);
			m_pDlgMainIdle->ShowWindow(SW_HIDE);
			m_pDlgMainTour->ShowWindow(SW_HIDE);
			m_pDlgMainRace->ShowWindow(SW_HIDE);
			m_pDlgMainDetail->ShowWindow(SW_HIDE);

			m_pDlgLog->ShowWindow(SW_SHOW);
			m_pDlgLog->fSetShowLog(TRUE);
			
			m_bShowLog = TRUE;
		}
		break;
	}
}