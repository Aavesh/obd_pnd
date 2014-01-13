#include "StdAfx.h"
#include "OBD_PNDDlg.h"
#include "OBDPNDDlgBase.h"

COBDPNDDlgBase::COBDPNDDlgBase(void)
{
}

COBDPNDDlgBase::~COBDPNDDlgBase(void)
{

}

void COBDPNDDlgBase::fSetBkImg(CDC* pDC,DWORD dwResID)
{
	fCommDrawImage(&m_memdcBkBmp,&m_cBitmapBkBmp,pDC,theMainDlg->m_rectWin,AfxGetResourceHandle(),dwResID);
}

// 画Title文本
int COBDPNDDlgBase::fDrawTitleText(CDC *memDC)
{
	if (memDC == NULL)
		return FALSE;

	//显示Title
	memDC->SetTextColor(theMainDlg->m_clrTitle);
	memDC->SelectObject(theMainDlg->m_fontTitle);
	memDC->DrawText(m_strTitle, -1, &theMainDlg->m_rectTitle, DT_CENTER|DT_VCENTER);

	return TRUE;
}
// 画Foot文本
int COBDPNDDlgBase::fDrawFootText(CDC *memDC)
{
	if (memDC == NULL)
		return FALSE;

	//显示Footer
	memDC->SelectObject(&theMainDlg->m_fontFootNormal); 	
	for (int i=0;i<MAX_FOOT_INDEX;i++)
	{
		if (m_iCurrFootIndex == i)
			memDC->SetTextColor(theMainDlg->m_clrFootFocus);   //设置字体颜色白色  
		else
			memDC->SetTextColor(theMainDlg->m_clrFootNormal);   //设置字体颜色白色  
		memDC->DrawText(m_strFoot[i], -1, &theMainDlg->m_rectFoot[i], DT_CENTER);

	}
	return TRUE;
}

void COBDPNDDlgBase::fSetCurrFootIndex(int iIndex)
{
	m_iCurrFootIndex = iIndex;
}

int	COBDPNDDlgBase::fGetCurrFootIndex(CPoint point)
{
	m_iCurrFootIndex = -1;
	if (point.y > theMainDlg->m_rectFoot[0].top - 10 && point.y < theMainDlg->m_rectFoot[0].bottom  + 10)
	{
		for (int i=0;i<MAX_FOOT_INDEX;i++)
		{
			if (point.x > theMainDlg->m_rectFoot[i].left && point.x < theMainDlg->m_rectFoot[i].right)
			{
				m_iCurrFootIndex = i;
				break;
			}
		}
	}	
	return m_iCurrFootIndex;
}

int	COBDPNDDlgBase::fGetCurrFootIndex()
{
	return m_iCurrFootIndex;
}

void COBDPNDDlgBase::fSetTitleText(CString strTitle)
{
	m_strTitle = strTitle;
}

void COBDPNDDlgBase::fSetFootText(int iFootIndex,CString strFootText)
{
	if(iFootIndex<MAX_FOOT_INDEX)
	{
		m_strFoot[iFootIndex] = strFootText;
	}
}