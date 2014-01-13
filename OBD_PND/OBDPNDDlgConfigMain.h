#pragma once
#include "OBDPNDDlgBase.h"

// COBDPNDDlgConfigMain �Ի���

struct TagTextImg
{
	DWORD dwResID;
	CBitmap cBmpUnit;							
	CDC		memdcUnit;	

	TagTextImg()
	{
		dwResID = NULL;
	}
};
class COBDPNDDlgConfigMain : public CDialog,public COBDPNDDlgBase
{
	DECLARE_DYNAMIC(COBDPNDDlgConfigMain)

public:
	COBDPNDDlgConfigMain(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~COBDPNDDlgConfigMain();

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
	// ��ҳ
	void fSetPage(int iPageIndex);
	// ���ø�ҳ��ͼ��
	void fSetTextImg(DWORD dwIndex,DWORD dwTextResID,DWORD dwImgResID);


private:
	CRect m_rectImg[10];
	CRect m_rectText[10];
	CString m_strText[10];
	DWORD m_dwTextImgID[10];	//����ÿ��λ�õ���ԴͼƬID
	CFont m_fontText;

	//�Ѿ�װ�ص�ͼƬ��Ϣ
	TagTextImg m_tagImg[40];

	int m_dwPageCount;
	int m_dwCurrPageIndex;
	int m_iCurrFunIndex;

	CRect m_rectCheck[4];
	int m_iCheckIndex;
};
