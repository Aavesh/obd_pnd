#pragma once

#include "OBDPNDDlgBase.h"

// COBDPNDDlgShowText �Ի���

class COBDPNDDlgShowText : public CDialog,public COBDPNDDlgBase
{
	DECLARE_DYNAMIC(COBDPNDDlgShowText)

public:
	COBDPNDDlgShowText(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~COBDPNDDlgShowText();

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
	void fSetTextList(vector<CString> vecList);

private:
	COLORREF	m_clrText;
	CFont		m_fontText;
	CRect		m_rectText;
	CString		m_strText;

	vector<CString> m_vecText;
	int			m_iTextIndex;

};
