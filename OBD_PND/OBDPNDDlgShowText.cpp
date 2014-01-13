// OBDPNDDlgShowText.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OBD_PND.h"
#include "OBDPNDDlgShowText.h"
#include "OBD_PNDDlg.h"


// COBDPNDDlgShowText �Ի���

IMPLEMENT_DYNAMIC(COBDPNDDlgShowText, CDialog)

COBDPNDDlgShowText::COBDPNDDlgShowText(CWnd* pParent /*=NULL*/)
	: CDialog(COBDPNDDlgShowText::IDD, pParent)
{
	m_iTextIndex = 0;
}

COBDPNDDlgShowText::~COBDPNDDlgShowText()
{
}

void COBDPNDDlgShowText::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(COBDPNDDlgShowText, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// COBDPNDDlgShowText ��Ϣ�������

BOOL COBDPNDDlgShowText::OnInitDialog()
{
	CDialog::OnInitDialog();

	fInitUI();

	return TRUE;
}

void COBDPNDDlgShowText::fInitUI()
{
	this->MoveWindow(theMainDlg->m_rectWin);
	switch (theMainDlg->m_dwScreenResolution)
	{
	case RESOLUTION_480X272:
		{

		}
		break;
	case RESOLUTION_800X480:
		{
			fSetBkImg(this->GetDC(),IDR_PNG_BASE_800X480);

			m_rectText = CRect(50,70,750,400);

			VERIFY(m_fontText.CreateFont(
				32,                        // nHeight
				0,                         // nWidth
				0,                         // nEscapement
				0,                         // nOrientation
				FW_NORMAL, //FW_BOLD,�Ӵ�        FW_NORMAL        // nWeight
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

	m_clrText = RGB(255,255,255);

	m_iTextIndex = 0;
	m_strText = m_vecText[m_iTextIndex];
	if (m_vecText.size()>1)	
		fSetFootText(MAX_FOOT_INDEX-2,theMainDlg->fGetBinText(DS_BTN_NEXT));//��һ��
	else
		fSetFootText(MAX_FOOT_INDEX-2,theMainDlg->fGetBinText(DS_BTN_OK));	//ȷ��
	fSetFootText(MAX_FOOT_INDEX-1,theMainDlg->fGetBinText(DS_BTN_RETURN));	//����

	this->Invalidate();
}

void COBDPNDDlgShowText::fSetTextList(vector<CString> vecList)
{
	m_vecText = vecList;
}

void COBDPNDDlgShowText::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CDialog::OnLButtonDown(nFlags, point);
}

void COBDPNDDlgShowText::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	switch (fGetCurrFootIndex(point))
	{
	case MAX_FOOT_INDEX-2:
		{
			m_iTextIndex++;
			if (m_iTextIndex<m_vecText.size())
			{
				m_strText = m_vecText[m_iTextIndex];
				if (m_vecText.size()>1)	
					fSetFootText(MAX_FOOT_INDEX-2,theMainDlg->fGetBinText(DS_BTN_NEXT));//��һ��
				else
					fSetFootText(MAX_FOOT_INDEX-2,theMainDlg->fGetBinText(DS_BTN_OK));	//ȷ��

				this->Invalidate();
			}
			else
			{
				CDialog::OnOK();
			}
		}
		break;
	case MAX_FOOT_INDEX-1:
		CDialog::OnCancel();
		break;
	}

	CDialog::OnLButtonUp(nFlags, point);
}

BOOL COBDPNDDlgShowText::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	return TRUE;

	return CDialog::OnEraseBkgnd(pDC);
}

void COBDPNDDlgShowText::OnPaint()
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
		memDC.SetTextColor(theMainDlg->m_clrTitle);

		fDrawTitleText(&memDC);

		memDC.SelectObject(&m_fontText); 
		memDC.SetTextColor(m_clrText);   
		
		CRect temp = m_rectText;
		int height = memDC.DrawText(m_strText,temp,DT_LEFT| DT_TOP| DT_WORDBREAK | DT_CALCRECT | DT_EDITCONTROL); // ����ı��߶�
		m_rectText.DeflateRect(0,(m_rectText.Height() - height) / 2); // �ı�rect
		memDC.DrawText(m_strText,m_rectText, DT_LEFT| DT_TOP | DT_EDITCONTROL | DT_WORDBREAK);

		fDrawFootText(&memDC);

		//������λͼ���Ƶ����ڿͻ��� 
		pDC.BitBlt(0,0,rect.Width(),rect.Height(),&memDC,0,0,SRCCOPY); 

		if(pOldMemBmp)   
			memDC.SelectObject(pOldMemBmp); 

		DeleteObject(pOldMemBmp);	
		DeleteDC(memDC);

	}
	CDialog::OnPaint();
}

void COBDPNDDlgShowText::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ	


	CDialog::OnTimer(nIDEvent);
}
