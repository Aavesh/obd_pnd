#include "StdAfx.h"
#include "OBDPNDConfigReadWrite.h"
#include "Markup.h"
#include "CeFileFind.h"
#include "OBD_PNDDlg.h"

COBDPNDConfigReadWrite::COBDPNDConfigReadWrite(void)
{
}

COBDPNDConfigReadWrite::~COBDPNDConfigReadWrite(void)
{
}

// 初始化应用配置信息
BOOL COBDPNDConfigReadWrite::fInitAppConfig(void)
{
	CMarkup xml;
	CString strAppPath = fCommGetAppPath() + _T("\\");

	CString strAppConfigFile = strAppPath + _T("Config.xml");
	if (!xml.Load(strAppConfigFile))
	{		
		return CONFIG_ERR_LOAD_FILE; 
	}

	xml.ResetMainPos();
	xml.FindElem();	

	if(xml.FindChildElem(_T("System")))
	{		
		xml.IntoElem();	
		if(xml.FindChildElem(_T("Base")))
		{			
			m_tagAppConfig.bIsDemo			= (xml.GetChildAttrib(_T("Demo_Mode"))==_T("ON")?TRUE:FALSE);
			m_tagAppConfig.strTextFile		= xml.GetChildAttrib(_T("File_Text"));
			m_tagAppConfig.strFilterAppList = xml.GetChildAttrib(_T("FILTER_APP_LIST"));
		}
		if(xml.FindChildElem(_T("COM_OBD")))
		{			
			m_tagAppConfig.tagComOBD.wPort = (WORD)_ttoi(xml.GetChildAttrib(_T("Port")));
			m_tagAppConfig.tagComOBD.dwBPS =(WORD) _ttoi(xml.GetChildAttrib(_T("BPS")));
			m_tagAppConfig.tagComOBD.wData_Bits = (WORD)_ttoi(xml.GetChildAttrib(_T("Data_Bits")));
			m_tagAppConfig.tagComOBD.wStop_Bits = (WORD)_ttoi(xml.GetChildAttrib(_T("Stop_Bits")));
			m_tagAppConfig.tagComOBD.wParity = 0;//_ttoi(xml.GetChildAttrib(_T("Parity")));
		}
		if(xml.FindChildElem(_T("COM_GPS")))
		{			
			m_tagAppConfig.tagComGPS.wPort = (WORD)_ttoi(xml.GetChildAttrib(_T("Port")));
			m_tagAppConfig.tagComGPS.dwBPS = (WORD)_ttoi(xml.GetChildAttrib(_T("BPS")));
			m_tagAppConfig.tagComGPS.wData_Bits = (WORD)_ttoi(xml.GetChildAttrib(_T("Data_Bits")));
			m_tagAppConfig.tagComGPS.wStop_Bits = (WORD)_ttoi(xml.GetChildAttrib(_T("Stop_Bits")));
			m_tagAppConfig.tagComGPS.wParity = 0;//_ttoi(xml.GetChildAttrib(_T("Parity")));
		}
		if(xml.FindChildElem(_T("Setting")))
		{			
			m_tagAppConfig.tagSetting.strLanguage			= xml.GetChildAttrib(_T("LANGUAGE"));
			m_tagAppConfig.tagSetting.bSaveErrorLog			= (xml.GetChildAttrib(_T("Save_ErrorLog"))==_T("OFF")?FALSE:TRUE);
			m_tagAppConfig.tagSetting.wBacklightDay			= _ttoi(xml.GetChildAttrib(_T("BACKLIGHT_DAY")));
			m_tagAppConfig.tagSetting.wBacklightNight		= _ttoi(xml.GetChildAttrib(_T("BACKLIGHT_NIGHT")));
			m_tagAppConfig.tagSetting.wBacklightDayStart	= _ttoi(xml.GetChildAttrib(_T("BACKLIGHT_DAY_START")));
			m_tagAppConfig.tagSetting.wBackLightDayEnd		= _ttoi(xml.GetChildAttrib(_T("BACKLIGHT_DAY_END")));
			m_tagAppConfig.tagSetting.dwActiveEcuID			= _ttoi(xml.GetChildAttrib(_T("Active_EUC_ID")));
			m_tagAppConfig.tagSetting.str24or12				= xml.GetChildAttrib(_T("Time_24_12Hour"));
			m_tagAppConfig.tagSetting.dwSystemOffTime		= _ttoi(xml.GetChildAttrib(_T("System_Off_Time")));
			m_tagAppConfig.tagSetting.dwSystemOffMode		= _ttoi(xml.GetChildAttrib(_T("System_Off_Mode")));
			m_tagAppConfig.tagSetting.dwAutoSelPanelTime	= _ttoi(xml.GetChildAttrib(_T("Auto_Sel_Panel_Time")));
			m_tagAppConfig.tagSetting.fAvgFuelParameter		= wcstod(xml.GetChildAttrib(_T("Parameter_Fuel_Avg")),0);
			m_tagAppConfig.tagSetting.fInstantFuelParameter	= wcstod(xml.GetChildAttrib(_T("Parameter_Fuel_Instant")),0);
			m_tagAppConfig.tagSetting.fFuelPrice			= wcstod(xml.GetChildAttrib(_T("Fuel_Price")),0);
			m_tagAppConfig.tagSetting.dwActiveEcuID			= _ttoi(xml.GetChildAttrib(_T("Active_ECU_ID")));
		}
		if(xml.FindChildElem(_T("Language_list")))
		{
			xml.IntoElem(); 

			m_tagAppConfig.vecLanguage.clear();
			while(xml.FindChildElem(_T("LANGUAGE")))
			{	
				_TagLanguage tagLanData;
				tagLanData.strLanID		= xml.GetChildAttrib(_T("LANGUAGE_ID"));
				tagLanData.strLanName	= xml.GetChildAttrib(_T("LANGUAGE_NAME"));
				tagLanData.strLanSN		= xml.GetChildAttrib(_T("LANGUAGE_SN"));
				tagLanData.strResFiles	= xml.GetChildAttrib(_T("LANGUAGE_RES"));

				m_tagAppConfig.vecLanguage.push_back(tagLanData);
			}

			xml.OutOfElem();
		}
		if(xml.FindChildElem(_T("Set_Alarm")))
		{		
			m_tagAppConfig.tagAlarmSet.bTroubleCode				= xml.GetChildAttrib(_T("TroubleCode"))==_T("ON")?TRUE:FALSE;
			m_tagAppConfig.tagAlarmSet.wSpeed					= _ttoi(xml.GetChildAttrib(_T("Speed")));
			m_tagAppConfig.tagAlarmSet.wCoolant					= _ttoi(xml.GetChildAttrib(_T("Coolant")));
			m_tagAppConfig.tagAlarmSet.wFatigue_Driving			= _ttoi(xml.GetChildAttrib(_T("Fatigue_Driving")));
			m_tagAppConfig.tagAlarmSet.dbFuel					= wcstod(xml.GetChildAttrib(_T("Fuel")),0);

			xml.IntoElem();
			if(xml.FindChildElem(_T("Shift")))
			{
				m_tagAppConfig.tagAlarmSet.wShiftRpm				= _ttoi(xml.GetChildAttrib(_T("Shift_Rpm")));
				m_tagAppConfig.tagAlarmSet.wShiftSpeed			= _ttoi(xml.GetChildAttrib(_T("Shift_Speed")));
			}
			if(xml.FindChildElem(_T("Volt")))
			{
				m_tagAppConfig.tagAlarmSet.wVoltMin				= _ttoi(xml.GetChildAttrib(_T("Volt_Min")));
				m_tagAppConfig.tagAlarmSet.wVoltMax				= _ttoi(xml.GetChildAttrib(_T("Volt_Max")));
			}

			xml.OutOfElem();
		}
		xml.OutOfElem();
	}

	if(xml.FindChildElem(_T("Mini_Dlg")))
	{			
		m_tagAppConfig.tagMiniSpeed.dw480X272_X			= _ttoi(xml.GetChildAttrib(_T("X_480X272")));	
		m_tagAppConfig.tagMiniSpeed.dw480X272_Y			= _ttoi(xml.GetChildAttrib(_T("Y_480X272")));		
		m_tagAppConfig.tagMiniSpeed.dw800X480_X			= _ttoi(xml.GetChildAttrib(_T("X_800X480")));		
		m_tagAppConfig.tagMiniSpeed.dw800X480_Y			= _ttoi(xml.GetChildAttrib(_T("Y_800X480")));				
	}

	if(xml.FindChildElem(_T("UiData")))
	{		
		xml.IntoElem();	
		if(xml.FindChildElem(_T("Idle")))
		{	
			m_tagAppConfig.tagUiDataIdle.dwLeftUpID			= fCommStr2Hex(xml.GetChildAttrib(_T("ID_Left_Up")));
			m_tagAppConfig.tagUiDataIdle.dwLeftMiddleID		= fCommStr2Hex(xml.GetChildAttrib(_T("ID_Left_Middle")));
			m_tagAppConfig.tagUiDataIdle.dwLeftDownID		= fCommStr2Hex(xml.GetChildAttrib(_T("ID_Left_Down")));
			m_tagAppConfig.tagUiDataIdle.dwRightUpID		= fCommStr2Hex(xml.GetChildAttrib(_T("ID_Right_Up")));
			m_tagAppConfig.tagUiDataIdle.dwRightMiddleID	= fCommStr2Hex(xml.GetChildAttrib(_T("ID_Right_Middle")));
			m_tagAppConfig.tagUiDataIdle.dwRightDownID		= fCommStr2Hex(xml.GetChildAttrib(_T("ID_Right_Down")));
		}

		if(xml.FindChildElem(_T("Tour")))
		{		
			m_tagAppConfig.tagUiDataTour.dwLeftUpID			= fCommStr2Hex(xml.GetChildAttrib(_T("ID_Left_Up")));
			m_tagAppConfig.tagUiDataTour.dwLeftMiddleID		= fCommStr2Hex(xml.GetChildAttrib(_T("ID_Left_Middle")));
			m_tagAppConfig.tagUiDataTour.dwLeftDownID		= fCommStr2Hex(xml.GetChildAttrib(_T("ID_Left_Down")));
			m_tagAppConfig.tagUiDataTour.dwRightUpID		= fCommStr2Hex(xml.GetChildAttrib(_T("ID_Right_Up")));
			m_tagAppConfig.tagUiDataTour.dwRightMiddleID	= fCommStr2Hex(xml.GetChildAttrib(_T("ID_Right_Middle")));
			m_tagAppConfig.tagUiDataTour.dwRightDownID		= fCommStr2Hex(xml.GetChildAttrib(_T("ID_Right_Down")));
		}

		if(xml.FindChildElem(_T("Race")))
		{		
			m_tagAppConfig.tagUiDataRace.dwLeftUpID			= fCommStr2Hex(xml.GetChildAttrib(_T("ID_Left_Up")));
			m_tagAppConfig.tagUiDataRace.dwLeftDownID		= fCommStr2Hex(xml.GetChildAttrib(_T("ID_Left_Down")));
			m_tagAppConfig.tagUiDataRace.dwRightUpID		= fCommStr2Hex(xml.GetChildAttrib(_T("ID_Right_Up")));
			m_tagAppConfig.tagUiDataRace.dwRightDownID		= fCommStr2Hex(xml.GetChildAttrib(_T("ID_Right_Down")));
			m_tagAppConfig.tagUiDataRace.dwMiddleUpID		= fCommStr2Hex(xml.GetChildAttrib(_T("ID_Middle_Up")));
			m_tagAppConfig.tagUiDataRace.dwMiddleMiddleID	= fCommStr2Hex(xml.GetChildAttrib(_T("ID_Middle_Middle")));
			m_tagAppConfig.tagUiDataRace.dwMiddleDownID		= fCommStr2Hex(xml.GetChildAttrib(_T("ID_Middle_Down")));
		}
		xml.OutOfElem();
	}
	if(xml.FindChildElem(_T("Sound")))
	{
		m_tagAppConfig.tagSoundFile.strDong					=  strAppPath + xml.GetChildAttrib(_T("Dong"));
		m_tagAppConfig.tagSoundFile.strInitialze			=  strAppPath + xml.GetChildAttrib(_T("Initialze"));
		m_tagAppConfig.tagSoundFile.strStart				=  strAppPath + xml.GetChildAttrib(_T("Start"));
		m_tagAppConfig.tagSoundFile.strGetData				=  strAppPath + xml.GetChildAttrib(_T("GetData"));
		m_tagAppConfig.tagSoundFile.strAlarmCoolant			=  strAppPath + xml.GetChildAttrib(_T("AlarmCoolant"));
		m_tagAppConfig.tagSoundFile.strAlarmFatigueDriving	=  strAppPath + xml.GetChildAttrib(_T("AlarmFatigueDriving"));
		m_tagAppConfig.tagSoundFile.strAlarmOverSpeed		=  strAppPath + xml.GetChildAttrib(_T("AlarmOverSpeed"));
		m_tagAppConfig.tagSoundFile.strAlarmVoltHigh		=  strAppPath + xml.GetChildAttrib(_T("AlarmVoltHigh"));
		m_tagAppConfig.tagSoundFile.strAlarmVoltLow			=  strAppPath + xml.GetChildAttrib(_T("AlarmVoltLow"));
		m_tagAppConfig.tagSoundFile.strAlarmTrouble			=  strAppPath + xml.GetChildAttrib(_T("AlarmTrouble"));
	}
	if(xml.FindChildElem(_T("GPS_LIST")))
	{
		xml.IntoElem();

		m_vecGPSList.clear();
		while(xml.FindChildElem(_T("GPS")))
		{	
			TagAppInfo tagAppInfo;
			tagAppInfo.strAppID = xml.GetChildAttrib(_T("ID"));
			tagAppInfo.strAppName = xml.GetChildAttrib(_T("NAME_CHS"));
			tagAppInfo.strAppPath =  xml.GetChildAttrib(_T("FOLDER"));
			tagAppInfo.strAppExe = xml.GetChildAttrib(_T("EXE"));
			tagAppInfo.bAutoRun = FALSE;

			m_vecGPSList.push_back(tagAppInfo);
		}
		xml.OutOfElem();
	}	

	if(xml.FindChildElem(_T("DSA_LIST")))
	{
		xml.IntoElem();

		m_vecDSAList.clear();
		while(xml.FindChildElem(_T("DSA"))) 
		{	
			TagAppInfo tagAppInfo;
			tagAppInfo.strAppID = xml.GetChildAttrib(_T("ID"));
			tagAppInfo.strAppName = xml.GetChildAttrib(_T("NAME_CHS"));
			tagAppInfo.strAppPath =  xml.GetChildAttrib(_T("FOLDER"));
			tagAppInfo.strAppExe= xml.GetChildAttrib(_T("EXE"));
			tagAppInfo.bAutoRun = FALSE;

			m_vecDSAList.push_back(tagAppInfo);
		}
		xml.OutOfElem();
	}	
	
	if(xml.FindChildElem(_T("APP_START_LIST")))
	{
		xml.IntoElem();

		m_vecAppList.clear();
		while(xml.FindChildElem(_T("APP"))) 
		{	
			TagAppInfo tagAppInfo;
			tagAppInfo.strAppID = xml.GetChildAttrib(_T("ID"));
			if (tagAppInfo.strAppID == _T("1"))
				tagAppInfo.strAppName = theMainDlg->fGetBinText(DS_SETUP_GUIDE_DSA_TITLE);
			if (tagAppInfo.strAppID == _T("2"))
				tagAppInfo.strAppName = theMainDlg->fGetBinText(DS_SETUP_GUIDE_GPS_TITLE);
			tagAppInfo.strAppPath =  xml.GetChildAttrib(_T("PATH"));
			if (xml.GetChildAttrib(_T("AUTO_RUN")) == _T("1"))
				tagAppInfo.bAutoRun = TRUE;
			else
				tagAppInfo.bAutoRun = FALSE;

			m_vecAppList.push_back(tagAppInfo);
		}
		xml.OutOfElem();
	}	


	return TRUE;
}

// 出厂配置信息
BOOL COBDPNDConfigReadWrite::fResetAppConfig(void)
{
	m_tagAppConfig.tagSetting.strLanguage			= _T("");
	m_tagAppConfig.tagSetting.wBacklightDay			= 5;
	m_tagAppConfig.tagSetting.wBacklightNight		= 2;
	m_tagAppConfig.tagSetting.wBacklightDayStart	= 360;
	m_tagAppConfig.tagSetting.wBackLightDayEnd		= 1080;
	m_tagAppConfig.tagSetting.bSaveErrorLog			= FALSE;
	m_tagAppConfig.tagSetting.dwSystemOffTime		= 15;
	m_tagAppConfig.tagSetting.dwSystemOffMode		= 0;
	m_tagAppConfig.tagSetting.dwAutoSelPanelTime	= 30;
	m_tagAppConfig.tagSetting.dwDefaultStartPanel	= 0;

	m_tagAppConfig.tagSetting.fInstantFuelParameter	= 331;
	m_tagAppConfig.tagSetting.fAvgFuelParameter		= 116;
	m_tagAppConfig.tagSetting.fFuelPrice			= 7.49;
	m_tagAppConfig.tagSetting.dwActiveEcuID			= 0x00000000;

	m_tagAppConfig.tagAlarmSet.bTroubleCode = TRUE;
	m_tagAppConfig.tagAlarmSet.wSpeed	= 120;
	m_tagAppConfig.tagAlarmSet.wCoolant	= 100;
	m_tagAppConfig.tagAlarmSet.wFatigue_Driving	= 120;
	m_tagAppConfig.tagAlarmSet.dbFuel	= 10;

	m_tagAppConfig.tagAlarmSet.wShiftRpm		= 0;
	m_tagAppConfig.tagAlarmSet.wShiftSpeed	= 0;
	m_tagAppConfig.tagAlarmSet.wVoltMin		= 110;
	m_tagAppConfig.tagAlarmSet.wVoltMax		= 160;

	m_tagAppConfig.tagMiniSpeed.dw480X272_X			= 85;	
	m_tagAppConfig.tagMiniSpeed.dw480X272_Y			= 37;	
	m_tagAppConfig.tagMiniSpeed.dw800X480_X			= 212;	
	m_tagAppConfig.tagMiniSpeed.dw800X480_Y			= 58;		

	m_tagAppConfig.tagUiDataIdle.dwLeftUpID			= 0x00020001;
	m_tagAppConfig.tagUiDataIdle.dwLeftMiddleID		= 0xFF010005;
	m_tagAppConfig.tagUiDataIdle.dwLeftDownID		= 0xFF010002;
	m_tagAppConfig.tagUiDataIdle.dwRightUpID		= 0x00000C00;
	m_tagAppConfig.tagUiDataIdle.dwRightMiddleID	= 0xFF01000B;
	m_tagAppConfig.tagUiDataIdle.dwRightDownID		= 0x00000F00;

	m_tagAppConfig.tagUiDataTour.dwLeftUpID			= 0xFF010007;
	m_tagAppConfig.tagUiDataTour.dwLeftMiddleID		= 0xFF010009;
	m_tagAppConfig.tagUiDataTour.dwLeftDownID		= 0x00000500;
	m_tagAppConfig.tagUiDataTour.dwRightUpID		= 0xFF010008;
	m_tagAppConfig.tagUiDataTour.dwRightMiddleID	= 0xFF010006;
	m_tagAppConfig.tagUiDataTour.dwRightDownID		= 0xFF010001;

	m_tagAppConfig.tagUiDataRace.dwLeftUpID			= 0x00000D00;
	m_tagAppConfig.tagUiDataRace.dwLeftDownID		= 0x00000F00;
	m_tagAppConfig.tagUiDataRace.dwMiddleUpID		= 0x00001100;
	m_tagAppConfig.tagUiDataRace.dwMiddleMiddleID	= 0x00000400;
	m_tagAppConfig.tagUiDataRace.dwMiddleDownID		= 0x00000E00;
	m_tagAppConfig.tagUiDataRace.dwRightUpID		= 0x00000C00;
	m_tagAppConfig.tagUiDataRace.dwRightDownID		= 0x00000B00;


	for (int i=0;i<m_vecAppList.size();i++)
	{
		m_vecAppList[i].strAppPath = _T("");
		m_vecAppList[i].bAutoRun = FALSE;
	}

	return TRUE;
}

// 保存配置信息
BOOL COBDPNDConfigReadWrite::fSaveAppConfig(void)
{
	CMarkup xml;

	CString strAppConfigFile = fCommGetAppPath() + _T("\\Config.xml");
	if (!xml.Load(strAppConfigFile))
	{		
		return CONFIG_ERR_LOAD_FILE; 
	}
	CString strText;

	xml.ResetMainPos();
	xml.FindElem();	
	if(xml.FindChildElem(_T("System")))
	{		
		xml.IntoElem();	
		if(xml.FindChildElem(_T("COM_OBD")))
		{
			strText.Format(_T("%d"),m_tagAppConfig.tagComOBD.wPort);
			xml.SetChildAttrib(_T("Port"),strText);
			//strText.Format(_T("%d"),m_tagAppConfig.tagComOBD.dwBPS);
			//xml.SetChildAttrib(_T("BPS"),strText);
			//strText.Format(_T("%d"),m_tagAppConfig.tagComOBD.wData_Bits);
			//xml.SetChildAttrib(_T("Data_Bits"),strText);
			//strText.Format(_T("%d"),m_tagAppConfig.tagComOBD.wStop_Bits);
			//xml.SetChildAttrib(_T("Stop_Bits"),strText);
			//xml.SetChildAttrib(_T("Parity"),_T("None"));
		}

		//if(xml.FindChildElem(_T("COM_GPS")))
		//{
		//	strText.Format(_T("%d"),m_tagAppConfig.tagComGPS.wPort);
		//	xml.SetChildAttrib(_T("Port"),strText);
		//	strText.Format(_T("%d"),m_tagAppConfig.tagComGPS.dwBPS);
		//	xml.SetChildAttrib(_T("BPS"),strText);
		//	strText.Format(_T("%d"),m_tagAppConfig.tagComGPS.wData_Bits);
		//	xml.SetChildAttrib(_T("Data_Bits"),strText);
		//	strText.Format(_T("%d"),m_tagAppConfig.tagComGPS.wStop_Bits);
		//	xml.SetChildAttrib(_T("Stop_Bits"),strText);
		//	xml.SetChildAttrib(_T("Parity"),_T("None"));
		//}

		if(xml.FindChildElem(_T("Setting")))
		{

			xml.SetChildAttrib(_T("LANGUAGE"),m_tagAppConfig.tagSetting.strLanguage);

			if (m_tagAppConfig.tagSetting.bSaveErrorLog)
				xml.SetChildAttrib(_T("Save_ErrorLog"),_T("ON"));
			else
				xml.SetChildAttrib(_T("Save_ErrorLog"),_T("OFF"));

			strText.Format(_T("%d"),m_tagAppConfig.tagSetting.wBacklightDay);
			xml.SetChildAttrib(_T("BACKLIGHT_DAY"),strText);
			strText.Format(_T("%d"),m_tagAppConfig.tagSetting.wBacklightNight);
			xml.SetChildAttrib(_T("BACKLIGHT_NIGHT"),strText);
			strText.Format(_T("%d"),m_tagAppConfig.tagSetting.wBacklightDayStart);
			xml.SetChildAttrib(_T("BACKLIGHT_DAY_START"),strText);
			strText.Format(_T("%d"),m_tagAppConfig.tagSetting.wBackLightDayEnd);
			xml.SetChildAttrib(_T("BACKLIGHT_DAY_END"),strText);

			strText.Format(_T("%d"),m_tagAppConfig.tagSetting.dwSystemOffTime);
			xml.SetChildAttrib(_T("System_Off_Time"),strText);

			strText.Format(_T("%d"),m_tagAppConfig.tagSetting.dwSystemOffMode);
			xml.SetChildAttrib(_T("System_Off_Mode"),strText);


			strText.Format(_T("%d"),m_tagAppConfig.tagSetting.dwAutoSelPanelTime);
			xml.SetChildAttrib(_T("Auto_Sel_Panel_Time"),strText);

			strText.Format(_T("%d"),m_tagAppConfig.tagSetting.dwDefaultStartPanel);
			xml.SetChildAttrib(_T("Default_Start_Panel"),strText);

			strText.Format(_T("%0.5f"),m_tagAppConfig.tagSetting.fInstantFuelParameter);
			xml.SetChildAttrib(_T("Parameter_Fuel_Instant"),strText);

			strText.Format(_T("%0.5f"),m_tagAppConfig.tagSetting.fAvgFuelParameter);
			xml.SetChildAttrib(_T("Parameter_Fuel_Avg"),strText);

			strText.Format(_T("%0.5f"),m_tagAppConfig.tagSetting.fFuelPrice);
			xml.SetChildAttrib(_T("Fuel_Price"),strText);

			strText.Format(_T("0x%08X"),m_tagAppConfig.tagSetting.dwActiveEcuID);
			xml.SetChildAttrib(_T("Active_ECU_ID"),strText);
		}	
		if(xml.FindChildElem(_T("Set_Alarm")))
		{
			if (m_tagAppConfig.tagAlarmSet.bTroubleCode)
				xml.SetChildAttrib(_T("TroubleCode"),_T("ON"));
			else
				xml.SetChildAttrib(_T("TroubleCode"),_T("OFF"));

			strText.Format(_T("%d"),m_tagAppConfig.tagAlarmSet.wSpeed);
			xml.SetChildAttrib(_T("Speed"),strText);

			strText.Format(_T("%d"),m_tagAppConfig.tagAlarmSet.wCoolant);
			xml.SetChildAttrib(_T("Coolant"),strText);

			strText.Format(_T("%d"),m_tagAppConfig.tagAlarmSet.wFatigue_Driving);
			xml.SetChildAttrib(_T("Fatigue_Driving"),strText);

			strText.Format(_T("%0.2f"),m_tagAppConfig.tagAlarmSet.dbFuel);
			xml.SetChildAttrib(_T("Fuel"),strText);

			xml.IntoElem();
			if(xml.FindChildElem(_T("Shift")))
			{
				strText.Format(_T("%d"),m_tagAppConfig.tagAlarmSet.wShiftRpm);
				xml.SetChildAttrib(_T("Shift_Rpm"),strText);

				strText.Format(_T("%d"),m_tagAppConfig.tagAlarmSet.wShiftSpeed);
				xml.SetChildAttrib(_T("Shift_Speed"),strText);
			}
			if(xml.FindChildElem(_T("Volt")))
			{
				strText.Format(_T("%d"),m_tagAppConfig.tagAlarmSet.wVoltMin);
				xml.SetChildAttrib(_T("Volt_Min"),strText);

				strText.Format(_T("%d"),m_tagAppConfig.tagAlarmSet.wVoltMax);
				xml.SetChildAttrib(_T("Volt_Max"),strText);
			}
			xml.OutOfElem();
		}
		xml.OutOfElem();
	}

	if(xml.FindChildElem(_T("Mini_Dlg")))
	{
		strText.Format(_T("%d"),m_tagAppConfig.tagMiniSpeed.dw480X272_X);
		xml.SetChildAttrib(_T("X_480X272"),strText);
		strText.Format(_T("%d"),m_tagAppConfig.tagMiniSpeed.dw480X272_Y);
		xml.SetChildAttrib(_T("Y_480X272"),strText);
		strText.Format(_T("%d"),m_tagAppConfig.tagMiniSpeed.dw800X480_X);
		xml.SetChildAttrib(_T("X_800X480"),strText);
		strText.Format(_T("%d"),m_tagAppConfig.tagMiniSpeed.dw800X480_Y);
		xml.SetChildAttrib(_T("Y_800X480"),strText);
	}	

	if(xml.FindChildElem(_T("UiData")))
	{
		xml.IntoElem();

		if(xml.FindChildElem(_T("Idle")))
		{	
			strText.Format(_T("0x%08X"),m_tagAppConfig.tagUiDataIdle.dwLeftUpID);
			xml.SetChildAttrib(_T("ID_Left_Up"),strText);

			strText.Format(_T("0x%08X"),m_tagAppConfig.tagUiDataIdle.dwLeftMiddleID);
			xml.SetChildAttrib(_T("ID_Left_Middle"),strText);

			strText.Format(_T("0x%08X"),m_tagAppConfig.tagUiDataIdle.dwLeftDownID);
			xml.SetChildAttrib(_T("ID_Left_Down"),strText);

			strText.Format(_T("0x%08X"),m_tagAppConfig.tagUiDataIdle.dwRightUpID);
			xml.SetChildAttrib(_T("ID_Right_Up"),strText);

			strText.Format(_T("0x%08X"),m_tagAppConfig.tagUiDataIdle.dwRightMiddleID);
			xml.SetChildAttrib(_T("ID_Right_Middle"),strText);

			strText.Format(_T("0x%08X"),m_tagAppConfig.tagUiDataIdle.dwRightDownID);
			xml.SetChildAttrib(_T("ID_Right_Down"),strText);
		}
		if(xml.FindChildElem(_T("Tour")))
		{		
			strText.Format(_T("0x%08X"),m_tagAppConfig.tagUiDataTour.dwLeftUpID);
			xml.SetChildAttrib(_T("ID_Left_Up"),strText);

			strText.Format(_T("0x%08X"),m_tagAppConfig.tagUiDataTour.dwLeftMiddleID);
			xml.SetChildAttrib(_T("ID_Left_Middle"),strText);

			strText.Format(_T("0x%08X"),m_tagAppConfig.tagUiDataTour.dwLeftDownID);
			xml.SetChildAttrib(_T("ID_Left_Down"),strText);

			strText.Format(_T("0x%08X"),m_tagAppConfig.tagUiDataTour.dwRightUpID);
			xml.SetChildAttrib(_T("ID_Right_Up"),strText);

			strText.Format(_T("0x%08X"),m_tagAppConfig.tagUiDataTour.dwRightMiddleID);
			xml.SetChildAttrib(_T("ID_Right_Middle"),strText);

			strText.Format(_T("0x%08X"),m_tagAppConfig.tagUiDataTour.dwRightDownID);
			xml.SetChildAttrib(_T("ID_Right_Down"),strText);
		}

		if(xml.FindChildElem(_T("Race")))
		{		
			strText.Format(_T("0x%08X"),m_tagAppConfig.tagUiDataRace.dwLeftUpID);
			xml.SetChildAttrib(_T("ID_Left_Up"),strText);

			strText.Format(_T("0x%08X"),m_tagAppConfig.tagUiDataRace.dwLeftDownID);
			xml.SetChildAttrib(_T("ID_Left_Down"),strText);

			strText.Format(_T("0x%08X"),m_tagAppConfig.tagUiDataRace.dwMiddleUpID);
			xml.SetChildAttrib(_T("ID_Middle_Up"),strText);

			strText.Format(_T("0x%08X"),m_tagAppConfig.tagUiDataRace.dwMiddleMiddleID);
			xml.SetChildAttrib(_T("ID_Middle_Middle"),strText);

			strText.Format(_T("0x%08X"),m_tagAppConfig.tagUiDataRace.dwMiddleDownID);
			xml.SetChildAttrib(_T("ID_Middle_Down"),strText);

			strText.Format(_T("0x%08X"),m_tagAppConfig.tagUiDataRace.dwRightUpID);
			xml.SetChildAttrib(_T("ID_Right_Up"),strText);

			strText.Format(_T("0x%08X"),m_tagAppConfig.tagUiDataRace.dwRightDownID);
			xml.SetChildAttrib(_T("ID_Right_Down"),strText);
		}
		xml.OutOfElem();
	}

	if(xml.FindChildElem(_T("APP_START_LIST")))
	{
		xml.IntoElem();

		while(xml.FindChildElem(_T("APP"))) 
		{	
			for (int i=0;i<m_vecAppList.size();i++)
			{
				if (xml.GetChildAttrib(_T("ID")) == m_vecAppList[i].strAppID)
				{
					xml.SetChildAttrib(_T("PATH"),m_vecAppList[i].strAppPath);
					if (m_vecAppList[i].bAutoRun)
						xml.SetChildAttrib(_T("AUTO_RUN"),_T("1"));
					else
						xml.SetChildAttrib(_T("AUTO_RUN"),_T("0"));
				}
			}
		}
		xml.OutOfElem();
	}	



	xml.Save(strAppConfigFile);

	return TRUE;
}


_TagUiDataIdle COBDPNDConfigReadWrite::fGetIdleUiData()
{
	return m_tagAppConfig.tagUiDataIdle;
}
_TagUiDataTour COBDPNDConfigReadWrite::fGetTourUiData()
{
	return m_tagAppConfig.tagUiDataTour;
}
_TagUiDataRace COBDPNDConfigReadWrite::fGetRaceUiData()
{
	return m_tagAppConfig.tagUiDataRace;
}

void COBDPNDConfigReadWrite::fSetIdleUiData(WORD wIndex,DWORD dwCmdID)
{
	switch (wIndex)
	{
	case 0:
		m_tagAppConfig.tagUiDataIdle.dwLeftUpID = dwCmdID;
		break;
	case 1:
		m_tagAppConfig.tagUiDataIdle.dwLeftMiddleID = dwCmdID;
		break;
	case 2:
		m_tagAppConfig.tagUiDataIdle.dwLeftDownID = dwCmdID;
		break;
	case 3:
		m_tagAppConfig.tagUiDataIdle.dwRightUpID = dwCmdID;
		break;
	case 4:
		m_tagAppConfig.tagUiDataIdle.dwRightMiddleID = dwCmdID;
		break;
	case 5:
		m_tagAppConfig.tagUiDataIdle.dwRightDownID = dwCmdID;
		break;
	}
}
void COBDPNDConfigReadWrite::fSetTourUiData(WORD wIndex,DWORD dwCmdID)
{
	switch (wIndex)
	{
	case 0:
		m_tagAppConfig.tagUiDataTour.dwLeftUpID = dwCmdID;
		break;
	case 1:
		m_tagAppConfig.tagUiDataTour.dwLeftMiddleID = dwCmdID;
		break;
	case 2:
		m_tagAppConfig.tagUiDataTour.dwLeftDownID = dwCmdID;
		break;
	case 3:
		m_tagAppConfig.tagUiDataTour.dwRightUpID = dwCmdID;
		break;
	case 4:
		m_tagAppConfig.tagUiDataTour.dwRightMiddleID = dwCmdID;
		break;
	case 5:
		m_tagAppConfig.tagUiDataTour.dwRightDownID = dwCmdID;
		break;
	}
}
void COBDPNDConfigReadWrite::fSetRaceUiData(WORD wIndex,DWORD dwCmdID)
{
	switch (wIndex)
	{
	case 0:
		m_tagAppConfig.tagUiDataRace.dwLeftUpID = dwCmdID;
		break;
	case 1:
		m_tagAppConfig.tagUiDataRace.dwLeftDownID = dwCmdID;
		break;
	case 2:
		m_tagAppConfig.tagUiDataRace.dwMiddleUpID = dwCmdID;
		break;
	case 3:
		m_tagAppConfig.tagUiDataRace.dwMiddleMiddleID = dwCmdID;
		break;
	case 4:
		m_tagAppConfig.tagUiDataRace.dwMiddleDownID = dwCmdID;
		break;
	case 5:
		m_tagAppConfig.tagUiDataRace.dwRightUpID = dwCmdID;
		break;
	case 6:
		m_tagAppConfig.tagUiDataRace.dwRightDownID = dwCmdID;
		break;
	}
}

// 平均油耗系数
double COBDPNDConfigReadWrite::fGetAvgFCC()
{
	return m_tagAppConfig.tagSetting.fAvgFuelParameter;
}
void COBDPNDConfigReadWrite::fSetAvgFCC(double dbValue)
{
	m_tagAppConfig.tagSetting.fAvgFuelParameter = dbValue;
}

// 即时油耗系数
double COBDPNDConfigReadWrite::fGetNowFCC()
{
	return m_tagAppConfig.tagSetting.fInstantFuelParameter;
}
void COBDPNDConfigReadWrite::fSetNowFCC(double dbValue)
{
	m_tagAppConfig.tagSetting.fInstantFuelParameter = dbValue;
}
//
//// 累计里程
//double COBDPNDConfigReadWrite::fGetTotalMileage()
//{
//	return m_tagAppConfig.tagSetting.dTotalMileage;
//}
//void COBDPNDConfigReadWrite::fSetTotalMileage(double dbValue)
//{
//	m_tagAppConfig.tagSetting.dTotalMileage = dbValue;
//}
//
//// 累计油耗
//double COBDPNDConfigReadWrite::fGetTotalFuelConsumption()
//{
//	return m_tagAppConfig.tagSetting.dTotalFuelConsumption;
//}
//void COBDPNDConfigReadWrite::fSetTotalFuelConsumption(double dbValue)
//{
//	m_tagAppConfig.tagSetting.dTotalFuelConsumption = dbValue;
//}
//
//// 剩余油量
//double COBDPNDConfigReadWrite::fGetFuelRemain()
//{
//	return m_tagAppConfig.tagSetting.fFuelRemain;
//}
//void COBDPNDConfigReadWrite::fSetFuelRemain(double dbValue)
//{
//	m_tagAppConfig.tagSetting.fFuelRemain = dbValue;
//}
//double COBDPNDConfigReadWrite::fGetFuelRemainAlarm()
//{
//	return m_tagAppConfig.tagAlarmSet.dbFuel;
//}
//void COBDPNDConfigReadWrite::fSetFuelRemainAlarm(double dbValue)
//{
//	m_tagAppConfig.tagAlarmSet.dbFuel = dbValue;
//}

// 油价
double COBDPNDConfigReadWrite::fGetFuelPrice()
{
	return m_tagAppConfig.tagSetting.fFuelPrice;
}
void COBDPNDConfigReadWrite::fSetFuelPrice(double dbValue)
{
	m_tagAppConfig.tagSetting.fFuelPrice = dbValue;
}


CString COBDPNDConfigReadWrite::fGetTextBinFileName()
{
	return fCommGetAppPath() + _T("\\") + m_tagAppConfig.strTextFile;
}

// 获取OBD串口配置信息
WORD COBDPNDConfigReadWrite::fGetOBDComPortNo()
{
	return m_tagAppConfig.tagComOBD.wPort;
}
DWORD COBDPNDConfigReadWrite::fGetOBDComBaun()
{
	return m_tagAppConfig.tagComOBD.dwBPS;
}
WORD COBDPNDConfigReadWrite::fGetOBDComDataBits()
{
	return m_tagAppConfig.tagComOBD.wData_Bits;
}
WORD COBDPNDConfigReadWrite::fGetOBDComStopBits()
{
	return m_tagAppConfig.tagComOBD.wStop_Bits;
}
WORD COBDPNDConfigReadWrite::fGetOBDComParity()
{
	return m_tagAppConfig.tagComOBD.wParity;
}
void COBDPNDConfigReadWrite::fSetOBDComPortNo(WORD wPort)
{
	m_tagAppConfig.tagComOBD.wPort = wPort;
}
void COBDPNDConfigReadWrite::fSetOBDComBaun(DWORD dwBps)
{
	m_tagAppConfig.tagComOBD.dwBPS = dwBps;
}
void COBDPNDConfigReadWrite::fSetOBDComDataBits(WORD wDataBits)
{
	m_tagAppConfig.tagComOBD.wData_Bits = wDataBits;
}
void COBDPNDConfigReadWrite::fSetOBDComStopBits(WORD wStopBits)
{
	m_tagAppConfig.tagComOBD.wStop_Bits = wStopBits;
}
void COBDPNDConfigReadWrite::fSetOBDComParity(WORD wParity)
{
	m_tagAppConfig.tagComOBD.wParity = wParity;
}

// 获取GPS串口配置信息
WORD COBDPNDConfigReadWrite::fGetGPSComPortNo()
{
	return m_tagAppConfig.tagComGPS.wPort;
}
DWORD COBDPNDConfigReadWrite::fGetGPSComBaun()
{
	return m_tagAppConfig.tagComGPS.dwBPS;
}
WORD COBDPNDConfigReadWrite::fGetGPSComDataBits()
{
	return m_tagAppConfig.tagComGPS.wData_Bits;
}
WORD COBDPNDConfigReadWrite::fGetGPSComStopBits()
{
	return m_tagAppConfig.tagComGPS.wStop_Bits;
}
WORD COBDPNDConfigReadWrite::fGetGPSComParity()
{
	return m_tagAppConfig.tagComGPS.wParity;
}
void COBDPNDConfigReadWrite::fSetGPSComPortNo(WORD wPort)
{
	m_tagAppConfig.tagComGPS.wPort = wPort;
}
void COBDPNDConfigReadWrite::fSetGPSComBaun(DWORD dwBps)
{
	m_tagAppConfig.tagComGPS.dwBPS = dwBps;
}
void COBDPNDConfigReadWrite::fSetGPSComDataBits(WORD wDataBits)
{
	m_tagAppConfig.tagComGPS.wData_Bits = wDataBits;
}
void COBDPNDConfigReadWrite::fSetGPSComStopBits(WORD wStopBits)
{
	m_tagAppConfig.tagComGPS.wStop_Bits = wStopBits;
}
void COBDPNDConfigReadWrite::fSetGPSComParity(WORD wParity)
{
	m_tagAppConfig.tagComGPS.wParity = wParity;
}



WORD COBDPNDConfigReadWrite::fGetBacklightDay()
{
	return m_tagAppConfig.tagSetting.wBacklightDay;
}
WORD COBDPNDConfigReadWrite::fGetBacklightNight()
{
	return m_tagAppConfig.tagSetting.wBacklightNight;
}
WORD COBDPNDConfigReadWrite::fGetBacklightDayStart()
{
	return m_tagAppConfig.tagSetting.wBacklightDayStart;
}
WORD COBDPNDConfigReadWrite::fGetBacklightDayEnd()
{
	return m_tagAppConfig.tagSetting.wBackLightDayEnd;
}
void COBDPNDConfigReadWrite::fSetBacklightDay(WORD wValue)
{
	m_tagAppConfig.tagSetting.wBacklightDay = wValue;
}
void COBDPNDConfigReadWrite::fSetBacklightNight(WORD wValue)
{
	m_tagAppConfig.tagSetting.wBacklightNight = wValue;
}
void COBDPNDConfigReadWrite::fSetBacklightDayStart(WORD wValue)
{
	m_tagAppConfig.tagSetting.wBacklightDayStart = wValue;
}
void COBDPNDConfigReadWrite::fSetBacklightDayEnd(WORD wValue)
{
	m_tagAppConfig.tagSetting.wBackLightDayEnd = wValue;
}

void COBDPNDConfigReadWrite::fSetActiveEcuID(DWORD dwEcuID)
{
	m_tagAppConfig.tagSetting.dwActiveEcuID = dwEcuID;
}

DWORD COBDPNDConfigReadWrite::fGetActiveEcuID()
{
	return m_tagAppConfig.tagSetting.dwActiveEcuID;
}
CString COBDPNDConfigReadWrite::fGetActiveLanguage()
{
	return m_tagAppConfig.tagSetting.strLanguage;
}
void COBDPNDConfigReadWrite::fSetActiveLanguage(CString strLanguage)
{
	m_tagAppConfig.tagSetting.strLanguage = strLanguage;
}
vector<_TagLanguage> COBDPNDConfigReadWrite::fGetLanguage()
{
	return m_tagAppConfig.vecLanguage;
}

DWORD COBDPNDConfigReadWrite::fGetSysOffTime()
{
	return m_tagAppConfig.tagSetting.dwSystemOffTime;
}
void COBDPNDConfigReadWrite::fSetSysOffTime(DWORD dwSysOffTime)
{
	m_tagAppConfig.tagSetting.dwSystemOffTime = dwSysOffTime;
}

BOOL COBDPNDConfigReadWrite::fGetSaveErrLogStatus()
{
	return m_tagAppConfig.tagSetting.bSaveErrorLog;
}
void COBDPNDConfigReadWrite::fSetSaveErrLogStatus(BOOL bEnable)
{
	m_tagAppConfig.tagSetting.bSaveErrorLog = bEnable;
}

BOOL COBDPNDConfigReadWrite::fGetTroubleCodeAlarmStatus()
{
	return m_tagAppConfig.tagAlarmSet.bTroubleCode;
}
void COBDPNDConfigReadWrite::fSetTroubleCodeAlarmStatus(BOOL bEnable)
{
	m_tagAppConfig.tagAlarmSet.bTroubleCode= bEnable;
}

DWORD COBDPNDConfigReadWrite::fGetWaterTemperatureAlarmValue()
{
	return m_tagAppConfig.tagAlarmSet.wCoolant;
}
void COBDPNDConfigReadWrite::fSetWaterTemperatureAlarmValue(DWORD dwValue)
{
	m_tagAppConfig.tagAlarmSet.wCoolant= dwValue;
}

DWORD COBDPNDConfigReadWrite::fGetFatigueDrivingAlarmValue()
{
	return m_tagAppConfig.tagAlarmSet.wFatigue_Driving;
}
void COBDPNDConfigReadWrite::fSetFatigueDrivingAlarmValue(DWORD dwValue)
{
	m_tagAppConfig.tagAlarmSet.wFatigue_Driving= dwValue;
}

DWORD COBDPNDConfigReadWrite::fGetOverSpeedAlarmValue()
{
	return m_tagAppConfig.tagAlarmSet.wSpeed;
}
void COBDPNDConfigReadWrite::fSetOverSpeedAlarmValue(DWORD dwValue)
{
	m_tagAppConfig.tagAlarmSet.wSpeed= dwValue;
}

DWORD COBDPNDConfigReadWrite::fGetShiftAlarmRpm()
{
	return m_tagAppConfig.tagAlarmSet.wShiftRpm;
}
void COBDPNDConfigReadWrite::fSetShiftAlarmRpm(DWORD dwValue)
{
	m_tagAppConfig.tagAlarmSet.wShiftRpm= dwValue;
}
DWORD COBDPNDConfigReadWrite::fGetShiftAlarmVss()
{
	return m_tagAppConfig.tagAlarmSet.wShiftSpeed;
}
void COBDPNDConfigReadWrite::fSetShiftAlarmVss(DWORD dwValue)
{
	m_tagAppConfig.tagAlarmSet.wShiftSpeed= dwValue;
}

DWORD COBDPNDConfigReadWrite::fGetVoltMaxAlarm()
{
	return m_tagAppConfig.tagAlarmSet.wVoltMax;
}
void COBDPNDConfigReadWrite::fSetVoltMaxAlarm(DWORD dwValue)
{
	m_tagAppConfig.tagAlarmSet.wVoltMax= dwValue;
}
DWORD COBDPNDConfigReadWrite::fGetVoltMinAlarm()
{
	return m_tagAppConfig.tagAlarmSet.wVoltMin;
}
void COBDPNDConfigReadWrite::fSetVoltMinAlarm(DWORD dwValue)
{
	m_tagAppConfig.tagAlarmSet.wVoltMin= dwValue;
}

DWORD COBDPNDConfigReadWrite::fGetAutoSelectPanelTime()
{
	return m_tagAppConfig.tagSetting.dwAutoSelPanelTime;
}
void COBDPNDConfigReadWrite::SetAutoSelectPanelTime(DWORD dwValue)
{
	m_tagAppConfig.tagSetting.dwAutoSelPanelTime = dwValue;
}
vector<TagAppInfo> COBDPNDConfigReadWrite::fGetStartList()
{
	return m_vecAppList;
}
void COBDPNDConfigReadWrite::fSetStartList(vector<TagAppInfo> vecAppList)
{
	m_vecAppList = vecAppList;
}

vector<CString> COBDPNDConfigReadWrite::fGetFilterAppList()
{
	vector<CString> vecAppList = fCommStrSplit(m_tagAppConfig.strFilterAppList,';');

	return vecAppList;
}


CPoint COBDPNDConfigReadWrite::fGetMiniDlgPoint()
{
	return CPoint(m_tagAppConfig.tagMiniSpeed.dw800X480_X,m_tagAppConfig.tagMiniSpeed.dw800X480_Y);
}
void COBDPNDConfigReadWrite::fSetMiniDlgPoint(CPoint pointNew)
{
	m_tagAppConfig.tagMiniSpeed.dw800X480_X = pointNew.x;
	m_tagAppConfig.tagMiniSpeed.dw800X480_Y = pointNew.y;
}

vector<TagAppInfo> COBDPNDConfigReadWrite::fGetExistGPS()
{
	CCeFileFind fileFindPath;
	CCeFileFind fileFindFile;
	BOOL bFoundPath = FALSE;
	BOOL bFoundFile = FALSE;
	vector<TagAppInfo> vecFileList;
	CString strPathName;
	CString strFileName;

	vector<CString> vecDirList;
	vecDirList.push_back(_T("\\SDMMC\\*.*"));
	CString strCurrDir = fCommGetAppPath();
	int iPos = strCurrDir.Find(_T("\\"));
	if (iPos>=0)
	{	
		iPos = strCurrDir.Find(_T("\\"),iPos+1);
		if (iPos>=0) 
			vecDirList.push_back(strCurrDir.Left(iPos+1)+_T("*.*"));
	}
	for (vector<CString>::iterator it=vecDirList.begin();it!=vecDirList.end();it++)
	{
		bFoundPath = fileFindPath.FindFile(*it);
		while (bFoundPath) 
		{
			bFoundPath = fileFindPath.FindNextFile();
			if (fileFindPath.IsDirectory())
			{
				strPathName = fileFindPath.GetFileName().MakeUpper(); 

				for (int i=0;i<m_vecGPSList.size();i++)
				{ 
					//找到目录 
					if (strPathName == m_vecGPSList[i].strAppPath.MakeUpper())
					{
						bFoundFile = fileFindFile.FindFile(fileFindPath.GetFilePath()+_T("\\*.*"));
						while (bFoundFile)
						{
							bFoundFile = fileFindFile.FindNextFile();
							if (!fileFindFile.IsDirectory() && !fileFindFile.IsDots())
							{
								strFileName = fileFindFile.GetFileName().MakeUpper();
								//找到文件
								if (strFileName == m_vecGPSList[i].strAppExe.MakeUpper())
								{
									TagAppInfo tagFileProperty;

									tagFileProperty.strAppName = m_vecGPSList[i].strAppName; 
									tagFileProperty.strAppExe = strFileName;
									tagFileProperty.strAppPath = fileFindFile.GetFilePath();
									tagFileProperty.bAutoRun = FALSE;
									vecFileList.push_back(tagFileProperty);
								}
							}
						}
					}		
				}
			}

		}

	}

	return vecFileList;
}
vector<TagAppInfo> COBDPNDConfigReadWrite::fGetExistDSA()
{

	CCeFileFind fileFindPath;
	CCeFileFind fileFindFile;
	BOOL bFoundPath = FALSE;
	BOOL bFoundFile = FALSE;
	vector<TagAppInfo> vecFileList;
	CString strPathName;
	CString strFileName;

	vector<CString> vecDirList;
	vecDirList.push_back(_T("\\SDMMC\\*.*"));
	CString strCurrDir = fCommGetAppPath();
	int iPos = strCurrDir.Find(_T("\\"));
	if (iPos>=0)
	{	
		iPos = strCurrDir.Find(_T("\\"),iPos+1);
		if (iPos>=0) 
			vecDirList.push_back(strCurrDir.Left(iPos+1)+_T("*.*"));
	}
	for (vector<CString>::iterator it=vecDirList.begin();it!=vecDirList.end();it++)
	{
		bFoundPath = fileFindPath.FindFile(*it);
		while (bFoundPath)
		{
			bFoundPath = fileFindPath.FindNextFile();
			if (fileFindPath.IsDirectory())
			{
				strPathName = fileFindPath.GetFileName().MakeUpper();

				for (int i=0;i<m_vecDSAList.size();i++)
				{
					//找到目录
					if (strPathName == m_vecDSAList[i].strAppPath.MakeUpper())
					{
						bFoundFile = fileFindFile.FindFile(fileFindPath.GetFilePath()+_T("\\*.*"));
						while (bFoundFile)
						{
							bFoundFile = fileFindFile.FindNextFile();
							if (!fileFindFile.IsDirectory() && !fileFindFile.IsDots())
							{
								strFileName = fileFindFile.GetFileName().MakeUpper();
								//找到文件
								if (strFileName == m_vecDSAList[i].strAppExe.MakeUpper()) 
								{
									TagAppInfo tagFileProperty;

									tagFileProperty.strAppName = m_vecDSAList[i].strAppName; 
									tagFileProperty.strAppExe = strFileName;
									tagFileProperty.strAppPath = fileFindFile.GetFilePath();
									tagFileProperty.bAutoRun = FALSE;

									vecFileList.push_back(tagFileProperty);
								}
							}
						}
					}		

				}
			}

		}

	}

	return vecFileList;
}


CString COBDPNDConfigReadWrite::fGetSoundFile(DWORD dwFileID)
{
	switch (dwFileID)
	{
	case SOUND_FILE_Dong:
		return m_tagAppConfig.tagSoundFile.strDong;
		break;
	case SOUND_FILE_Initialze:
		return m_tagAppConfig.tagSoundFile.strInitialze;
		break;
	case SOUND_FILE_Start:
		return m_tagAppConfig.tagSoundFile.strStart;
		break;
	case SOUND_FILE_GetData:
		return m_tagAppConfig.tagSoundFile.strGetData;
		break;
	case SOUND_FILE_AlarmCoolant:
		return m_tagAppConfig.tagSoundFile.strAlarmCoolant;
		break;
	case SOUND_FILE_AlarmFatigueDriving:
		return m_tagAppConfig.tagSoundFile.strAlarmFatigueDriving;
		break;
	case SOUND_FILE_AlarmOverSpeed:
		return m_tagAppConfig.tagSoundFile.strAlarmOverSpeed;
		break;
	case SOUND_FILE_AlarmVoltHigh:
		return m_tagAppConfig.tagSoundFile.strAlarmVoltHigh;
		break;
	case SOUND_FILE_AlarmVoltLow:
		return m_tagAppConfig.tagSoundFile.strAlarmVoltLow;
		break;
	case SOUND_FILE_AlarmTrouble:
		return m_tagAppConfig.tagSoundFile.strAlarmTrouble;
		break;
	}
}