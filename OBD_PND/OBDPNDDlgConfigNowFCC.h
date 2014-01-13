#pragma once

#include "OBDPNDDlgBase.h"

// COBDPNDDlgConfigNowFCC �Ի���

class COBDPNDDlgConfigNowFCC : public CDialog,public COBDPNDDlgBase
{
	DECLARE_DYNAMIC(COBDPNDDlgConfigNowFCC)

public:
	COBDPNDDlgConfigNowFCC(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~COBDPNDDlgConfigNowFCC();

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
	void fSetData(WORD wSpeed,double dbFuelCon);
	double fGetNewFuelConCoeff();
private:
	CFont	m_fontText;
	CFont	m_fontText2;
	CFont	m_fontText3;
	CRect	m_rectList[3];

	CString m_strText[4];

	WORD	m_wSetupSpeed;			//�������ٳ���
	double	m_dbSetupFuelCon;			//���õ��ͺ�
	double	m_dbCurrFuelCon;		//��ǰ�ͺ�
	double	m_dbOldFuelConCoeff;	//ԭ���ͺ�ϵ��
	double	m_dbNewFuelConCoeff;	//���������ͺ�ϵ��

	WORD	m_wCountTime;		//����ʱ������
	BOOL	m_bCountDown;
};
