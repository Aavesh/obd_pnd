/*
 * Copyright(C) 2013,V-Checker 保留所有权利。( All rights reserved. )
 * 
 * 文件名称：OBDRClsConfigReadWrite.h
 * 摘    要：应用程序配置信息处理类
 * 当前版本：1.0
 * 作    者：Yipb
 * 创建日期：2013年12月5日
 */
#pragma once

struct TagAppInfo
{
	CString strAppID;
	CString strAppName;
	CString	strAppExe;
	CString strAppPath;

	BOOL	bAutoRun;
};

//错误信息
enum CONFIG_ERR
{
	CONFIG_OK,
	CONFIG_ERR_FIND_FILE,							//不存在配置文件
	CONFIG_ERR_LOAD_FILE,							//装载配置文件失败
};

enum SOUND_FILE_ID
{
	SOUND_FILE_Dong,
	SOUND_FILE_Initialze,
	SOUND_FILE_Start,
	SOUND_FILE_GetData,
	SOUND_FILE_AlarmCoolant,
	SOUND_FILE_AlarmFatigueDriving,
	SOUND_FILE_AlarmOverSpeed,
	SOUND_FILE_AlarmVoltHigh,
	SOUND_FILE_AlarmVoltLow,
	SOUND_FILE_AlarmTrouble,
};

struct TagComData
{
	WORD	wPort;			//串口号
	DWORD	dwBPS;			//波特率
	WORD	wData_Bits;		//数据位
	WORD	wStop_Bits;		//停止位
	WORD	wParity;		//效验

	TagComData()
	{
		wPort = 1;
		dwBPS = 115200;
		wData_Bits = 8;
		wStop_Bits = 0;
		wParity = 0;
	}
};


//语言列表
struct _TagLanguage
{
	CString strLanID;
	CString strLanName;
	CString strLanSN;
	CString strResFiles;
};
//报警设置
struct _TagSetAlarm
{
	BOOL	bTroubleCode;			//是否开启报警
	DWORD	wSpeed;					//超速报警设置
	DWORD	wCoolant;				//水温报警设置
	DWORD	wFatigue_Driving;		//疲劳驾驶报警设置（分钟）

	double	dbFuel;					//油量报警

	DWORD	wShiftRpm;				//换挡提醒
	DWORD	wShiftSpeed;			//换挡提醒

	DWORD	wVoltMin;				//电压低报警
	DWORD	wVoltMax;				//电压高报警

	_TagSetAlarm()
	{
		bTroubleCode	= TRUE;
		wSpeed			= 120;
		wCoolant		= 100;
		wFatigue_Driving= 120;
		dbFuel			= 0.0;

		wShiftRpm		= 0;
		wShiftSpeed		= 0;

		wVoltMin		= 0;
		wVoltMax		= 0;
	}

};

struct _TagMiniSpeed
{
	DWORD	dw800X480_X;
	DWORD	dw800X480_Y;
	DWORD	dw480X272_X;
	DWORD	dw480X272_Y;

	_TagMiniSpeed()
	{
		dw800X480_X = 0;
		dw800X480_Y = 0;
		dw480X272_X = 0;
		dw480X272_Y = 0;

	}

};

//怠速界面配置数据
struct _TagUiDataIdle
{
	DWORD	dwLeftUpID;
	DWORD	dwLeftMiddleID;
	DWORD	dwLeftDownID;
	DWORD	dwRightUpID;
	DWORD	dwRightMiddleID;
	DWORD	dwRightDownID;
	_TagUiDataIdle()
	{
		dwLeftUpID=dwLeftDownID=dwLeftMiddleID=dwRightUpID=dwRightDownID=dwRightUpID=dwRightMiddleID = 0;
	}
};

//巡航界面配置数据
struct _TagUiDataTour
{
	DWORD	dwLeftUpID;
	DWORD	dwLeftMiddleID;
	DWORD	dwLeftDownID;
	DWORD	dwRightUpID;
	DWORD	dwRightMiddleID;
	DWORD	dwRightDownID;
	_TagUiDataTour()
	{
		dwLeftUpID=dwLeftDownID=dwLeftMiddleID=dwRightUpID=dwRightDownID=dwRightUpID=dwRightMiddleID = 0;
	}
};

//竞技界面配置数据
struct _TagUiDataRace 
{
	DWORD	dwLeftUpID;
	DWORD	dwLeftDownID;
	DWORD	dwRightUpID;
	DWORD	dwRightDownID;
	DWORD	dwMiddleUpID;
	DWORD	dwMiddleMiddleID;
	DWORD	dwMiddleDownID;
	_TagUiDataRace()
	{
		dwLeftUpID=dwLeftDownID=dwRightUpID=dwRightDownID=dwRightUpID=dwMiddleUpID=dwMiddleMiddleID=dwMiddleDownID=0;
	}
};


struct _TagAppSetting
{
	CString	strLanguage;
	BOOL	bSaveErrorLog;
	WORD	wBacklightDay;
	WORD	wBacklightNight;
	WORD	wBacklightDayStart;
	WORD	wBackLightDayEnd;
	DWORD   dwActiveEcuID;
	CString str24or12;
	DWORD	dwSystemOffTime;
	DWORD	dwSystemOffMode;
	DWORD	dwAutoSelPanelTime;
	DWORD	dwDefaultStartPanel;
	double	fInstantFuelParameter;
	double	fAvgFuelParameter;
	double	fFuelPrice;

	_TagAppSetting()
	{
		strLanguage				= _T("");
		wBacklightDay			= 5;
		wBacklightNight			= 2;
		wBacklightDayStart		= 360;
		wBackLightDayEnd		= 1080;
		dwActiveEcuID			= 0;
		bSaveErrorLog			= FALSE;
		str24or12				= _T("");
		dwSystemOffTime			= 15;
		dwSystemOffMode			= 0;
		dwAutoSelPanelTime		= 30;
		dwDefaultStartPanel		= 0;
		fInstantFuelParameter	= 331.000;
		fAvgFuelParameter		= 116.000;
		fFuelPrice				= 7.49000;
	}
};

struct _TagSoundFile
{
	CString strDong;
	CString strInitialze;
	CString	strStart;
	CString strGetData;
	CString strAlarmCoolant;
	CString strAlarmFatigueDriving;
	CString strAlarmOverSpeed;
	CString strAlarmVoltHigh;
	CString strAlarmVoltLow;
	CString strAlarmTrouble;

	_TagSoundFile()
	{
		strDong = strInitialze = strStart = strGetData = strAlarmCoolant = strAlarmFatigueDriving = strAlarmOverSpeed = strAlarmVoltHigh = strAlarmVoltLow = strAlarmTrouble = _T("");
	}
};

//应用软件配置信息
struct _TagAppConfig
{
	BOOL	bIsDemo;
	CString strTextFile;
	CString strFilterAppList;
	_TagAppSetting	tagSetting;
	vector<_TagLanguage> vecLanguage;
	_TagSetAlarm tagAlarmSet;
	_TagMiniSpeed tagMiniSpeed;
	_TagUiDataIdle	tagUiDataIdle;
	_TagUiDataTour	tagUiDataTour;
	_TagUiDataRace	tagUiDataRace;
	_TagSoundFile	tagSoundFile;

	TagComData	tagComOBD;
	TagComData	tagComGPS;
};


class COBDPNDConfigReadWrite
{
public:
	COBDPNDConfigReadWrite(void);
	~COBDPNDConfigReadWrite(void);

	// 初始化应用配置信息
	BOOL fInitAppConfig(void);
	// 保存配置信息
	BOOL fSaveAppConfig(void);
	// 出厂配置信息
	BOOL fResetAppConfig(void);
	// 获取怠速界面配置信息
	_TagUiDataIdle fGetIdleUiData();
	_TagUiDataTour fGetTourUiData();
	_TagUiDataRace fGetRaceUiData();
	void fSetIdleUiData(WORD wIndex,DWORD dwCmdID);
	void fSetTourUiData(WORD wIndex,DWORD dwCmdID);
	void fSetRaceUiData(WORD wIndex,DWORD dwCmdID);

	// 获取文本库文件路径
	CString fGetTextBinFileName();	
	// 获取OBD串口配置信息
	WORD	fGetOBDComPortNo();
	DWORD	fGetOBDComBaun();
	WORD	fGetOBDComDataBits();
	WORD	fGetOBDComStopBits();
	WORD	fGetOBDComParity();

	void fSetOBDComPortNo(WORD wPort);
	void fSetOBDComBaun(DWORD dwBps);
	void fSetOBDComDataBits(WORD wDataBits);
	void fSetOBDComStopBits(WORD wStopBits);
	void fSetOBDComParity(WORD wParity);

	// 获取GPS串口配置信息
	WORD	fGetGPSComPortNo();
	DWORD	fGetGPSComBaun();
	WORD	fGetGPSComDataBits();
	WORD	fGetGPSComStopBits();
	WORD	fGetGPSComParity();

	void fSetGPSComPortNo(WORD wPort);
	void fSetGPSComBaun(DWORD dwBps);
	void fSetGPSComDataBits(WORD wDataBits);
	void fSetGPSComStopBits(WORD wStopBits);
	void fSetGPSComParity(WORD wParity);


	WORD	fGetBacklightDay();
	WORD	fGetBacklightNight();
	WORD	fGetBacklightDayStart();
	WORD	fGetBacklightDayEnd();
	void fSetBacklightDay(WORD wValue);
	void fSetBacklightNight(WORD wValue);
	void fSetBacklightDayStart(WORD wValue);
	void fSetBacklightDayEnd(WORD wValue);

	// 即时油耗系数
	double fGetAvgFCC();
	void fSetAvgFCC(double dbValue);
	// 平均油耗系数
	double fGetNowFCC();
	void fSetNowFCC(double dbValue);
	// 油价
	double fGetFuelPrice();
	void fSetFuelPrice(double dbValue);

	DWORD fGetActiveEcuID();
	void fSetActiveEcuID(DWORD dwEcuID);

	CString fGetActiveLanguage();
	void fSetActiveLanguage(CString strLanguage);
	vector<_TagLanguage> fGetLanguage();

	DWORD fGetSysOffTime();
	void fSetSysOffTime(DWORD dwSysOffTime);

	BOOL fGetSaveErrLogStatus();
	void fSetSaveErrLogStatus(BOOL bEnable);

	BOOL fGetTroubleCodeAlarmStatus();
	void fSetTroubleCodeAlarmStatus(BOOL bEnable);

	DWORD fGetWaterTemperatureAlarmValue();
	void fSetWaterTemperatureAlarmValue(DWORD dwValue);

	DWORD fGetFatigueDrivingAlarmValue();
	void fSetFatigueDrivingAlarmValue(DWORD dwValue);

	DWORD fGetOverSpeedAlarmValue();
	void fSetOverSpeedAlarmValue(DWORD dwValue);

	DWORD fGetShiftAlarmRpm();
	void fSetShiftAlarmRpm(DWORD dwValue);
	DWORD fGetShiftAlarmVss();
	void fSetShiftAlarmVss(DWORD dwValue);

	DWORD fGetVoltMaxAlarm();
	void fSetVoltMaxAlarm(DWORD dwValue);
	DWORD fGetVoltMinAlarm();
	void fSetVoltMinAlarm(DWORD dwValue);

	DWORD fGetAutoSelectPanelTime();
	void SetAutoSelectPanelTime(DWORD dwValue);

	vector<TagAppInfo> fGetExistGPS();
	vector<TagAppInfo> fGetExistDSA();

	vector<TagAppInfo> fGetStartList();
	void fSetStartList(vector<TagAppInfo> vecAppList);

	vector<CString> fGetFilterAppList();

	CPoint fGetMiniDlgPoint();
	void fSetMiniDlgPoint(CPoint pointNew);

	CString fGetSoundFile(DWORD dwFileID);
private:
	// 应用软件配置信息
	_TagAppConfig m_tagAppConfig;

	vector<TagAppInfo> m_vecGPSList;
	vector<TagAppInfo> m_vecDSAList;

	vector<TagAppInfo> m_vecAppList;

};
