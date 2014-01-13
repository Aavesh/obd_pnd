// OBDPNDDlgLog.cpp : 实现文件
//

#include "stdafx.h"
#include "OBD_PND.h"
#include "OBDPNDDlgLog.h"
#include "OBD_PNDDlg.h"

// COBDPNDDlgLog 对话框

IMPLEMENT_DYNAMIC(COBDPNDDlgLog, CDialog)

COBDPNDDlgLog::COBDPNDDlgLog(CWnd* pParent /*=NULL*/)
	: CDialog(COBDPNDDlgLog::IDD, pParent)
{
	m_bEnableLog = TRUE;
	m_bShowLog = TRUE;


}

COBDPNDDlgLog::~COBDPNDDlgLog()
{
}

void COBDPNDDlgLog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listLog);
	DDX_Control(pDX, IDC_CHECK1, m_chkSeriesData);
	DDX_Control(pDX, IDC_CHECK2, m_chkGpsData);
}


BEGIN_MESSAGE_MAP(COBDPNDDlgLog, CDialog)
	ON_LBN_DBLCLK(IDC_LIST1, &COBDPNDDlgLog::OnLbnDblclkList1)
	ON_LBN_SELCHANGE(IDC_LIST1, &COBDPNDDlgLog::OnLbnSelchangeList1)
	ON_BN_CLICKED(IDC_BUTTON_EXIT, &COBDPNDDlgLog::OnBnClickedButtonExit)
	ON_BN_CLICKED(IDC_BUTTON_COM, &COBDPNDDlgLog::OnBnClickedButtonCom)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &COBDPNDDlgLog::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &COBDPNDDlgLog::OnBnClickedButtonReset)
	ON_BN_CLICKED(IDC_CHECK1, &COBDPNDDlgLog::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &COBDPNDDlgLog::OnBnClickedCheck2)
END_MESSAGE_MAP()


// COBDPNDDlgLog 消息处理程序

BOOL COBDPNDDlgLog::OnInitDialog()
{
	CDialog::OnInitDialog();

	this->MoveWindow(theMainDlg->m_rectWin);

	CRect rect;
	m_listLog.GetWindowRect(rect);
	rect.left = theMainDlg->m_rectWin.left + 3;
	rect.right = theMainDlg->m_rectWin.right - 3;
	rect.bottom = theMainDlg->m_rectWin.bottom - 3;

	m_listLog.MoveWindow(rect);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void COBDPNDDlgLog::fSetShowLog(BOOL bShow)
{
	m_bShowLog = bShow;
}

void COBDPNDDlgLog::fAddLog(CString strMsg,WORD wMsgType)
{
	if (theMainDlg->m_pConfigReadWrite->fGetSaveErrLogStatus())
	{
		CTime time = CTime::GetCurrentTime();
		CString strTime;
		strTime.Format(_T("%02d:%02d:%02d "),time.GetHour(),time.GetMinute(),time.GetSecond());
		if (m_bShowLog) 
		{
			switch (wMsgType)
			{
			case LOG_TYPE_OBD_DATA:
				{
					if (m_chkSeriesData.GetCheck())
					{
						if (m_listLog.GetCount() > MAX_LOG_SIZE)
						{
							m_listLog.DeleteString(MAX_LOG_SIZE);			
						}	
						m_listLog.InsertString(0,strTime+strMsg);	
					}
				}
				break;
			case LOG_TYPE_GPS_DATA:
				{
					if (m_chkGpsData.GetCheck())
					{
						if (m_listLog.GetCount() > MAX_LOG_SIZE)
						{
							m_listLog.DeleteString(MAX_LOG_SIZE);			
						}	
						m_listLog.InsertString(0,strTime+strMsg);	
					}
				}
				break;
			default:
				{
					if (m_listLog.GetCount() > MAX_LOG_SIZE)
					{
						m_listLog.DeleteString(MAX_LOG_SIZE);
					}	
					m_listLog.InsertString(0,strTime+strMsg);
				}
				break;				
			}
		}

		//保存日志记录
		TagLogInfo tagLogInfo;
		tagLogInfo.dwDateTime = GetTickCount();
		tagLogInfo.strLogMsg = strTime+strMsg;
		tagLogInfo.wLogType = wMsgType;
		if (m_vecLogInfo.size() == MAX_LOG_SIZE)
		{
			for (UINT i=0;i<m_vecLogInfo.size()-1;i++)
			{
				m_vecLogInfo[i] = m_vecLogInfo[i+1];
			}
			m_vecLogInfo[MAX_LOG_SIZE-1] = tagLogInfo;
		}
		else
		{
			m_vecLogInfo.push_back(tagLogInfo);
		}

	}
}

void COBDPNDDlgLog::fSaveLog()
{
	CString strLogFileName;
	CTime time = CTime::GetCurrentTime();

	strLogFileName.Format(_T("%s\\OBD_PND%04d%02d%02d%02d%02d%02d.txt"),
		fCommGetAppPath(),time.GetYear(),time.GetMonth(),time.GetDay(),time.GetHour(),time.GetMinute(),time.GetSecond());

	char * bufData = NULL;
	CFile   file;   
	CString strCur;	
	WORD wStrLen = 0;

	if(!file.Open(strLogFileName,CFile::modeReadWrite))   
	{   
		file.Open(strLogFileName,CFile::modeCreate|CFile::modeReadWrite);  		
	}   

	file.SeekToEnd();

	for (int i=0;i<m_vecLogInfo.size();i++)
	{
		strCur = m_vecLogInfo[i].strLogMsg;
		wStrLen = strCur.GetLength()*2+1;
		bufData = new char[wStrLen];
		ZeroMemory(bufData,wStrLen);
		fCommW2C(strCur.GetBuffer(strCur.GetLength()),bufData,wStrLen);
		file.Write(bufData, wStrLen); 
		file.Write("\r\n",2); 					
	}

	//for (int i=0;i<m_listLog.GetCount();i++)
	//{
	//	m_listLog.GetText(i,strCur);
	//	//if (strCur != _T(""))
	//	{
	//		wStrLen = strCur.GetLength()*2+1;
	//		bufData = new char[wStrLen];
	//		ZeroMemory(bufData,wStrLen);
	//		//fW2C(bufData,strCur.GetBuffer(strCur.GetLength()),wStrLen);
	//		fCommW2C(strCur.GetBuffer(strCur.GetLength()),bufData,wStrLen);
	//		file.Write(bufData, wStrLen); 
	//		file.Write("\r\n",2); 		
	//	}		
	//}


	file.Write("\r\n",2); 	
	file.Write("\r\n",2); 

	file.Close(); 

	delete bufData;
	bufData = NULL;
}


void COBDPNDDlgLog::OnLbnSelchangeList1()
{
	int         dx=0; 
	CSize       sz; 
	CDC*        pDC   =   m_listLog.GetDC(); 
	CString		str; 
	for(int j = 0;j < m_listLog.GetCount();j++) 
	{ 
		m_listLog.GetText(j,str); 

		sz = pDC-> GetTextExtent(str); 
		if (sz.cx > dx) 
			dx = sz.cx; 
	} 
	m_listLog.ReleaseDC(pDC); 
	m_listLog.SetHorizontalExtent(dx);	
}

void COBDPNDDlgLog::OnLbnDblclkList1()
{
	// TODO: 在此添加控件通知处理程序代码

}
void COBDPNDDlgLog::OnBnClickedButtonExit()
{ 
	// TODO: 在此添加控件通知处理程序代码

	m_bShowLog = FALSE;

	theMainDlg->fReturnCurrPanelIndex();
}

void COBDPNDDlgLog::OnBnClickedButtonCom()
{
	// TODO: 在此添加控件通知处理程序代码

	m_bShowLog = !m_bShowLog;
	//// 初始化通讯模块
	//if (theMainDlg->m_pCommunication == NULL)
	//{
	//	theMainDlg->m_pCommunication = new COBDPNDCommunication();
	//}	
	//if(!theMainDlg->m_pCommunication->fStartSeries(theMainDlg->m_pConfigReadWrite->fGetOBDComPortNo()) ) 
	//{
	//	fAddLog(_T("打开串口失败，检测可用串口..."));
	//	int iPortNo =theMainDlg-> m_pCommunication->fCheckVaildSerialPort();
	//	if(iPortNo >= 0)
	//	{
	//		theMainDlg->m_pConfigReadWrite->fSetOBDComPortNo(iPortNo);
	//		theMainDlg->m_pCommunication->fStartSeries(iPortNo);
	//	}
	//}
}

void COBDPNDDlgLog::OnBnClickedButtonSave()
{
	// TODO: 在此添加控件通知处理程序代码
	fSaveLog();
}

void COBDPNDDlgLog::OnBnClickedButtonReset()
{
	// TODO: 在此添加控件通知处理程序代码
	theMainDlg->fStopSerial();
	theMainDlg->fStartSeries(theMainDlg->m_pConfigReadWrite->fGetOBDComPortNo());
}

void COBDPNDDlgLog::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL b = m_chkSeriesData.GetCheck();
	m_chkSeriesData.SetCheck(b);
}

void COBDPNDDlgLog::OnBnClickedCheck2()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL b = m_chkGpsData.GetCheck();
	m_chkGpsData.SetCheck(b);
}
