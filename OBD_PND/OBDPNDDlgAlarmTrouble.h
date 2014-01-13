#pragma once


#include "OBDPNDDlgBase.h"


// COBDPNDDlgAlarmTrouble �Ի���

class COBDPNDDlgAlarmTrouble : public CDialog,public COBDPNDDlgBase
{
	DECLARE_DYNAMIC(COBDPNDDlgAlarmTrouble)

public:
	COBDPNDDlgAlarmTrouble(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~COBDPNDDlgAlarmTrouble();

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

	BOOL fIsOpenAlarmTourble();

private:
	BOOL		m_bIsOpenAlarmTourble;
	COLORREF	m_clrText;
	CFont		m_fontText;
	CRect		m_rectText;

	COLORREF	m_clrStatus;
	CFont		m_fontStatus;
	CRect		m_rectStatus;

};
