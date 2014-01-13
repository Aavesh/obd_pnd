#pragma once
#include "OBDPNDDlgBase.h"
#include "OBDPNDConfigReadWrite.h"

enum SETUP_GUIDE_TYPE
{
	SETUP_GUIDE_DEVICE_SET,			//������
	SETUP_GUIDE_TIP_INIT,			//������ʾ
	SETUP_GUIDE_TIP_START_CAR,		//��������

	SETUP_GUIDE_FUEL_PRICE,			//�ͼ�
	SETUP_GUIDE_SET_ENGINE,			//ˮ�±���
	SETUP_GUIDE_FUEL_CONSUMPTION,	//�ͺ�ϵ��

	SETUP_GUIDE_DSA,				//DSA
	SETUP_GUIDE_GPS,				//GPS
	SETUP_GUIDE_AUTOSTART,			//�Զ�����

	SETUP_GUIDE_OK,				//OK
};


// COBDPNDDlgSetupGuide �Ի���

class COBDPNDDlgSetupGuide : public CDialog,public COBDPNDDlgBase
{
	DECLARE_DYNAMIC(COBDPNDDlgSetupGuide)

public:
	COBDPNDDlgSetupGuide(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~COBDPNDDlgSetupGuide();

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


private:
	DWORD	m_dwType;

	COLORREF	m_clrText;
	CFont		m_fontText;

	CRect		m_rectText;
	CString		m_strText;

	//�����
	CFont		m_fontEditValue;
	CString		m_strEditText;
	CString		m_strEditValue;
	CRect		m_rectEditText;
	CRect	m_rectEditValue;
	CBitmap m_cBitmapEditValue;
	CDC		m_memdcEditValue;

	//�ͺ�ϵ��
	CRect	m_rectFuel[3][3];
	WORD	m_wFuelIndex;

	
	// DAS GPS
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


	vector<TagAppInfo> m_vecList;

	vector<TagAppInfo> m_vecStartList;
};
