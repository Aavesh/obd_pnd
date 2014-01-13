#pragma once
#include "OBDPNDDlgBase.h"
#include "cximage_ce/include/ximage.h"
// CSplashWnd
class CSplashWnd : public CWnd,public COBDPNDDlgBase
{
	DECLARE_DYNAMIC(CSplashWnd)
private: 
	CxImage m_ximage;

public: 
	void CreateSplash(); 
	BOOL CheckBMP();


public:
	CSplashWnd();
	virtual ~CSplashWnd();

protected:
	afx_msg void OnPaint(); 
	DECLARE_MESSAGE_MAP()
};
