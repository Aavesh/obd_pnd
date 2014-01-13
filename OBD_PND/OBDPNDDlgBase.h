/*
 * Copyright(C) 2013,V-Checker ��������Ȩ����( All rights reserved. )
 * 
 * �ļ����ƣ�OBDRDlgBase.h
 * ժ    Ҫ���Ի�������ࡣ�Ի�����̳У����ù�����Ϣ��
 * ��ǰ�汾��1.0
 * ��    �ߣ�Yipb
 * �������ڣ�2013��12��5��
 */
#pragma once

//ҳβ���ܰ�ť����
#define  MAX_FOOT_INDEX	7


//���������ݽ�����Ϣ
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
//  �������ƣ�fSetBkImg
//  ��    �������ñ���ͼƬ
//    - CDC* pDC---------���ñ�����CD
//    - DWORD dwResID----������ԴID
//	  - 
//  ��    �أ�void
//  �������ã�
//  �޸����ڣ�2013��12��5��  By YuJian
//-----------------------------------------------------------
	void fSetBkImg(CDC* pDC,DWORD dwResID);

	//��ȡ��ǰѡ�еĹ���λ��
	void fSetCurrFootIndex(int iIndex);
	int fGetCurrFootIndex();
	int fGetCurrFootIndex(CPoint point);	
	//����̧ͷ�ı�
	void fSetTitleText(CString strTitle);
	//���ù����ı�
	void fSetFootText(int iFootIndex,CString strFootText);
	// ��Title�ı�
	int fDrawTitleText(CDC *memDC);
	// ��ҳ���ı�
	int fDrawFootText(CDC *memDC);

protected:
	CBitmap m_cBitmapBkBmp;						//����ͼƬ
	CDC		m_memdcBkBmp;						//����ͼƬDC   

private:
	int		m_iCurrFootIndex;
	CString		m_strTitle;						//�����ı�
	CString		m_strFoot[MAX_FOOT_INDEX];		//

};
