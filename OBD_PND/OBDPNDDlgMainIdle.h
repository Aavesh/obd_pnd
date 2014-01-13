#pragma once
#include "OBDPNDDlgBase.h"

// COBDPNDDlgMainIdle 对话框

class COBDPNDDlgMainIdle : public CDialog,public COBDPNDDlgBase
{
	DECLARE_DYNAMIC(COBDPNDDlgMainIdle)

public:
	COBDPNDDlgMainIdle(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~COBDPNDDlgMainIdle();

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
	//刷新数据
	void fRefreshData();

	// 填充水温图片
	int fDrawFill(CDC * dc,CRect rectCircle,int iPercent);

private:
	//数据值位置
	CRect	m_rectDate;					//日期
	CRect	m_rectTime;					//时间
	CRect	m_rectWeek;					//星期
	CRect	m_rectWater;
	CRect	m_rectWaterTempValue;		//水温值
	CRect	m_rectWaterTempText;		//水温文本

	//数据值字体
	CFont	m_fontDate;					//日期字体
	CFont	m_fontTime;					//时间字体
	CFont	m_fontWeek;					//星期字体
	CFont	m_fontWaterTempValue;		//水温值字体
	CFont	m_fontWaterTempText;		//水温文本字体

	CFont	m_fontDsDataValue;			//数据流数据值字体
	CFont	m_fontDsDataText;			//数据流文本字体

	COLORREF m_clrTempText;
	COLORREF m_clrTempValue;
	COLORREF m_clrTempValueAlarm;

	//数据值文本
	CString m_strDate;
	CString m_strTime;
	CString m_strWeek;
	CString m_strWaterTempValue;
	CString m_strWaterTempText;

	TagDataStreamInfo m_tagDsInfo[6];

	// 按键按下
	int m_iKeyDownTime;
	int m_iKeyDownIndex;
};
