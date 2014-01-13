#pragma once
#include "OBDPNDDlgBase.h"

// COBDPNDDlgMainRace 对话框

class COBDPNDDlgMainRace : public CDialog,public COBDPNDDlgBase
{
	DECLARE_DYNAMIC(COBDPNDDlgMainRace)

public:
	COBDPNDDlgMainRace(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~COBDPNDDlgMainRace();

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
	//刷新数据
	void fRefreshData();
	//初始化界面位置，字体等信息
	void fInitUI();

private:
	//数据值位置
	CRect	m_rectLeftValue;				//车速
	CRect	m_rectRightValue;				//转速
	CRect	m_rectLeftText;
	CRect	m_rectRightText;

	//数据值字体
	CFont	m_fontValue;			//车速值字体
	CFont	m_fontText;				//车速文本字体

	CFont	m_fontDsDataValue;			//数据流数据值字体
	CFont	m_fontDsDataText;			//数据流文本字体

	TagDataStreamInfo m_tagDsInfo[7];
	// 按键按下
	int m_iKeyDownTime;
	int m_iKeyDownIndex;
};
