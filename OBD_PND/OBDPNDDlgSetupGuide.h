#pragma once
#include "OBDPNDDlgBase.h"
#include "OBDPNDConfigReadWrite.h"

enum SETUP_GUIDE_TYPE
{
	SETUP_GUIDE_DEVICE_SET,			//设置向导
	SETUP_GUIDE_TIP_INIT,			//开机提示
	SETUP_GUIDE_TIP_START_CAR,		//发动汽车

	SETUP_GUIDE_FUEL_PRICE,			//油价
	SETUP_GUIDE_SET_ENGINE,			//水温报警
	SETUP_GUIDE_FUEL_CONSUMPTION,	//油耗系数

	SETUP_GUIDE_DSA,				//DSA
	SETUP_GUIDE_GPS,				//GPS
	SETUP_GUIDE_AUTOSTART,			//自动启动

	SETUP_GUIDE_OK,				//OK
};


// COBDPNDDlgSetupGuide 对话框

class COBDPNDDlgSetupGuide : public CDialog,public COBDPNDDlgBase
{
	DECLARE_DYNAMIC(COBDPNDDlgSetupGuide)

public:
	COBDPNDDlgSetupGuide(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~COBDPNDDlgSetupGuide();

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


private:
	DWORD	m_dwType;

	COLORREF	m_clrText;
	CFont		m_fontText;

	CRect		m_rectText;
	CString		m_strText;

	//输入框
	CFont		m_fontEditValue;
	CString		m_strEditText;
	CString		m_strEditValue;
	CRect		m_rectEditText;
	CRect	m_rectEditValue;
	CBitmap m_cBitmapEditValue;
	CDC		m_memdcEditValue;

	//油耗系数
	CRect	m_rectFuel[3][3];
	WORD	m_wFuelIndex;

	
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


	vector<TagAppInfo> m_vecList;

	vector<TagAppInfo> m_vecStartList;
};
