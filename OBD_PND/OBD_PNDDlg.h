// OBD_PNDDlg.h : ͷ�ļ�
//

#pragma once
#include "Resource.h"
#include "SplashWnd.h"

#include "OBDPNDConfigReadWrite.h"
#include "OBDPNDCommunication.h"

#include "OBDPNDDlgLog.h"
#include "OBDPNDDlgTipInfo.h"
#include "OBDPNDDlgLogo.h"
#include "OBDPNDDlgMini.h"

#include "OBDPNDDlgMainIdle.h"
#include "OBDPNDDlgMainTour.h"
#include "OBDPNDDlgMainRace.h"
#include "OBDPNDDlgMainDetail.h"

#include "OBDPNDDlgDTCInfo.h"

#include "SeriesGPS.h"

#define MAX_FOOT_INDEX					7							// ҳ�Ű�ť���ܿ�

#define WM_OBDPND_MINI_SHOW		WM_USER + 0x0002	//����ǰ��
#define WM_OBDPND_MINI_HIDE		WM_USER + 0x0003	//��������

enum OBDPND_PANEL
{
	OBDPND_PANEL_IDLE,
	OBDPND_PANEL_TOUR,
	OBDPND_PANEL_RACE,
	OBDPND_PANEL_DETAIL,
	OBDPND_PANEL_SET,
	OBDPND_PANEL_BACK,
	OBDPND_PANEL_EXIT,

	OBDPND_PANEL_LOGO,
	OBDPND_PANEL_LOG,
};

enum OBDPND_TIMER
{
	OBDPND_TIMER,
	OBDPND_TIMER_START_COMM,				//��ʼ��ȡ��������
	OBDPND_TIMER_REFRESH_UI_BEGIN,				//ˢ��UI����
	OBDPND_TIMER_REFRESH_UI,				//ˢ��UI����
	OBDPND_TIMER_AUTO_SEL_PANEL,			//�Զ��л����
	OBDPND_TIMER_CHECK_LAST_GET_DATA_TIME,	//�������ȡ����ʱ��
	OBDPND_TIMER_GET_DTC_INFO,				//��ȡ��������Ϣ
	OBDPND_TIMER_AUTO_START_APP,			//�Զ�������������
	OBDPND_TIMER_EXIT,						//�˳����

	OBDPND_TIMER_ALARM_OVERSPEED,			//����
	OBDPND_TIMER_ALARM_COOLANT,				//ˮ��
	OBDPND_TIMER_ALARM_FATIGUE_DRIVING,		//ƣ�ͼ�ʻ
	OBDPND_TIMER_ALARM_FATIGUE_DRIVING_STAY,
	OBDPND_TIMER_ALARM_BATTERY_VOLTAGE,		//��ƿ��ѹ
	OBDPND_TIMER_ALARM_BATTERY_VOLTAGE_STAY,

	OBDPND_TIMER_CHECK_GPS_RUNNING,			//���GPS�Ƿ�����
	OBDPND_TIMER_CHECK_BACKLIGHT,			//��鱳��

};




// COBD_PNDDlg �Ի���
class COBD_PNDDlg : public CDialog
{
// ����
public:
	COBD_PNDDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_OBD_PND_DIALOG };


	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	afx_msg void OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/);
#endif
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg LONG OnMiniShow(WPARAM wParam,LPARAM lParam);
	afx_msg LONG OnMiniHide(WPARAM wParam,LPARAM lParam);
	// ���ڽ������ݴ�����
	afx_msg LONG OnSerialRecvData(WPARAM wParam,LPARAM lParam);
	afx_msg LONG OnSerialRestart(WPARAM wParam,LPARAM lParam);

public:
	// ��ʼ��UI�������
	void fInitUiSetting();

	// ��Bin�ļ���ȡ�ı�
	CString fGetBinText(DWORD dwID);

	// ���ݵ�λ�ı���ȡ��ԴID
	DWORD fGetUnitResID(CString strUnit);

	// ��ǰ���������
	WORD fGetCurrPanelIndex();
	WORD fGetBeforPanelIndex();
	void fSetCurrPanelIndex(WORD wIndex);
	void fSetBeforPanelIndex();
	void fReturnCurrPanelIndex();

	// ����Ӧ�ó�������
	int	fSaveAppData();

	// �˳�ϵͳ
	void fAppExit();

	// �����־
	int fAddLog(CString strText,WORD wType = NULL);

	// ��ʾ��ʾ��Ϣ
	int fShowTipInfo(CString strText,WORD wType = NULL);
	int fHideTipInfo();

	// ������UI��������
	void fRefreshUiData();

	BOOL fIsBackRunning();
	// ��ȡָ����λ��
	float fGetCompassValue();

	// ��ȡ����������
	CString fGetDataStreamValue(DWORD dwDsID);
	float fGetDataStreamValueF(DWORD dwDsID);
	CString fGetDataStreamText(DWORD dwDsID);
	map<DWORD,TagDtcInfo> fGetDtcList();

	void fStopSerial();
	BOOL fStartSeries(int iPortNo);

	void fShowDtcInfo();
private:
	CSplashWnd			* m_pSplashWindow;	//��������
	COBDPNDDlgLog		* m_pDlgLog;		//��־����
	COBDPNDDlgTipInfo	* m_pDlgTip;		//��ʾ����
	COBDPNDDlgMini		* m_pDlgMini;		//mini����

	COBDPNDDlgMainIdle		* m_pDlgMainIdle;	//���ٽ���
	COBDPNDDlgMainTour		* m_pDlgMainTour;	//Ѳ������
	COBDPNDDlgMainRace		* m_pDlgMainRace;	//��������
	COBDPNDDlgMainDetail	* m_pDlgMainDetail;	//��ϸ����

	CSeriesGPS	* m_pSeriesGps;

	WORD				m_wCurrPanelIndex;		//��ǰ��ť����
	WORD				m_wBeforPanelIndex;		//֮ǰѡ���������


	map<DWORD,BOOL> m_mapAlarmStates;	//����״̬
	COBDPNDDlgTipInfo	* m_pDlgAlarmOverSpeed;		//
	COBDPNDDlgTipInfo	* m_pDlgAlarmCoolant;			//
	COBDPNDDlgTipInfo	* m_pDlgAlarmFatigueDriving;			//FATIGUE_DRIVING
	COBDPNDDlgTipInfo	* m_pDlgAlarmBatteryVoltage;			//BATTERY_VOLTAGE
	
	BOOL	m_bVaildComm;		//�Ƿ�ͨѶ����

	float m_fCompassValue;		//ָ�����ֵ

	CString	m_strGpsExe;		//gps��������

	BOOL	m_bIsDay;			//�Ƿ�Ϊ����

	BOOL	m_bUpdateDateTimeByGps;//

	COBDPNDCommunication	* m_pCommunication;
public:
	COBDPNDConfigReadWrite	* m_pConfigReadWrite;

	DWORD		m_dwScreenResolution;	//��Ļ�ֱ���
	CRect		m_rectWin;				//���ڴ�С
	CString		m_strAppTitle;			//Ӧ������

	CRect		m_rectTitle;			//��������С
	CFont		m_fontTitle;			//����������
	COLORREF	m_clrTitle;				//��������ɫ

	CRect		m_rectFoot[MAX_FOOT_INDEX];	//ҳ�Ź�������
	CFont		m_fontFootNormal;			//����״̬����
	CFont		m_fontFootFocus;			//����״̬����
	COLORREF	m_clrFootNormal;			//����״̬��ɫ
	COLORREF	m_clrFootFocus;				//����״̬��ɫ
	CString		m_strFoot[MAX_FOOT_INDEX];

	COLORREF	m_clrText;					//�ı���ɫ

	BOOL	m_bBackRunning;	//��̨����
	BOOL	m_bSetting;		//������

	BOOL	m_bShowLog;


};

extern COBD_PNDDlg * theMainDlg;