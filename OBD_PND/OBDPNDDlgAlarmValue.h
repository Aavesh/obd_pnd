#pragma once
#include "OBDPNDDlgBase.h"

//报警类型
enum VALUE_TYPE
{
	ALARM_VALUE_OVERSPEED,			//超速
	ALARM_VALUE_COOLANT,			//水温
	ALARM_VALUE_FATIGUE_DRIVING,	//疲劳驾驶
	ALARM_VALUE_BATTERY_VOLTAGE,	//电瓶电压

	CONFIG_FUEL_CALIBRATION,//油耗校准
	CONFIT_FUEL_PRICE,		//设置油价

	CONFIG_FUEL_NOW_SETUP, //即时油耗校准
	CONFIG_FUEL_AVG_SETUP, //平均油耗校准

};
// COBDPNDDlgAlarmValue 对话框

class COBDPNDDlgAlarmValue : public CDialog,public COBDPNDDlgBase
{
	DECLARE_DYNAMIC(COBDPNDDlgAlarmValue)

public:
	COBDPNDDlgAlarmValue(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~COBDPNDDlgAlarmValue();

// 对话框数据
	enum { IDD = IDD_OBD_PND_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	DECLARE_MESSAGE_MAP()
public:
	//初始化界面位置，字体等信息
	void fInitUI();
	void fSetAlarmType(DWORD dwAlarmType);
	void fSetDecLen(WORD wDecLen = 0);
	DWORD fGetValue1();
	DWORD fGetValue2();
private:
	DWORD		m_dwAlarmType;		//报警类型

	CFont		m_fontText;			//文本字体
	COLORREF	m_clrText;			//文本信息颜色
	CString		m_strText[2][2];	//文本内容	
	CRect		m_rectText[2][2];	//文本位置

	//输入框
	CBitmap		m_cBitmapEdit[2];
	CDC			m_memdcEdit[2];

	//按钮数字
	CFont	m_fontNumber;
	int		m_iCurrNumberIndex;
	CRect	m_rectNumber[11];

	//是否允许小数，小数长度
	BOOL	m_bEnableDec;
	WORD	m_wDecLen;

	//切换上下行
	BOOL	m_bSwitch;
	COLORREF m_clrSwitch;



};
