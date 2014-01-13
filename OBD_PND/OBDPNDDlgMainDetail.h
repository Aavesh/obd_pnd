#pragma once
#include "OBDPNDDlgBase.h"

// COBDPNDDlgMainDetail 对话框

class COBDPNDDlgMainDetail : public CDialog,public COBDPNDDlgBase
{
	DECLARE_DYNAMIC(COBDPNDDlgMainDetail)

public:
	COBDPNDDlgMainDetail(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~COBDPNDDlgMainDetail();

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
	CRect	m_rectText[10];
	CRect	m_rectValue[10];

	//数据值字体
	CFont	m_fontText;			//字体
	CFont	m_fontValue;

	CString m_strText[10];
	CString m_strValue[10];
};
