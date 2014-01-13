// OBD_PNDDlg.h : 头文件
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

#define MAX_FOOT_INDEX					7							// 页脚按钮功能块

#define WM_OBDPND_MINI_SHOW		WM_USER + 0x0002	//窗口前置
#define WM_OBDPND_MINI_HIDE		WM_USER + 0x0003	//窗口隐藏

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
	OBDPND_TIMER_START_COMM,				//开始获取串口数据
	OBDPND_TIMER_REFRESH_UI_BEGIN,				//刷新UI界面
	OBDPND_TIMER_REFRESH_UI,				//刷新UI界面
	OBDPND_TIMER_AUTO_SEL_PANEL,			//自动切换面板
	OBDPND_TIMER_CHECK_LAST_GET_DATA_TIME,	//检查最后获取数据时间
	OBDPND_TIMER_GET_DTC_INFO,				//获取到故障信息
	OBDPND_TIMER_AUTO_START_APP,			//自动启动其他程序
	OBDPND_TIMER_EXIT,						//退出软件

	OBDPND_TIMER_ALARM_OVERSPEED,			//超速
	OBDPND_TIMER_ALARM_COOLANT,				//水温
	OBDPND_TIMER_ALARM_FATIGUE_DRIVING,		//疲劳驾驶
	OBDPND_TIMER_ALARM_FATIGUE_DRIVING_STAY,
	OBDPND_TIMER_ALARM_BATTERY_VOLTAGE,		//电瓶电压
	OBDPND_TIMER_ALARM_BATTERY_VOLTAGE_STAY,

	OBDPND_TIMER_CHECK_GPS_RUNNING,			//检查GPS是否运行
	OBDPND_TIMER_CHECK_BACKLIGHT,			//检查背光

};




// COBD_PNDDlg 对话框
class COBD_PNDDlg : public CDialog
{
// 构造
public:
	COBD_PNDDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_OBD_PND_DIALOG };


	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	afx_msg void OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/);
#endif
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg LONG OnMiniShow(WPARAM wParam,LPARAM lParam);
	afx_msg LONG OnMiniHide(WPARAM wParam,LPARAM lParam);
	// 串口接收数据处理函数
	afx_msg LONG OnSerialRecvData(WPARAM wParam,LPARAM lParam);
	afx_msg LONG OnSerialRestart(WPARAM wParam,LPARAM lParam);

public:
	// 初始化UI界面参数
	void fInitUiSetting();

	// 从Bin文件获取文本
	CString fGetBinText(DWORD dwID);

	// 根据单位文本获取资源ID
	DWORD fGetUnitResID(CString strUnit);

	// 当前界面的索引
	WORD fGetCurrPanelIndex();
	WORD fGetBeforPanelIndex();
	void fSetCurrPanelIndex(WORD wIndex);
	void fSetBeforPanelIndex();
	void fReturnCurrPanelIndex();

	// 保存应用程序数据
	int	fSaveAppData();

	// 退出系统
	void fAppExit();

	// 添加日志
	int fAddLog(CString strText,WORD wType = NULL);

	// 显示提示信息
	int fShowTipInfo(CString strText,WORD wType = NULL);
	int fHideTipInfo();

	// 输入下UI界面数据
	void fRefreshUiData();

	BOOL fIsBackRunning();
	// 获取指南针位置
	float fGetCompassValue();

	// 获取数据流数据
	CString fGetDataStreamValue(DWORD dwDsID);
	float fGetDataStreamValueF(DWORD dwDsID);
	CString fGetDataStreamText(DWORD dwDsID);
	map<DWORD,TagDtcInfo> fGetDtcList();

	void fStopSerial();
	BOOL fStartSeries(int iPortNo);

	void fShowDtcInfo();
private:
	CSplashWnd			* m_pSplashWindow;	//开机画面
	COBDPNDDlgLog		* m_pDlgLog;		//日志窗口
	COBDPNDDlgTipInfo	* m_pDlgTip;		//提示窗口
	COBDPNDDlgMini		* m_pDlgMini;		//mini窗口

	COBDPNDDlgMainIdle		* m_pDlgMainIdle;	//怠速界面
	COBDPNDDlgMainTour		* m_pDlgMainTour;	//巡航界面
	COBDPNDDlgMainRace		* m_pDlgMainRace;	//竞技界面
	COBDPNDDlgMainDetail	* m_pDlgMainDetail;	//明细界面

	CSeriesGPS	* m_pSeriesGps;

	WORD				m_wCurrPanelIndex;		//当前按钮索引
	WORD				m_wBeforPanelIndex;		//之前选择的主界面


	map<DWORD,BOOL> m_mapAlarmStates;	//报警状态
	COBDPNDDlgTipInfo	* m_pDlgAlarmOverSpeed;		//
	COBDPNDDlgTipInfo	* m_pDlgAlarmCoolant;			//
	COBDPNDDlgTipInfo	* m_pDlgAlarmFatigueDriving;			//FATIGUE_DRIVING
	COBDPNDDlgTipInfo	* m_pDlgAlarmBatteryVoltage;			//BATTERY_VOLTAGE
	
	BOOL	m_bVaildComm;		//是否通讯正常

	float m_fCompassValue;		//指南针的值

	CString	m_strGpsExe;		//gps进程名字

	BOOL	m_bIsDay;			//是否为白天

	BOOL	m_bUpdateDateTimeByGps;//

	COBDPNDCommunication	* m_pCommunication;
public:
	COBDPNDConfigReadWrite	* m_pConfigReadWrite;

	DWORD		m_dwScreenResolution;	//屏幕分辨率
	CRect		m_rectWin;				//窗口大小
	CString		m_strAppTitle;			//应用名称

	CRect		m_rectTitle;			//标题栏大小
	CFont		m_fontTitle;			//标题栏字体
	COLORREF	m_clrTitle;				//标题栏颜色

	CRect		m_rectFoot[MAX_FOOT_INDEX];	//页脚功能区域
	CFont		m_fontFootNormal;			//正常状态字体
	CFont		m_fontFootFocus;			//焦点状态字体
	COLORREF	m_clrFootNormal;			//正常状态颜色
	COLORREF	m_clrFootFocus;				//焦点状态颜色
	CString		m_strFoot[MAX_FOOT_INDEX];

	COLORREF	m_clrText;					//文本颜色

	BOOL	m_bBackRunning;	//后台运行
	BOOL	m_bSetting;		//设置中

	BOOL	m_bShowLog;


};

extern COBD_PNDDlg * theMainDlg;