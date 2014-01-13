/*
 * Copyright(C) 2013,V-Checker 保留所有权利。( All rights reserved. )
 * 
 * 文件名称：OBDRDlgBase.h
 * 摘    要：对话框基本类。对话框类继承，可用公用信息。
 * 当前版本：1.0
 * 作    者：Yipb
 * 创建日期：2013年12月5日
 */
#pragma once

//页尾功能按钮个数
#define  MAX_FOOT_INDEX	7


//数据流数据界面信息
struct TagDataStreamInfo 
{
	DWORD dwDsID;

	CRect rectData;
	CRect rectText;
	CRect rectUnit;

	float	fValue;
	int		iDecLen;
	CString strValue;

	CString strText;
	CString strUnit;

	CBitmap cBmpUnit;							
	CDC		memdcUnit;	

};

class COBDPNDDlgBase
{
public:
	COBDPNDDlgBase(void);
	~COBDPNDDlgBase(void);

//-----------------------------------------------------------
//  函数名称：fSetBkImg
//  参    数：设置背景图片
//    - CDC* pDC---------设置背景的CD
//    - DWORD dwResID----背景资源ID
//	  - 
//  返    回：void
//  函数作用：
//  修改日期：2013年12月5日  By YuJian
//-----------------------------------------------------------
	void fSetBkImg(CDC* pDC,DWORD dwResID);

	//获取当前选中的功能位置
	void fSetCurrFootIndex(int iIndex);
	int fGetCurrFootIndex();
	int fGetCurrFootIndex(CPoint point);	
	//设置抬头文本
	void fSetTitleText(CString strTitle);
	//设置功能文本
	void fSetFootText(int iFootIndex,CString strFootText);
	// 画Title文本
	int fDrawTitleText(CDC *memDC);
	// 画页脚文本
	int fDrawFootText(CDC *memDC);

protected:
	CBitmap m_cBitmapBkBmp;						//背景图片
	CDC		m_memdcBkBmp;						//背景图片DC   

private:
	int		m_iCurrFootIndex;
	CString		m_strTitle;						//标题文本
	CString		m_strFoot[MAX_FOOT_INDEX];		//

};
