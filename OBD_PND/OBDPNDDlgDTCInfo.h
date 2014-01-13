#pragma once

#include "OBDPNDDlgBase.h"
#include "OBDPNDCommunication.h"

// COBDPNDDlgDTCInfo 对话框

class COBDPNDDlgDTCInfo : public CDialog,public COBDPNDDlgBase
{
	DECLARE_DYNAMIC(COBDPNDDlgDTCInfo)

public:
	COBDPNDDlgDTCInfo(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~COBDPNDDlgDTCInfo();

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
	COLORREF	m_clrDtcCode;
	CFont		m_fontDtcCode;
	CString		m_strDtcCode;
	CRect		m_rectCode;
	CRect		m_rectCodeValue;

	COLORREF	m_clrDtcText;
	CFont		m_fontDtcText;
	CString		m_strDtcText;
	CRect		m_rectText;
	CRect		m_rectTextValue;

	map<DWORD,TagDtcInfo> m_mapDtcList;

	map<DWORD,TagDtcInfo>::iterator m_itDtcInfo;
	int m_iIndex;
};
