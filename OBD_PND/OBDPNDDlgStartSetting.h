#pragma once
#include "OBDPNDDlgBase.h"
#include "OBDPNDConfigReadWrite.h"

// COBDPNDDlgStartSetting 对话框

class COBDPNDDlgStartSetting : public CDialog,public COBDPNDDlgBase
{
	DECLARE_DYNAMIC(COBDPNDDlgStartSetting)

public:
	COBDPNDDlgStartSetting(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~COBDPNDDlgStartSetting();

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
	vector<TagAppInfo> fGetStartList();

private:	
	// DAS GPS
	CString	m_strGrid[5][3];	//文本内容	
	CRect	m_rectGrid[5][3];	//文本区域

	//文本字体
	CFont   m_fontInfoTitle;
	COLORREF m_clrInfoTitle;		//文本信息颜色
	CFont   m_fontInfoText;
	COLORREF m_clrInfoText;			//文本信息颜色
	CFont   m_fontInfoCheck;	

	int		m_iCurrSelectedIndex;
	int		m_iCurrRowIndex;					//当前行索引
	int		m_iPageCount;
	int		m_iCurrPageIndex;

	vector<TagAppInfo> m_vecAppList;
};
