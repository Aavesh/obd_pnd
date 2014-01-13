#pragma once

#include "OBDPNDDlgBase.h"

// COBDPNDDlgMainTour 对话框

class COBDPNDDlgMainTour : public CDialog,public COBDPNDDlgBase
{
	DECLARE_DYNAMIC(COBDPNDDlgMainTour)

public:
	COBDPNDDlgMainTour(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~COBDPNDDlgMainTour();

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
	//画指南针
	int fDrawCompass(CDC * dc,CPoint pointCircle,int iRaduis,double dAngle ,int iLen,int iLen2);
	//画车速指针
	int fDrawPoint(CDC * dc,CRect rectCircle,int iValue);
private:
	//数据值位置
	CRect	m_rectSpeed;				//车速
	CRect	m_rectTime;					//时间
	CRect	m_rectMeterRight;

	//数据值字体
	CFont	m_fontSpeed;				//车速字体
	CFont	m_fontTime;					//时间字体

	CFont	m_fontDsDataValue;			//数据流数据值字体
	CFont	m_fontDsDataText;			//数据流文本字体

	COLORREF m_clrSpeedAlarm;

	//数据值文本
	CString m_strSpeed;
	CString m_strTime;
	DWORD	m_dwSpeed;

	TagDataStreamInfo m_tagDsInfo[6];
	// 按键按下
	int m_iKeyDownTime;
	int m_iKeyDownIndex;

	CPoint	m_pointCompass;
	double	m_fCompassValue;
	double	m_dRaduis;
	WORD	m_wPointLen;
	WORD	m_wCompassPointLenght;
	WORD	m_wCompassPointLenght2;

};
