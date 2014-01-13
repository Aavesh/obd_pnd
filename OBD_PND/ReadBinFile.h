/*
创建:	Yipb	2011-09-02
说明:	Ecu Bin文件读取操作类
*/
#pragma once

#include <map>
using namespace std;

#define MAX_RETURN_SIZE 512			//最大返回字符串长度

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
	
	WORD	m_dwIDCount;	//文件中ID总数
	BYTE	m_bFileUB;		//文件类型	U:UNICODE	B:BINTEXT
	BYTE	m_bFileDE;		//文件加密	D:未加密	E:加密
	BYTE	m_bFileBL;		//文件高地位B:高字节在前	L:低字节在前

	map<DWORD,DWORD>	m_mapIDAddr;	//ID地址
public:	
	//单字节转多字节
	int StrC2W(char * cstr, WCHAR * wstr);
	//Unicode字节转CString
	CString StrC2S(BYTE * cstr,WORD wLen);
	// 检查文件是否存在
	int fIsExistFile(CString strFullPathName);
	// 根据ID获取ID的数据段起始地址
	DWORD fGetDataStartAddrByID(DWORD dwID);
	// 根据ID获取该ID数据段
	WORD fGetDataByID(DWORD dwID, BYTE* pBufData);
	// 根据ID获取该ID数据段
	DWORD fGetDataIDCount();
};
