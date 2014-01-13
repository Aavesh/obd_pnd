/*
 * Copyright(C) 2013,V-Checker ��������Ȩ����( All rights reserved. )
 * 
 * �ļ����ƣ�OBDRClsConfigReadWrite.h
 * ժ    Ҫ��Ӧ�ó���������Ϣ������
 * ��ǰ�汾��1.0
 * ��    �ߣ�Yipb
 * �������ڣ�2013��12��5��
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

//������Ϣ
enum CONFIG_ERR
{
	CONFIG_OK,
	CONFIG_ERR_FIND_FILE,							//�����������ļ�
	CONFIG_ERR_LOAD_FILE,							//װ�������ļ�ʧ��
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
	WORD	wPort;			//���ں�
	DWORD	dwBPS;			//������
	WORD	wData_Bits;		//����λ
	WORD	wStop_Bits;		//ֹͣλ
	WORD	wParity;		//Ч��

	TagComData()
	{
		wPort = 1;
		dwBPS = 115200;
		wData_Bits = 8;
		wStop_Bits = 0;
		wParity = 0;
	}
};


//�����б�
struct _TagLanguage
{
	CString strLanID;
	CString strLanName;
	CString strLanSN;
	CString strResFiles;
};
//��������
struct _TagSetAlarm
{
	BOOL	bTroubleCode;			//�Ƿ�������
	DWORD	wSpeed;					//���ٱ�������
	DWORD	wCoolant;				//ˮ�±�������
	DWORD	wFatigue_Driving;		//ƣ�ͼ�ʻ�������ã����ӣ�

	double	dbFuel;					//��������

	DWORD	wShiftRpm;				//��������
	DWORD	wShiftSpeed;			//��������

	DWORD	wVoltMin;				//��ѹ�ͱ���
	DWORD	wVoltMax;				//��ѹ�߱���

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

//���ٽ�����������
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

//Ѳ��������������
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

//����������������
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

//Ӧ�����������Ϣ
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

	// ��ʼ��Ӧ��������Ϣ
	BOOL fInitAppConfig(void);
	// ����������Ϣ
	BOOL fSaveAppConfig(void);
	// ����������Ϣ
	BOOL fResetAppConfig(void);
	// ��ȡ���ٽ���������Ϣ
	_TagUiDataIdle fGetIdleUiData();
	_TagUiDataTour fGetTourUiData();
	_TagUiDataRace fGetRaceUiData();
	void fSetIdleUiData(WORD wIndex,DWORD dwCmdID);
	void fSetTourUiData(WORD wIndex,DWORD dwCmdID);
	void fSetRaceUiData(WORD wIndex,DWORD dwCmdID);

	// ��ȡ�ı����ļ�·��
	CString fGetTextBinFileName();	
	// ��ȡOBD����������Ϣ
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

	// ��ȡGPS����������Ϣ
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

	// ��ʱ�ͺ�ϵ��
	double fGetAvgFCC();
	void fSetAvgFCC(double dbValue);
	// ƽ���ͺ�ϵ��
	double fGetNowFCC();
	void fSetNowFCC(double dbValue);
	// �ͼ�
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
	// Ӧ�����������Ϣ
	_TagAppConfig m_tagAppConfig;

	vector<TagAppInfo> m_vecGPSList;
	vector<TagAppInfo> m_vecDSAList;

	vector<TagAppInfo> m_vecAppList;

};
