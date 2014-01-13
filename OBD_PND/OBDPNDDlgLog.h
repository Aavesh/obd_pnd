#pragma once
#include "afxwin.h"

struct TagLogInfo
{
	DWORD	dwDateTime;
	CString	strLogMsg;
	WORD	wLogType;
};
enum LogType
{
	LOG_TYPE_TEXT,
	LOG_TYPE_OBD_DATA,
	LOG_TYPE_GPS_DATA,
	LOG_TYPE_WARN,
	LOG_TYPE_ERROR,
};
#define MAX_LOG_SIZE				300			//保存最近操作日志

// COBDPNDDlgLog 对话框

class COBDPNDDlgLog : public CDialog
{
	DECLARE_DYNAMIC(COBDPNDDlgLog)

public:
	COBDPNDDlgLog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~COBDPNDDlgLog();

// 对话框数据
	enum { IDD = IDD_OBD_PND_DIALOG_LOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
private:
	CListBox	m_listLog;
	BOOL		m_bEnableLog;	//是否开启日志
	BOOL		m_bShowLog;		//是否显示日志

	vector<TagLogInfo>	m_vecLogInfo;

public:
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnLbnDblclkList1();

	void fAddLog(CString strMsg,WORD wMsgType=0);
	void fSaveLog();
	afx_msg void OnBnClickedButtonExit();
	afx_msg void OnBnClickedButtonCom();
	afx_msg void OnBnClickedButtonSave();

	void fSetShowLog(BOOL bShow);
	afx_msg void OnBnClickedButtonReset();
	CButton m_chkSeriesData;
	afx_msg void OnBnClickedCheck1();
	CButton m_chkGpsData;
	afx_msg void OnBnClickedCheck2();
};
