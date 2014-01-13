/*
����:	Yipb	2011-09-02
˵��:	Ecu Bin�ļ���ȡ������
*/
#pragma once

#include <map>
using namespace std;

#define MAX_RETURN_SIZE 512			//��󷵻��ַ�������

class  CReadBinFile
{
public:
	CReadBinFile(void);
	CReadBinFile(CString strFileName);
	~CReadBinFile(void);
	
	BOOL    m_bFileReady;

private:
	CFile	m_fileHex;	
	CString	m_strBinFileName;		
	
	WORD	m_dwIDCount;	//�ļ���ID����
	BYTE	m_bFileUB;		//�ļ�����	U:UNICODE	B:BINTEXT
	BYTE	m_bFileDE;		//�ļ�����	D:δ����	E:����
	BYTE	m_bFileBL;		//�ļ��ߵ�λB:���ֽ���ǰ	L:���ֽ���ǰ

	map<DWORD,DWORD>	m_mapIDAddr;	//ID��ַ
public:	
	//���ֽ�ת���ֽ�
	int StrC2W(char * cstr, WCHAR * wstr);
	//Unicode�ֽ�תCString
	CString StrC2S(BYTE * cstr,WORD wLen);
	// ����ļ��Ƿ����
	int fIsExistFile(CString strFullPathName);
	// ����ID��ȡID�����ݶ���ʼ��ַ
	DWORD fGetDataStartAddrByID(DWORD dwID);
	// ����ID��ȡ��ID���ݶ�
	WORD fGetDataByID(DWORD dwID, BYTE* pBufData);
	// ����ID��ȡ��ID���ݶ�
	DWORD fGetDataIDCount();
};
