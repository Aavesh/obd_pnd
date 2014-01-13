#pragma once
#include "OBDPNDDlgBase.h"


// COBDPNDDlgVersion �Ի���

class COBDPNDDlgVersion : public CDialog,public COBDPNDDlgBase
{
	DECLARE_DYNAMIC(COBDPNDDlgVersion)

public:
	COBDPNDDlgVersion(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~COBDPNDDlgVersion();

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
