#pragma once
#include "OBDPNDDlgBase.h"

// COBDPNDDlgMainIdle �Ի���

class COBDPNDDlgMainIdle : public CDialog,public COBDPNDDlgBase
{
	DECLARE_DYNAMIC(COBDPNDDlgMainIdle)

public:
	COBDPNDDlgMainIdle(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~COBDPNDDlgMainIdle();

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
	//ˢ������
	void fRefreshData();

	// ���ˮ��ͼƬ
	int fDrawFill(CDC * dc,CRect rectCircle,int iPercent);

private:
	//����ֵλ��
	CRect	m_rectDate;					//����
	CRect	m_rectTime;					//ʱ��
	CRect	m_rectWeek;					//����
	CRect	m_rectWater;
	CRect	m_rectWaterTempValue;		//ˮ��ֵ
	CRect	m_rectWaterTempText;		//ˮ���ı�

	//����ֵ����
	CFont	m_fontDate;					//��������
	CFont	m_fontTime;					//ʱ������
	CFont	m_fontWeek;					//��������
	CFont	m_fontWaterTempValue;		//ˮ��ֵ����
	CFont	m_fontWaterTempText;		//ˮ���ı�����

	CFont	m_fontDsDataValue;			//����������ֵ����
	CFont	m_fontDsDataText;			//�������ı�����

	COLORREF m_clrTempText;
	COLORREF m_clrTempValue;
	COLORREF m_clrTempValueAlarm;

	//����ֵ�ı�
	CString m_strDate;
	CString m_strTime;
	CString m_strWeek;
	CString m_strWaterTempValue;
	CString m_strWaterTempText;

	TagDataStreamInfo m_tagDsInfo[6];

	// ��������
	int m_iKeyDownTime;
	int m_iKeyDownIndex;
};
