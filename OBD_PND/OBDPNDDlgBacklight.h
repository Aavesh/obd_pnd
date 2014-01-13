#pragma once

#include "OBDPNDDlgBase.h"

#include <windows.h> 

typedef struct tagBACKLIGHTINFO
{
	DWORD UseExt;
	DWORD UseBattery;
	DWORD ACTimeOut;
	DWORD BatteryTimeOut;
	DWORD ACPrescale;
	DWORD BatteryPrescale;
}BACKLIGHTINFO,*PBACKLIGHTINFO;

enum BACKLIGHT_TYPE
{
	BACKLIGHT_DAY,
	BACKLIGHT_NIGHT,
};

// COBDPNDDlgBacklight �Ի���

class COBDPNDDlgBacklight : public CDialog,public COBDPNDDlgBase
{
	DECLARE_DYNAMIC(COBDPNDDlgBacklight)

public:
	COBDPNDDlgBacklight(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~COBDPNDDlgBacklight();

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
	BOOL YFGetBacklightInfo(BACKLIGHTINFO* Backlight);
	BOOL YFSetBacklightInfo(const BACKLIGHTINFO* Backlight);
	void fSetBackLight(WORD wValue);
	int fGetBackLight();
private:
	WORD	m_wBacklight;
	WORD	m_wBacklightDayEnd;
	WORD	m_wBacklightDayStart;

	WORD	m_wBacklightDay;
	WORD	m_wBacklightNight;

	BACKLIGHTINFO   m_curBackLightInfo;

	CRect	m_rectEditStart;
	CBitmap m_bmpEditStart;								
	CDC		m_memdcEditStart;						

	CRect	m_rectEditEnd;
	CBitmap m_bmpEditEnd;						
	CDC		m_memdcEditEnd;		

	BYTE	m_bType;

	BOOL	m_bKeyDown;
	CPoint	m_pointKeyDown;
};
