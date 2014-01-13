#include "StdAfx.h"
#include "Utilities.h"
#include "ReadBinFile.h"

#include "cximage_ce/include/ximage.h"

#include <INITGUID.h> 
#include <imaging.h>

#include "tlhelp32.h"
#pragma comment(lib,"toolhelp.lib")
#include <winioctl.h>
static const DWORD TH32CS_SNAPNOHEAPS = 0x40000000;  


CUtilities::CUtilities(void)
{
}

CUtilities::~CUtilities(void)
{
}


//************************************************************************************
//��    ����fDrawImage
//��    �ܣ���ָ����Դ�ļ�������ԴID����ͼƬ                                 
//��    ����dcMem-----------����ͼƬDC
//			parentDC--------����DC��������������ͼƬDC
//			rect------------ͼƬ��С
//			hInstance-------��Դ���
//			nIDResource-----ͼƬ��ԴID
//			lpType----------ͼƬ����
//��    ��: ��
//************************************************************************************
void fCommDrawImage(CDC * dcMem,CBitmap * cBitmap,CDC * dcParent,const CRect rect,const HINSTANCE hInstance,UINT nIDResource,CString lpType)
{
	HBITMAP hBmpBk = NULL;
	HBITMAP hOldBmp = NULL;	

	if (dcMem->m_hDC == NULL && dcMem != dcParent)
	{	
		if (dcMem->m_hDC == NULL)	
			dcMem->CreateCompatibleDC(dcParent);
		if (cBitmap->m_hObject == NULL)
			cBitmap->CreateCompatibleBitmap(dcParent,rect.Width(),rect.Height());		
	}
	//�ڻ����н���BITMAP 
	hBmpBk = CreateCompatibleBitmap(dcParent->m_hDC, rect.Width(),rect.Height());
	dcMem->SelectObject(hBmpBk); 
	//������ǰ���� 
	dcMem->BitBlt(0, 0, rect.Width(),rect.Height(), dcParent, rect.left, rect.top, SRCCOPY);
	
	HRSRC hRsrc = ::FindResource(hInstance, MAKEINTRESOURCE(nIDResource), lpType); 
	if (!hRsrc)
		return;
	CxImage ximage;
	if(!ximage.LoadResource(hRsrc,CXIMAGE_FORMAT_PNG))
		return;

	ximage.Draw(dcMem->GetSafeHdc(),0,0,rect.Width(),rect.Height());

	////��Դ��С
	//DWORD len = SizeofResource(hInstance, hRsrc);
	////ת��ͼƬ��ԴΪ�ֽ�����
	//BYTE* lpRsrc = (BYTE*)LoadResource(hInstance, hRsrc);
	//if (!lpRsrc)
	//	return;

	//IImagingFactory *pImgFactory = NULL;
	//IImage *pImage = NULL;

	//CoInitializeEx(NULL, COINIT_MULTITHREADED);

	//if (SUCCEEDED(CoCreateInstance (CLSID_ImagingFactory,
	//	NULL,
	//	CLSCTX_INPROC_SERVER,
	//	IID_IImagingFactory,
	//	(void **)&pImgFactory)))
	//{
	//	ImageInfo imageInfo;
	//	if (SUCCEEDED(pImgFactory->CreateImageFromBuffer(lpRsrc,len,BufferDisposalFlagNone,&pImage)) && 
	//		SUCCEEDED(pImage->GetImageInfo(&imageInfo)))
	//	{
	//		CRect rectTemp = CRect(0,0,rect.Width(),rect.Height());
	//		pImage->Draw(dcMem->GetSafeHdc(), &rectTemp, NULL);				
	//		pImage->Release();
	//	}
	//	pImgFactory->Release();
	//}
	//CoUninitialize();

	//�ͷ���Դ
	FreeResource(hRsrc);


	DeleteObject(hOldBmp);
	DeleteObject(hBmpBk);
}

//************************************************************************************
//��    ����fGetAppPath()      
//��    �ܣ���ǰӦ������·��
//��    ����
//��    ��: ��ǰӦ������·��
//************************************************************************************
CString fCommGetAppPath()
{
	CString strPath;
	GetModuleFileName(NULL,strPath.GetBufferSetLength(MAX_PATH+1),MAX_PATH);
	strPath.ReleaseBuffer();
	int pos = strPath.ReverseFind('\\');
	strPath = strPath.Left(pos);
	return strPath;
}


//************************************************************************************
//��    ����fW2C
//��    �ܣ����ֽ�ת���ֽ�                      
//��    ����pwstr---------Դ���ֽ�����
//			pcstr---------ת����ĵ��ֽ�����
//			len-----------���ݳ���
//��    ��: ��ȡ���г���·��
//************************************************************************************
void fCommW2C(const wchar_t *pwstr,char *pcstr,size_t len)
{
	int nlength=wcslen(pwstr);
	//��ȡת����ĳ���
	int nbytes = WideCharToMultiByte( 0, // specify the code page used to perform the conversion
		0,         // no special flags to handle unmapped characters
		pwstr,     // wide character string to convert
		nlength,   // the number of wide characters in that string
		NULL,      // no output buffer given, we just want to know how long it needs to be
		0,
		NULL,      // no replacement character given
		NULL );    // we don't want to know if a character didn't make it through the translation
	// make sure the buffer is big enough for this, making it larger if necessary

	if(nbytes>len)   nbytes=len;
	// ͨ�����ϵõ��Ľ����ת��unicode �ַ�Ϊascii �ַ�
	WideCharToMultiByte( 0, // specify the code page used to perform the conversion
		0,         // no special flags to handle unmapped characters
		pwstr,   // wide character string to convert
		nlength,   // the number of wide characters in that string
		pcstr, // put the output ascii characters at the end of the buffer
		nbytes,                           // there is at least this much space there
		NULL,      // no replacement character given
		NULL );
}
//************************************************************************************
//��    ����fCommStr2Hex
//��    �ܣ���һ��ʮ�����ƴ��ַ���ת����ֵ                  
//��    ����pwstr---------Դ���ֽ�����
//			pcstr---------ת����Ŀ��ֽ�����
//			len-----------���ݳ���
//��    ��: ��ȡ���г���·��
//************************************************************************************
DWORD fCommStr2Hex(CString strHex)
{	
	if (strHex == _T(""))
		return 0;

	DWORD num[16]={0};   
	DWORD count=1;   
	DWORD result=0;   
	DWORD length = strHex.GetLength();

	for (int i=length-1;i>=0;i--)   
	{   
		if ((strHex[i]>='0') && (strHex[i]<='9'))   
			num[i]=strHex[i]-48;//�ַ�0��ASCIIֵΪ48
		else if ((strHex[i]>='a') && (strHex[i]<='f'))   
			num[i]=strHex[i]-'a'+10;   
		else if ((strHex[i]>='A') && (strHex[i]<='F'))   
			num[i]=strHex[i]-'A'+10;   
		else   
			num[i]=0; 
		result=result+num[i]*count;   
		count=count*16;//ʮ������(����ǰ˽��ƾ����������8)    
	}   
	return result;   
}

CString  fCommGetBinFileText(DWORD dwID,CString strBinFileName)
{
	CString strReturnText = _T("");
	CReadBinFile cReadBinFile(strBinFileName);

	BYTE * pBufData = new BYTE[512];
	ZeroMemory(pBufData,512);
	BYTE byDataLen = cReadBinFile.fGetDataByID(dwID,pBufData);

	if (byDataLen>0)
	{
		strReturnText = CString((WCHAR*)pBufData);
	}

	delete pBufData;
	pBufData = NULL;
	return strReturnText;
}

float fIEEE745ToFloat(DWORD dwValue)
{
	union uu
	{
		char aa[4];
		float bb;
	} t;

	t.aa[3] = dwValue/0x1000000;
	t.aa[2] = (dwValue%0x1000000)/0x10000;
	t.aa[1] =(dwValue%0x10000)/0x100;
	t.aa[0] = dwValue%0x100;

	return t.bb;
}

// ��һ���ַ�����Ϊvector����bSplitChar�ָ���
vector<CString> fCommStrSplit(CString str, BYTE bSplitChar)
{
	str.Replace(_T(" "),_T(""));

	vector<CString> vecList;

	CString strTemp = str; //�˸�ֵ������
	CString strText = _T("");
	int nIndex = 0; //  
	while( 1 )  
	{  
		nIndex = strTemp.Find( bSplitChar );  
		if( nIndex >= 0 )  
		{  
			strText = strTemp.Left( nIndex );
			vecList.push_back(strText);
			strTemp = strTemp.Right( strTemp.GetLength() - nIndex - 1 );  		
		}  
		else 
		{
			strText = strTemp;
			vecList.push_back(strText);
			break;  
		}
	}  

	return vecList ;
}

BOOL fCommFindProcess(CString strProcessName)
{
	if (strProcessName == _T(""))
		return FALSE;

	BOOL bFind = FALSE;

	int i=0;

	HANDLE hSnap;
	CString strPath;
	LPTSTR path = new TCHAR[MAX_PATH];

	hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS| TH32CS_SNAPNOHEAPS,0);
	PROCESSENTRY32 process;
	process.dwSize=sizeof(PROCESSENTRY32);
	BOOL bResult = Process32First(hSnap,&process);

	while (bResult)
	{
		ZeroMemory(path,MAX_PATH);
		for (int i=0;i<MAX_PATH;i++)
			path[i] = process.szExeFile[i];

		strPath = path;
		strPath.MakeUpper();
		strProcessName.MakeUpper();
		if (strProcessName == strPath)
		{
			bFind = TRUE;
			break;
		}

		bResult = Process32Next(hSnap,&process);
	}

	delete path;
	path = NULL;

	CloseToolhelp32Snapshot(hSnap);

	return bFind;

}

void fCommKillProcess(CString strProcessName)
{
	if (strProcessName == _T(""))
		return;

	int i=0;

	HANDLE hSnap;
	CString strPath;
	LPTSTR path = new TCHAR[MAX_PATH];

	hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS| TH32CS_SNAPNOHEAPS,0);
	PROCESSENTRY32 process;
	process.dwSize=sizeof(PROCESSENTRY32);
	BOOL bResult = Process32First(hSnap,&process);

	while (bResult)
	{
		ZeroMemory(path,MAX_PATH);
		for (int i=0;i<MAX_PATH;i++)
			path[i] = process.szExeFile[i];

		strPath = path;
		strPath.MakeUpper();
		strProcessName.MakeUpper();
		if (strProcessName == strPath)
		{
			HANDLE killProcess = OpenProcess(0,FALSE,process.th32ProcessID);
			if(killProcess)
			{
				TCHAR szKillPath[MAX_PATH]={0};
				GetModuleFileName((HMODULE)killProcess,szKillPath,MAX_PATH);	
				TerminateProcess(killProcess,NULL);
			}			
			CloseHandle(killProcess);
			break;
		}

		bResult = Process32Next(hSnap,&process);
	}

	delete path;
	path = NULL;

	CloseToolhelp32Snapshot(hSnap);
}


BOOL fCommRunProcess(CString strFullPathName)
{
	SHELLEXECUTEINFO ShellInfo; 
	memset(&ShellInfo, 0, sizeof(ShellInfo)); 
	ShellInfo.cbSize = sizeof(ShellInfo);
	ShellInfo.hwnd = NULL; 
	ShellInfo.lpVerb = _T("open"); 
	ShellInfo.lpFile = strFullPathName; 
	ShellInfo.nShow = SW_SHOWNORMAL; 
	ShellInfo.fMask = SEE_MASK_NOCLOSEPROCESS; 
	ShellInfo.lpParameters = _T("");	
	BOOL bResult = ShellExecuteEx(&ShellInfo);

	return bResult;

}
// ����ļ��Ƿ����
BOOL fCommIsExistFile(CString strFullPathName)
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
void fCommPlaySound(CString strPathFile)
{
	PlaySound(strPathFile, NULL, SND_FILENAME | SND_ASYNC);
}