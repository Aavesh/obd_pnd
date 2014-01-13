#pragma once
#include "OBDPNDDlgBase.h"


enum TIP_INFO_TYPE
{
	TIP_INFO_TEXT,
	TIP_INFO_WARN,
	TIP_INFO_ERROR,
};

// COBDPNDDlgTipInfo �Ի���

class COBDPNDDlgTipInfo : public CDialog,public COBDPNDDlgBase
{
	DECLARE_DYNAMIC(COBDPNDDlgTipInfo)

public:
	COBDPNDDlgTipInfo(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~COBDPNDDlgTipInfo();

// �Ի�������
	enum { IDD = IDD_OBD_PND_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	DECLARE_MESSAGE_MAP()
public:
	//��ʼ������λ�ã��������Ϣ
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
