// OBDPNDDlgSelFile.cpp : 实现文件
//

#include "stdafx.h"
#include "OBD_PND.h"
#include "OBDPNDDlgSelFile.h"
#include "OBD_PNDDlg.h"

// COBDPNDDlgSelFile 对话框

IMPLEMENT_DYNAMIC(COBDPNDDlgSelFile, CDialog)

COBDPNDDlgSelFile::COBDPNDDlgSelFile(CWnd* pParent /*=NULL*/)
	: CDialog(COBDPNDDlgSelFile::IDD, pParent)
{

}

COBDPNDDlgSelFile::~COBDPNDDlgSelFile()
{
}

void COBDPNDDlgSelFile::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
}


BEGIN_MESSAGE_MAP(COBDPNDDlgSelFile, CDialog)
	ON_BN_CLICKED(IDOK, &COBDPNDDlgSelFile::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &COBDPNDDlgSelFile::OnBnClickedCancel)
	ON_LBN_DBLCLK(IDC_LIST1, &COBDPNDDlgSelFile::OnLbnDblclkList1)
	ON_LBN_SELCHANGE(IDC_LIST1, &COBDPNDDlgSelFile::OnLbnSelchangeList1)
	ON_BN_CLICKED(IDC_BUTTON_OK, &COBDPNDDlgSelFile::OnBnClickedButtonOk)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &COBDPNDDlgSelFile::OnBnClickedButtonCancel)
END_MESSAGE_MAP()


// COBDPNDDlgSelFile 消息处理程序

// COBDPNDDlgSelFile 消息处理程序
BOOL COBDPNDDlgSelFile::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString strText = theMainDlg->fGetBinText(DS_BTN_OK);
	GetDlgItem(IDC_BUTTON_OK)->SetWindowText(strText);
	strText = theMainDlg->fGetBinText(DS_BTN_CLOSE);
	GetDlgItem(IDC_BUTTON_CANCEL)->SetWindowText(strText);

	this->CenterWindow();


	CCeFileFind fileFind;

	m_strCurrDirPath = _T("\\");
	this->SetWindowText(m_strCurrDirPath);
	m_vecCurrDirFileList = fFindFile(m_strCurrDirPath);
	m_list.AddString(_T(".."));
	for (int i=0;i<m_vecCurrDirFileList.size();i++)
	{
		m_list.AddString(m_vecCurrDirFileList[i].strFileName);
	}

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void COBDPNDDlgSelFile::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnLbnDblclkList1();
}

void COBDPNDDlgSelFile::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}


vector<TagFILE_PROPERTY> COBDPNDDlgSelFile::fFindFile(CString strDirPath)
{
	CCeFileFind fileFind;
	vector<TagFILE_PROPERTY> vecFileList;

	if (m_strCurrDirPath.Right(1) != _T("\\"))
		m_strCurrDirPath += _T("\\");

	m_strCurrDirPath += _T("*.*");

	BOOL bFound = fileFind.FindFile(m_strCurrDirPath);
	while (bFound)
	{
		bFound = fileFind.FindNextFile();
		CString strName = fileFind.GetFileName().MakeUpper();
		if (fileFind.IsDirectory() || strName.Right(4) == _T(".EXE"))
		{
			TagFILE_PROPERTY tagFileProperty;

			if (fileFind.IsDirectory())
				tagFileProperty.bIsDirectory = TRUE;
			else
				tagFileProperty.bIsDirectory = FALSE;

			tagFileProperty.strFileName = fileFind.GetFileName();
			tagFileProperty.strFilePath = fileFind.GetFilePath();


			vecFileList.push_back(tagFileProperty);
		}		
	}

	return vecFileList;
}
void COBDPNDDlgSelFile::OnLbnDblclkList1()
{
	if (m_list.GetCurSel() == 0)	//上一级目录
	{
		int iPos = m_strCurrDirPath.ReverseFind('\\');
		if (iPos>0)
		{
			m_strCurrDirPath = m_strCurrDirPath.Left(iPos);
			iPos = m_strCurrDirPath.ReverseFind('\\');
			m_strCurrDirPath = m_strCurrDirPath.Left(iPos+1);

			m_vecCurrDirFileList = fFindFile(m_strCurrDirPath);
		}
	}
	else if (m_list.GetCurSel() <= m_vecCurrDirFileList.size() &&
		m_vecCurrDirFileList[m_list.GetCurSel()-1].bIsDirectory)	//选择文件夹
	{
		m_strCurrDirPath = m_vecCurrDirFileList[m_list.GetCurSel()-1].strFilePath;
		m_vecCurrDirFileList = fFindFile(m_strCurrDirPath);
	}
	else	//选择文件
	{		
		m_setSelFilePathName = m_vecCurrDirFileList[m_list.GetCurSel()-1].strFilePath;
		CDialog::OnOK();
	}

	this->SetWindowText(m_strCurrDirPath);

	m_list.ResetContent();
	m_list.AddString(_T(".."));
	for (int i=0;i<m_vecCurrDirFileList.size();i++)
	{
		//CString str;
		//str.Format(_T("%d	%s"),m_vecCurrDirFileList[i].bIsDirectory,m_vecCurrDirFileList[i].strFilePath);
		m_list.AddString(m_vecCurrDirFileList[i].strFileName);
	}
}

void COBDPNDDlgSelFile::OnLbnSelchangeList1()
{
	int         dx=0; 
	CSize       sz; 
	CDC*        pDC   =   m_list.GetDC(); 
	CString		str; 
	for(int j = 0;j < m_list.GetCount();j++) 
	{ 
		m_list.GetText(j,str); 

		sz = pDC-> GetTextExtent(str); 
		if (sz.cx > dx) 
			dx = sz.cx; 
	} 
	m_list.ReleaseDC(pDC); 
	m_list.SetHorizontalExtent(dx);	
}

void COBDPNDDlgSelFile::OnBnClickedButtonOk()
{
	if (m_list.GetCurSel() == 0)	//上一级目录
	{
		int iPos = m_strCurrDirPath.ReverseFind('\\');
		if (iPos>0)
		{
			m_strCurrDirPath = m_strCurrDirPath.Left(iPos);
			iPos = m_strCurrDirPath.ReverseFind('\\');
			m_strCurrDirPath = m_strCurrDirPath.Left(iPos+1);

			m_vecCurrDirFileList = fFindFile(m_strCurrDirPath);
		}
	}
	else if (m_list.GetCurSel() <= m_vecCurrDirFileList.size() &&
		m_vecCurrDirFileList[m_list.GetCurSel()-1].bIsDirectory)	//选择文件夹
	{
		m_strCurrDirPath = m_vecCurrDirFileList[m_list.GetCurSel()-1].strFilePath;
		m_vecCurrDirFileList = fFindFile(m_strCurrDirPath);
	}
	else	//选择文件
	{		
		m_setSelFilePathName = m_vecCurrDirFileList[m_list.GetCurSel()-1].strFilePath;
		CDialog::OnOK();
	}

	this->SetWindowText(m_strCurrDirPath);

	m_list.ResetContent();
	m_list.AddString(_T(".."));
	for (int i=0;i<m_vecCurrDirFileList.size();i++)
	{
		//CString str;
		//str.Format(_T("%d	%s"),m_vecCurrDirFileList[i].bIsDirectory,m_vecCurrDirFileList[i].strFilePath);
		m_list.AddString(m_vecCurrDirFileList[i].strFileName);
	}
}

void COBDPNDDlgSelFile::OnBnClickedButtonCancel()
{
	CDialog::OnCancel();
}
