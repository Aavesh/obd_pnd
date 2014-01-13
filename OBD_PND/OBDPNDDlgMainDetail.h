#pragma once
#include "OBDPNDDlgBase.h"

// COBDPNDDlgMainDetail �Ի���

class COBDPNDDlgMainDetail : public CDialog,public COBDPNDDlgBase
{
	DECLARE_DYNAMIC(COBDPNDDlgMainDetail)

public:
	COBDPNDDlgMainDetail(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~COBDPNDDlgMainDetail();

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
	//ˢ������
	void fRefreshData();
	//��ʼ������λ�ã��������Ϣ
	void fInitUI();

private:
	//����ֵλ��
	CRect	m_rectText[10];
	CRect	m_rectValue[10];

	//����ֵ����
	CFont	m_fontText;			//����
	CFont	m_fontValue;

	CString m_strText[10];
	CString m_strValue[10];
};
