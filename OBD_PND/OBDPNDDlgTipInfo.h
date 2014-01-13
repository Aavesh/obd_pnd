#pragma once
#include "OBDPNDDlgBase.h"


enum TIP_INFO_TYPE
{
	TIP_INFO_TEXT,
	TIP_INFO_WARN,
	TIP_INFO_ERROR,
};

// COBDPNDDlgTipInfo 对话框

class COBDPNDDlgTipInfo : public CDialog,public COBDPNDDlgBase
{
	DECLARE_DYNAMIC(COBDPNDDlgTipInfo)

public:
	COBDPNDDlgTipInfo(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~COBDPNDDlgTipInfo();

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

	void fSetTipText(CString strText,WORD wType = TIP_INFO_TEXT);
	void fSetSoundFile(CString strSoundFile);

	void fShowTipText(CString strText=_T(""),WORD wType = TIP_INFO_TEXT);
	void fHideTipText();

private:
	CRect m_rect;
	CRect m_rectTipText;
	CFont m_fontText;
	DWORD m_dwTipType;

	COLORREF m_clrTextNormal;
	COLORREF m_clrTextWarn;
	COLORREF m_clrTextError;

	CString m_strTipText;
	CString m_strSoundFile;

	WORD	m_wCount;
};
