#pragma once
#include "OBDPNDDlgBase.h"

// COBDPNDDlgConfigMain 对话框

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
	COBDPNDDlgConfigMain(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~COBDPNDDlgConfigMain();

// 对话框数据
	enum { IDD = IDD_OBD_PND_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	DECLARE_MESSAGE_MAP()
public:
	//初始化界面位置，字体等信息
	void fInitUI();
	// 翻页
	void fSetPage(int iPageIndex);
	// 设置该页的图标
	void fSetTextImg(DWORD dwIndex,DWORD dwTextResID,DWORD dwImgResID);


private:
	CRect m_rectImg[10];
	CRect m_rectText[10];
	CString m_strText[10];
	DWORD m_dwTextImgID[10];	//设置每个位置的资源图片ID
	CFont m_fontText;

	//已经装载的图片信息
	TagTextImg m_tagImg[40];

	int m_dwPageCount;
	int m_dwCurrPageIndex;
	int m_iCurrFunIndex;

	CRect m_rectCheck[4];
	int m_iCheckIndex;
};
