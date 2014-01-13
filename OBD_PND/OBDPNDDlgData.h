#pragma once

#include "OBDPNDDlgBase.h"

// COBDPNDDlgData �Ի���

class COBDPNDDlgData : public CDialog,public COBDPNDDlgBase
{
	DECLARE_DYNAMIC(COBDPNDDlgData)

public:
	COBDPNDDlgData(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~COBDPNDDlgData();

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
	void fInitUI(DWORD dwResID = NULL);
	void fSetBkResID(DWORD dwResID = NULL);

private:
	DWORD m_dwBkResID;

	CRect m_rectWin;

};
