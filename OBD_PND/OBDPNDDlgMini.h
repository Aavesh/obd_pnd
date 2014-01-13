#pragma once

#include "OBDPNDDlgBase.h"

// COBDPNDDlgMini 对话框

class COBDPNDDlgMini : public CDialog,public COBDPNDDlgBase
{
	DECLARE_DYNAMIC(COBDPNDDlgMini)

public:
	COBDPNDDlgMini(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~COBDPNDDlgMini();

// 对话框数据
	enum { IDD = IDD_OBD_PND_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()
public:
	//初始化界面位置，字体等信息
	void fInitUI();
	void fShowDlg(BOOL bShow);
private:
	CRect		m_rectWin;

	COLORREF	m_clrText;
	CFont		m_fontText;
	CRect		m_rectText;

	COLORREF	m_clrData;
	CFont		m_fontData;
	CRect		m_rectData;

	CString		m_strText;
	CString		m_strData;

	DWORD	m_keyDownTime1;
	DWORD	m_keyDownTime2;

	WORD	m_wCount;

};
