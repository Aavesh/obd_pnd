#pragma once

#include "OBDPNDDlgBase.h"

// COBDPNDDlgMainTour �Ի���

class COBDPNDDlgMainTour : public CDialog,public COBDPNDDlgBase
{
	DECLARE_DYNAMIC(COBDPNDDlgMainTour)

public:
	COBDPNDDlgMainTour(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~COBDPNDDlgMainTour();

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
	//��ָ����
	int fDrawCompass(CDC * dc,CPoint pointCircle,int iRaduis,double dAngle ,int iLen,int iLen2);
	//������ָ��
	int fDrawPoint(CDC * dc,CRect rectCircle,int iValue);
private:
	//����ֵλ��
	CRect	m_rectSpeed;				//����
	CRect	m_rectTime;					//ʱ��
	CRect	m_rectMeterRight;

	//����ֵ����
	CFont	m_fontSpeed;				//��������
	CFont	m_fontTime;					//ʱ������

	CFont	m_fontDsDataValue;			//����������ֵ����
	CFont	m_fontDsDataText;			//�������ı�����

	COLORREF m_clrSpeedAlarm;

	//����ֵ�ı�
	CString m_strSpeed;
	CString m_strTime;
	DWORD	m_dwSpeed;

	TagDataStreamInfo m_tagDsInfo[6];
	// ��������
	int m_iKeyDownTime;
	int m_iKeyDownIndex;

	CPoint	m_pointCompass;
	double	m_fCompassValue;
	double	m_dRaduis;
	WORD	m_wPointLen;
	WORD	m_wCompassPointLenght;
	WORD	m_wCompassPointLenght2;

};
