#pragma once
#include "CeFileFind.h"

// COBDPNDDlgSelFile �Ի���

class COBDPNDDlgSelFile : public CDialog
{
	DECLARE_DYNAMIC(COBDPNDDlgSelFile)

public:
	COBDPNDDlgSelFile(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~COBDPNDDlgSelFile();

// �Ի�������
	enum { IDD = IDD_OBD_PND_DIALOG_SEL_FILE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_list;
	CString m_setSelFilePathName;

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	vector<TagFILE_PROPERTY> fFindFile(CString strDirPath);

	CString m_strCurrDirPath;
	vector<TagFILE_PROPERTY> m_vecCurrDirFileList;

	afx_msg void OnLbnDblclkList1();
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnBnClickedButtonOk();
	afx_msg void OnBnClickedButtonCancel();
};
