#include "stdafx.h"
#include "resource.h"
#include "SplashWnd.h"
#include "OBD_PNDDlg.h"


// CSplashWnd

IMPLEMENT_DYNAMIC(CSplashWnd, CWnd)

CSplashWnd::CSplashWnd()
{

	HRSRC hRsrc = ::FindResource(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_PNG_LOGO_800X480), _T("PNG")); 
	if (!hRsrc)
		return;
	if(!m_ximage.LoadResource(hRsrc,CXIMAGE_FORMAT_PNG))
		return;

}

CSplashWnd::~CSplashWnd()
{
}


BEGIN_MESSAGE_MAP(CSplashWnd, CWnd)
	ON_WM_PAINT() 
END_MESSAGE_MAP()


BOOL CSplashWnd::CheckBMP()
{
	COLORREF clr;
	CString str;
	
	COLORREF * clrList = new COLORREF[100];
	for (int i=0;i<40;i++) 
	{
		if (i == 12)
			clrList[12] = RGB(255,0,0);
		else if (i == 13)
			clrList[13] = RGB(255,255,255);
		else if (i == 17)
			clrList[17] = RGB(0,4,0);
		else if(i == 21)
			clrList[21] = RGB(8,12,8);
		else if(i == 22)
			clrList[22] = RGB(255,211,0);
		else if(i == 23)
			clrList[23] = RGB(0,4,0);
		else if(i == 33)
			clrList[33] = RGB(239,239,247);
		else
			clrList[i] = RGB(0,0,0);
	}

	int iCount = 0;
	CDC *dc = this->GetDC();
	for (int i=0;i<800;i+=100)
	{
		for (int j=0;j<480;j+=100)
		{
			clr = dc->GetPixel(i,j);

			if (GetRValue(clr) != GetRValue(clrList[iCount]) ||
				GetGValue(clr) != GetGValue(clrList[iCount]) ||
				GetBValue(clr) != GetBValue(clrList[iCount]))
			{			
				//str.Format(_T(" %d = %d,%d \r\n %d,%d,%d \r\n %d,%d,%d"),iCount,i,j,
				//	GetRValue(clr),GetGValue(clr),GetBValue(clr),
				//	GetRValue(clrList[iCount]),GetGValue(clrList[iCount]),GetBValue(clrList[iCount]));
				//AfxMessageBox(str);

				return FALSE;
			}
			iCount++;
		}
	}

	return TRUE;
}


void CSplashWnd::CreateSplash() 
{ 
	//Create Splash Window 
	CWnd::CreateEx(0, 
		AfxRegisterWndClass( 
		0, 
		AfxGetApp()->LoadStandardCursor(IDC_UPARROW)), 
		NULL, 
		//            "SplashWindow Sample",  
		WS_POPUP, 
		0, 
		0, 
		800,  //Bitmap Width = Splash Window Width 
		480, //Bitmap Height = Splash Window Height 
		NULL, 
		NULL, 
		NULL); 
} 
void CSplashWnd::OnPaint() 
{ 
	CPaintDC dc(this); 

	m_ximage.Draw(dc.GetSafeHdc(),0,0,800,480);
} 


//
////IDB_BITMAP1是添加的位图资源。
//
//
//然后在MainFrm.cpp中OnCreate函数开始处
//m_pSplashWindow = new CSplashWnd();
//// CRect rect(10,10,500,500);
//// m_pSplashWindow->Create(NULL,NULL,0,rect,NULL,0);
//m_pSplashWindow->CreateSplash(); 
//m_pSplashWindow->CenterWindow(); 
//m_pSplashWindow->ShowWindow(SW_SHOW);
//m_pSplashWindow->UpdateWindow(); 
//// Sleep(2000);
//m_pSplashWindow->DestroyWindow();
