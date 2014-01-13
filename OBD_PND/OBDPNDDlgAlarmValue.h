#pragma once
#include "OBDPNDDlgBase.h"

//��������
enum VALUE_TYPE
{
	ALARM_VALUE_OVERSPEED,			//����
	ALARM_VALUE_COOLANT,			//ˮ��
	ALARM_VALUE_FATIGUE_DRIVING,	//ƣ�ͼ�ʻ
	ALARM_VALUE_BATTERY_VOLTAGE,	//��ƿ��ѹ

	CONFIG_FUEL_CALIBRATION,//�ͺ�У׼
	CONFIT_FUEL_PRICE,		//�����ͼ�

	CONFIG_FUEL_NOW_SETUP, //��ʱ�ͺ�У׼
	CONFIG_FUEL_AVG_SETUP, //ƽ���ͺ�У׼

};
// COBDPNDDlgAlarmValue �Ի���

class COBDPNDDlgAlarmValue : public CDialog,public COBDPNDDlgBase
{
	DECLARE_DYNAMIC(COBDPNDDlgAlarmValue)

public:
	COBDPNDDlgAlarmValue(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~COBDPNDDlgAlarmValue();

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
	void fSetAlarmType(DWORD dwAlarmType);
	void fSetDecLen(WORD wDecLen = 0);
	DWORD fGetValue1();
	DWORD fGetValue2();
private:
	DWORD		m_dwAlarmType;		//��������

	CFont		m_fontText;			//�ı�����
	COLORREF	m_clrText;			//�ı���Ϣ��ɫ
	CString		m_strText[2][2];	//�ı�����	
	CRect		m_rectText[2][2];	//�ı�λ��

	//�����
	CBitmap		m_cBitmapEdit[2];
	CDC			m_memdcEdit[2];

	//��ť����
	CFont	m_fontNumber;
	int		m_iCurrNumberIndex;
	CRect	m_rectNumber[11];

	//�Ƿ�����С����С������
	BOOL	m_bEnableDec;
	WORD	m_wDecLen;

	//�л�������
	BOOL	m_bSwitch;
	COLORREF m_clrSwitch;



};
