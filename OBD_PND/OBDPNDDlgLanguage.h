#pragma once
#include "OBDPNDDlgBase.h"


// COBDPNDDlgLanguage �Ի���

class COBDPNDDlgLanguage : public CDialog,public COBDPNDDlgBase
{
	DECLARE_DYNAMIC(COBDPNDDlgLanguage)

public:
	COBDPNDDlgLanguage(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~COBDPNDDlgLanguage();

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
private:
	//��ʼ������λ�ã��������Ϣ
	void fInitUI();
	void fUpdataByPage();

public:
	CString fGetSelectedLanID();

private:
	CString	m_strGrid[5][3];	//�ı�����	
	CRect	m_rectGrid[5][3];	//�ı�����

	//�ı�����
	CFont   m_fontInfoTitle;
	COLORREF m_clrInfoTitle;		//�ı���Ϣ��ɫ
	CFont   m_fontInfoText;
	COLORREF m_clrInfoText;			//�ı���Ϣ��ɫ
	CFont   m_fontInfoCheck;	

	int		m_iCurrSelectedIndex;
	int		m_iCurrRowIndex;					//��ǰ������
	int		m_iPageCount;
	int		m_iCurrPageIndex;
};
