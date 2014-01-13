#pragma once

#include "OBDPNDDlgBase.h"

// COBDPNDDlgConfigNowFCC 对话框

class COBDPNDDlgConfigNowFCC : public CDialog,public COBDPNDDlgBase
{
	DECLARE_DYNAMIC(COBDPNDDlgConfigNowFCC)

public:
	COBDPNDDlgConfigNowFCC(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~COBDPNDDlgConfigNowFCC();

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
	void fSetData(WORD wSpeed,double dbFuelCon);
	double fGetNewFuelConCoeff();
private:
	CFont	m_fontText;
	CFont	m_fontText2;
	CFont	m_fontText3;
	CRect	m_rectList[3];

	CString m_strText[4];

	WORD	m_wSetupSpeed;			//设置匀速车速
	double	m_dbSetupFuelCon;			//设置的油耗
	double	m_dbCurrFuelCon;		//当前油耗
	double	m_dbOldFuelConCoeff;	//原本油耗系数
	double	m_dbNewFuelConCoeff;	//计算后的新油耗系数

	WORD	m_wCountTime;		//倒计时计数器
	BOOL	m_bCountDown;
};
