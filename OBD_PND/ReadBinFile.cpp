/*
创建:	Yipb	2011-11-10
说明:	Ecu Bin文件读取操作类
*/

#include "StdAfx.h"
#include "ReadBinFile.h"
#include "Decrypt.h"


CReadBinFile::CReadBinFile(void)
{
	m_strBinFileName	= _T("");	

	m_dwIDCount		= 0;	
	m_bFileUB		= 0;		
	m_bFileDE		= 0;		
	m_bFileBL		= 0;		
	m_bFileReady	= FALSE;
}

CReadBinFile::~CReadBinFile(void)
{
	m_fileHex.Close();
}

CReadBinFile::CReadBinFile(CString strFileName)
{
	m_strBinFileName = strFileName;

	//将 Stream.Bin 文件读取到内存
	m_fileHex.Open(m_strBinFileName,CFile::modeRead|CFile::typeBinary);
	if (!m_fileHex)
	{
		AfxMessageBox(_T("Bin file open error!"));
		return ;
	}	

	//读文件头
	m_fileHex.Seek(1,CFile::begin);
	m_fileHex.Read(&m_dwIDCount,2);	

	m_fileHex.Seek(9,CFile::begin);
	m_fileHex.Read(&m_bFileDE,1);	

	m_fileHex.Seek(17,CFile::begin);
	m_fileHex.Read(&m_bFileBL,1);

	m_fileHex.Seek(26,CFile::begin);
	m_fileHex.Read(&m_bFileUB,1);


	CString strText;
	strText.Format(_T("%c%c%c"),m_bFileDE,m_bFileBL,m_bFileUB);
	//保存所有ID以及ID对应的数据段起始地址
	if (!(m_bFileUB == 'U' && m_bFileDE == 'E' && m_bFileBL == 'L'))
	{
		AfxMessageBox(_T("Bin file type error!")+strText);
		return ;
	}

	m_bFileReady = TRUE;
}

// 根据ID获取该ID数据段
DWORD CReadBinFile::fGetDataIDCount()
{
	return m_dwIDCount;
}

// 根据ID获取该ID数据段
WORD CReadBinFile::fGetDataByID(DWORD dwID, BYTE* pBufData)
{
	if (!m_fileHex && m_bFileReady)
	{
		AfxMessageBox(_T("Bin file read error!"));
		return 0;
	}	

	DWORD dwIDDataAddr = fGetDataStartAddrByID(dwID);

	if (dwIDDataAddr == 0)
	{
		//AfxMessageBox(_T("Bin file read id data error!"));
		return 0;
	}	
	
	DWORD dwDataLen = 0;
	m_fileHex.Seek(dwIDDataAddr,CFile::begin);
	m_fileHex.Read(&dwDataLen,2);
	
	m_fileHex.Seek(dwIDDataAddr+2,CFile::begin);
	m_fileHex.Read(pBufData,dwDataLen);

	CDecrypt cDecrypt;

	//CString strText;
	//CString strTemp;
	//strText.Format(_T("dwID=%08X	addr=%08X	dwDataLen=%d\r\n"),dwID,dwIDDataAddr - (32+m_dwIDCount*8),dwDataLen);
	//for ( int i=0;i<dwDataLen;i++)
	//{
	//	strTemp.Format(_T("%02X "),pBufData[i]);
	//	strText += strTemp;			 
	//}
	//strText += _T("\r\n");		

	cDecrypt.EncodeStr(pBufData,dwID,dwIDDataAddr - (32+m_dwIDCount*8),dwDataLen);
//	EncodeStr(pBufData,dwID,dwIDDataAddr - (32+m_dwIDCount*8),dwDataLen);

//	for ( int i=0;i<dwDataLen;i++)
//	{
//		strTemp.Format(_T("%02X "),pBufData[i]);
//		strText += strTemp;			 
//	}
//AfxMessageBox(strText);

	return dwDataLen;
}

// 根据ID获取ID的数据段起始地址
DWORD CReadBinFile::fGetDataStartAddrByID(DWORD dwID)
{
	map<DWORD, DWORD>::iterator iter;
	iter = m_mapIDAddr.find(dwID);
	
	if(iter != m_mapIDAddr.end())
	{				
		return iter->second;
	}

	if (!m_fileHex)
	{
		AfxMessageBox(_T("Bin file read error!"));
		return 0;
	}	

	DWORD dwIDFind = 0;
	DWORD dwIDAddr = 0;

	DWORD dwIndexBegin = 0;
	DWORD dwIndexEnd = m_dwIDCount;
	DWORD dwIndex = m_dwIDCount/2;

	DWORD dwIndexCount = 0;
	DWORD dwIndexOld = 0;
	
	while(dwIndex>=0)
	{
		m_fileHex.Seek(32+dwIndex*8,CFile::begin);		
		m_fileHex.Read(&dwIDFind,4);

		if (dwIDFind < dwID)
		{			
			dwIndexBegin = dwIndex;
			dwIndex = dwIndexBegin + (dwIndexEnd - dwIndexBegin)/2;			
		}
		else if (dwIDFind > dwID)
		{
			dwIndexEnd = dwIndex;
			dwIndex = dwIndex/2;					
		}
		else if (dwIDFind == dwID)
		{
			m_fileHex.Seek(32+dwIndex*8+4,CFile::begin);		
			m_fileHex.Read(&dwIDAddr,4);
			return dwIDAddr;
		}

		if (dwIndexOld == dwIndex)
		{
			dwIndexCount ++;
		}
		if (dwIndexCount>2)
		{
			return 0;
		}

		dwIndexOld = dwIndex;
		
	}
	return 0;
}


int CReadBinFile::StrC2W(char * cstr, WCHAR * wstr)
{
	int i = MultiByteToWideChar(CP_ACP,MB_COMPOSITE,(char*)cstr,-1,0,0);
	MultiByteToWideChar(CP_ACP,MB_COMPOSITE,(char*)cstr,-1,wstr,i);
	return 0;
}

CString CReadBinFile::StrC2S(BYTE * cstr,WORD wLen)
{	
	wchar_t* buf = new wchar_t[wLen/2 + 1];
	memset(buf, 0, sizeof(wchar_t) * (wLen/2 + 1));

	for(int i=0, j=0; i<wLen; i+=2)
	{
		BYTE high = cstr[i];
		BYTE low = cstr[i+1];
		buf[j++] = MAKEWORD(low, high);
	}
	CString str(buf);

	delete buf;
	buf = NULL;
	return str;
}


// 检查文件是否存在
int CReadBinFile::fIsExistFile(CString strFullPathName)
{
	if(::GetFileAttributes(strFullPathName) == 0xFFFFFFFF)
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	} 
}


