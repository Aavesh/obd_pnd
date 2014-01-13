// OBDPNDDlgTipInfo.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OBD_PND.h"
#include "OBDPNDDlgTipInfo.h"
#include "OBD_PNDDlg.h"


// COBDPNDDlgTipInfo �Ի���

IMPLEMENT_DYNAMIC(COBDPNDDlgTipInfo, CDialog)

COBDPNDDlgTipInfo::COBDPNDDlgTipInfo(CWnd* pParent /*=NULL*/)
	: CDialog(COBDPNDDlgTipInfo::IDD, pParent)
{
	m_strSoundFile = _T("");
}

COBDPNDDlgTipInfo::~COBDPNDDlgTipInfo()
{
}

void COBDPNDDlgTipInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(COBDPNDDlgTipInfo, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// COBDPNDDlgTipInfo ��Ϣ�������


BOOL COBDPNDDlgTipInfo::OnInitDialog()
{
	CDialog::OnInitDialog();

	fInitUI();

	return TRUE;
}

void COBDPNDDlgTipInfo::fInitUI()
{
	switch (theMainDlg->m_dwScreenResolution)
	{
	case RESOLUTION_480X272:
		{

		}
		break;
	case RESOLUTION_800X480:
		{

			m_rect = CRect(0,0,469,125);
			m_rectTipText =  CRect(100,0,469,125);
			fCommDrawImage(&m_memdcBkBmp,&m_cBitmapBkBmp,this->GetDC(),m_rect,AfxGetResourceHandle(),IDR_PNG_TIP_INFO_800X480);

			VERIFY(m_fontText.CreateFont(
				50,                        // nHeight
				0,                         // nWidth
				0,                         // nEscapement
				0,                         // nOrientation
				FW_BOLD, //FW_BOLD,�Ӵ�        FW_NORMAL        // nWeight
				FALSE,                     // bItalic
				FALSE,                     // bUnderline
				0,                         // cStrikeOut
				ANSI_CHARSET,              // nCharSet
				OUT_DEFAULT_PRECIS,        // nOutPrecision
				CLIP_DEFAULT_PRECIS,       // nClipPrecision
				DEFAULT_QUALITY,           // nQuality
				DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
				_T("Arial Rounded MT")));              // lpszFacename
			
		}

		break;
	}
	this->MoveWindow(m_rect);	
	this->Invalidate();
	this->CenterWindow();

	m_clrTextNormal = RGB(244,78,6);
	m_clrTextWarn   = RGB(255,255,0);
	m_clrTextError  = RGB(255,0,0);
	this->Invalidate();
}

void COBDPNDDlgTipInfo::fSetTipText(CString strText,WORD wType)
{
	m_strTipText = strText;
	m_dwTipType = wType;

	this->Invalidate();
}

void COBDPNDDlgTipInfo::fShowTipText(CString strText,WORD wType)
{
	if (strText != _T(""))
		fSetTipText(strText,wType);
	if (m_strTipText !=_T(""))
	{
		this->BringWindowToTop();
		::SetWindowPos(GetSafeHwnd(),HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
		this->ShowWindow(SW_SHOW);
	}
	if (m_strSoundFile != _T(""))
	{
		fCommPlaySound(m_strSoundFile);
	}
}

void COBDPNDDlgTipInfo::fSetSoundFile(CString strSoundFile)
{
	m_strSoundFile = strSoundFile;
}

void COBDPNDDlgTipInfo::fHideTipText()
{
	this->ShowWindow(SW_HIDE);
}

void COBDPNDDlgTipInfo::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//static int dwOldTime;  
	//int dwTimeElapsed;  
	//if(m_wCount == 0)  
	//{  
	//	m_wCount++;  
	//	dwOldTime = GetTickCount();  
	//	SetTimer(1,1000,NULL);  
	//}  
	//else  
	//{  
	//	dwTimeElapsed = GetTickCount() - dwOldTime;  
	//	if(dwTimeElapsed <1000  &&dwTimeElapsed > 100 && m_wCount == 1)  
	//	{  
	//		m_wCount++;  
	//	}  
	//	else  
	//		m_wCount = 0;  
	//}  

	//if(m_wCount == 2) //˫���ж�  
	//{  
	//	KillTimer(1);

	//	theMainDlg->OnMiniHide(0,0);

	//}  
	CDialog::OnLButtonDown(nFlags, point);
}

void COBDPNDDlgTipInfo::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	theMainDlg->OnMiniHide(0,0);

	CDialog::OnLButtonUp(nFlags, point);
}

BOOL COBDPNDDlgTipInfo::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	return TRUE;

	return CDialog::OnEraseBkgnd(pDC);
}

void COBDPNDDlgTipInfo::OnPaint()
{
	if (m_memdcBkBmp != NULL)
	{
		CClientDC pDC(this);

		CRect rect; 
		CDC   memDC; 

		GetWindowRect(&rect); 	
		memDC.CreateCompatibleDC(&pDC); 	

		CBitmap* pOldMemBmp = NULL; 
		pOldMemBmp = memDC.SelectObject(&m_cBitmapBkBmp);
		memDC.BitBlt(0,0,rect.Width(),rect.Height(),&m_memdcBkBmp,0,0,SRCCOPY);

		memDC.SetBkMode(TRANSPARENT);	

	
		memDC.SelectObject(&m_fontText);
		if (m_dwTipType == TIP_INFO_ERROR)
			memDC.SetTextColor(theMainDlg->m_clrTitle);   //����������ɫ��ɫ  
		else
			memDC.SetTextColor(theMainDlg->m_clrTitle);   //����������ɫ��ɫ  

		CRect boundary ;//�����ı�Ҫ��ʾ��������
		GetClientRect(boundary); // ��ÿͻ�����Χ
		CRect rectTemp(0,0,boundary.Width() ,boundary.Height());//����������ʾ��ʱ����
		//ʹ��DT_CALCRECT��DT_CENTER��DT_EDITCONTROL��DT_WORDBREAK����������Ҫ���ı��߶� 
		int height = memDC.DrawText(m_strTipText, rectTemp, DT_CALCRECT|DT_CENTER|DT_EDITCONTROL|DT_WORDBREAK);
		rectTemp = boundary;//������ɺ�ԭ�������򸳻�rect
		if(boundary.Height() > height)//�жϸ߶��Ƿ񳬳���Χ��������ָ������
			rectTemp.top += (boundary.Height() - height)/2;//����հ׸߶ȵ�һ�룬����Ҫ��+=
		memDC.DrawText(m_strTipText, rectTemp,DT_CENTER|DT_EDITCONTROL|DT_WORDBREAK); //�����ֱ����+���Ҿ���+�Զ����е��ı����


		//������λͼ���Ƶ����ڿͻ��� 
		pDC.BitBlt(0,0,rect.Width(),rect.Height(),&memDC,0,0,SRCCOPY); 

		if(pOldMemBmp)   
			memDC.SelectObject(pOldMemBmp); 

		DeleteObject(pOldMemBmp);	
		DeleteDC(memDC);

	}
	CDialog::OnPaint();
}

void COBDPNDDlgTipInfo::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ	
	switch (nIDEvent)
	{
	case 1:
		{
			m_wCount = 0;  
			KillTimer(1);  
		}
		break;
	}
	CDialog::OnTimer(nIDEvent);
}
