#pragma once

#include "OBDPNDDlgBase.h"

// COBDPNDDlgConfigAvgFCC �Ի���

class COBDPNDDlgConfigAvgFCC : public CDialog,public COBDPNDDlgBase
{
	DECLARE_DYNAMIC(COBDPNDDlgConfigAvgFCC)

public:
	COBDPNDDlgConfigAvgFCC(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~COBDPNDDlgConfigAvgFCC();

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
	void fSetAvgFuelConmuption(float fSetAvgFuelCon);

private:
	CFont	m_fontText;
	CFont	m_fontText2;
	CFont	m_fontText3;
	CRect	m_rectList[3];

	CString m_strText[3];

	float	m_fSettingAvgFuelCon;
};