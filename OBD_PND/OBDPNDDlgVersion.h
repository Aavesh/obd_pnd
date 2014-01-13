#pragma once
#include "OBDPNDDlgBase.h"


// COBDPNDDlgVersion 对话框

class COBDPNDDlgVersion : public CDialog,public COBDPNDDlgBase
{
	DECLARE_DYNAMIC(COBDPNDDlgVersion)

public:
	COBDPNDDlgVersion(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~COBDPNDDlgVersion();

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

private:
	COLORREF	m_clrText;
	CFont		m_fontText;

	CRect		m_rectText[6][2];
	CString		m_strText[6][2];

	DWORD		m_dwKeyDownTime;
	BOOL		m_bShowHideInfo;

	CRect m_rectCheck[4];
	int m_iCheckIndex;
};
