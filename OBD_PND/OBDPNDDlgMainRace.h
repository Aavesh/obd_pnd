#pragma once
#include "OBDPNDDlgBase.h"

// COBDPNDDlgMainRace �Ի���

class COBDPNDDlgMainRace : public CDialog,public COBDPNDDlgBase
{
	DECLARE_DYNAMIC(COBDPNDDlgMainRace)

public:
	COBDPNDDlgMainRace(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~COBDPNDDlgMainRace();

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
	CRect	m_rectLeftValue;				//����
	CRect	m_rectRightValue;				//ת��
	CRect	m_rectLeftText;
	CRect	m_rectRightText;

	//����ֵ����
	CFont	m_fontValue;			//����ֵ����
	CFont	m_fontText;				//�����ı�����

	CFont	m_fontDsDataValue;			//����������ֵ����
	CFont	m_fontDsDataText;			//�������ı�����

	TagDataStreamInfo m_tagDsInfo[7];
	// ��������
	int m_iKeyDownTime;
	int m_iKeyDownIndex;
};
