#pragma once

#include "OBDPNDDlgBase.h"

// COBDPNDDlgMini �Ի���

class COBDPNDDlgMini : public CDialog,public COBDPNDDlgBase
{
	DECLARE_DYNAMIC(COBDPNDDlgMini)

public:
	COBDPNDDlgMini(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~COBDPNDDlgMini();

// �Ի�������
	enum { IDD = IDD_OBD_PND_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
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
	//��ʼ������λ�ã��������Ϣ
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
