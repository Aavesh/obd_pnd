#include "StdAfx.h"
#include "Series.h"
#include "Decrypt.h"


CSeries::CSeries(void)
{
	m_hComm = INVALID_HANDLE_VALUE;
	m_hOwner = NULL;
}

CSeries::~CSeries(void)
{
}

//串口读线程函数
DWORD CSeries::ReadThreadFunc(LPVOID lparam)
{
	CSeries *ceSeries = (CSeries*)lparam;

	BYTE * pBufReadData = new BYTE[1];
	ZeroMemory(pBufReadData,1);
	BYTE * pBufHeadData = new BYTE[4];
	ZeroMemory(pBufHeadData,4);

	BYTE * pBufRecv = new BYTE[MAX_CMD_SIZE];
	ZeroMemory(pBufRecv,MAX_CMD_SIZE);

	DWORD dwActualReadLen = 0;	//每次读取到的实际长度
	DWORD dwRecvLen = 0;		//读取到的数据总长度
	DWORD dwDataLen = 0;		//数据帧长度

	DWORD dwBeginTime = GetTickCount();		//读取开始时间
	BOOL bReadOverTime = FALSE;

	while (TRUE)
	{
		ZeroMemory(pBufReadData,1);
		ZeroMemory(pBufHeadData,4);
		ZeroMemory(pBufRecv,MAX_CMD_SIZE);

		dwActualReadLen = 0;	//每次读取到的实际长度
		dwRecvLen = 0;		//读取到的数据总长度
		dwDataLen = 0;		//数据帧长度

		dwBeginTime = GetTickCount();		//读取开始时间
		bReadOverTime = FALSE;
		//
		while(TRUE)
		{
			////超时退出
			//if((GetTickCount() - dwBeginTime) > DEFAULT_OVERTIME)
			//{
			//	dwRecvLen = 0;
			//	bReadOverTime = TRUE;
			//	break;
			//}

			ReadFile(ceSeries->m_hComm, pBufReadData, 1, &dwActualReadLen,0);

			//ceSeries->m_OnSeriesRead(pBufHeadData,4);
			if (dwActualReadLen != 1)
				continue;

			pBufHeadData[0] = pBufHeadData[1];
			pBufHeadData[1] = pBufHeadData[2];
			pBufHeadData[2] = pBufHeadData[3]; 
			pBufHeadData[3] = pBufReadData[0];

			if (pBufHeadData[0] == 0x55 && pBufHeadData[1] == 0xAA &&
				pBufHeadData[2] == 0x81 && pBufHeadData[3] == 0x01)
			{
				memcpy(pBufRecv,pBufHeadData,4);
				dwRecvLen = 4;
				break;
			}						
		}

		//读取头完成，读取数据段
		if (!bReadOverTime)
		{
			//如果读数据帧失败，直接返回超时
			while(TRUE)
			{	
				////超时退出
				//if((GetTickCount() - dwBeginTime) > DEFAULT_OVERTIME)
				//{
				//	dwRecvLen = 0;
				//	bReadOverTime = TRUE;
				//	break;
				//}

				ReadFile(ceSeries->m_hComm, pBufReadData, 1, &dwActualReadLen,0);
				if (dwActualReadLen != 1)
					continue;

				//每次读取一个字节，并保存
				memcpy(pBufRecv + dwRecvLen,pBufReadData,1);
				dwRecvLen++;

				//ceSeries->m_OnSeriesRead(pBufRecv,dwRecvLen);

				//计算数据帧长度
				if(dwRecvLen == 7)
				{
					dwDataLen = pBufRecv[5]*0100 + pBufRecv[6];

					if (dwDataLen == 0)
					{
						dwRecvLen = 0;
						bReadOverTime = TRUE;
						break;
					}
				}

				//读取完成退出循环
				if (dwRecvLen == 7+dwDataLen)
					break;		
			}

		}

		if (!bReadOverTime)
		{
			if (dwRecvLen > 0)
			{
				//对接收的数据进行效验位判断
				CDecrypt cDecrypt;
				DWORD dwCmdID = pBufRecv[7]*0x100 + pBufRecv[8];	//命令ID
				WORD  wLen =  pBufRecv[5]*0x100 + pBufRecv[6];		//数据帧+命令ID的长度
				WORD wCheckBit = cDecrypt.fCalcCheckBit(dwCmdID,pBufRecv[6],pBufRecv+9,wLen-2);	
				if (wCheckBit == pBufRecv[4])
				{
					ceSeries->m_OnSeriesRead(pBufRecv,dwRecvLen);
					
				}
			}
		}
		else
		{
			::SendMessage(ceSeries->m_hOwner,WM_SERIES_RESTART,0,0);
		}

	}

	delete pBufRecv;
	pBufRecv = NULL;

	delete pBufHeadData;
	pBufHeadData = NULL;

	delete pBufReadData;
	pBufReadData = NULL;

	return 0;
}

//关打开读线程
void CSeries::OpenReadThread()
{
	CloseReadThread();
	//创建串口读数据监听线程
	m_hReadThread = CreateThread(NULL,0,ReadThreadFunc,this,0,&m_dwReadThreadID);
}

//关闭读线程
void CSeries::CloseReadThread()
{
	SetEvent(m_hReadCloseEvent);
	//设置所有事件无效无效
	SetCommMask(m_hComm, 0);
	//清空所有将要读的数据
	PurgeComm( m_hComm,  PURGE_RXCLEAR );
	//等待4秒，如果读线程没有退出，则强制退出
	if (WaitForSingleObject(m_hReadThread,4000) == WAIT_TIMEOUT)
	{
		TerminateThread(m_hReadThread,0);
	}
	m_hReadThread = NULL;
}

//************************************************************************************
//函    数：fSendCmd(BYTE * bufData)      
//功    能：打开串口                                                  
//参    数：void* pOwner----------指向父指针
//			UINT portNo-----------串口号		= 1,		
//			UINT baud-------------波特率		= 115200,
//			UINT parity-----------奇偶校验		= NOPARITY,
//			UINT databits---------数据位		= 8,
//			UINT stopbits---------停止位		= 1
//返    回: 打开串口是否成功
//************************************************************************************
BOOL CSeries::OpenPort(HWND hOwner,UINT portNo,UINT baud,UINT parity,UINT databits,UINT stopbits)
{
	DCB commParam;
	TCHAR szPort[15];	

	//ASSERT(pOwner!=NULL);
	m_hOwner = hOwner;

	// 已经打开的话，直接关闭
	if (m_hComm != INVALID_HANDLE_VALUE)
	{
		//关闭串口
		CloseHandle (m_hComm);

		m_hComm = INVALID_HANDLE_VALUE;
		m_bOpened = FALSE;
	}

	//设置串口名
	wsprintf(szPort, L"COM%d:", portNo);
	//打开串口
	m_hComm = CreateFile(
		szPort,
		GENERIC_READ | GENERIC_WRITE,	//允许读和写
		0,								//独占方式（共享模式）
		NULL,
		OPEN_EXISTING,					//打开而不是创建（创建方式）
		FILE_FLAG_OVERLAPPED,
		NULL 
		);

	if (m_hComm == INVALID_HANDLE_VALUE)
	{
		// 无效句柄,返回。		
		//theMainDlg->fAddLog(_T("CreateFile 返回无效句柄\n"));
		return FALSE;
	}

	// 得到打开串口的当前属性参数，修改后再重新设置串口。
	if (!GetCommState(m_hComm,&commParam))
	{		
		//关闭串口
		CloseHandle (m_hComm);
		m_hComm = INVALID_HANDLE_VALUE;
		return FALSE;
	}

	//设置串口参数
	commParam.BaudRate = baud;					// 设置波特率 
	commParam.fBinary = TRUE;					// 设置二进制模式，此处必须设置TRUE
	commParam.fParity = TRUE;					// 支持奇偶校验 
	commParam.ByteSize = databits;				// 数据位,范围:4-8 
	commParam.Parity = parity;				    // 校验模式
	commParam.StopBits = stopbits;				// 停止位 

	commParam.fOutxCtsFlow = FALSE;				// No CTS output flow control 
	commParam.fOutxDsrFlow = FALSE;				// No DSR output flow control 
	commParam.fDtrControl = DTR_CONTROL_ENABLE; 
	// DTR flow control type 
	commParam.fDsrSensitivity = FALSE;			// DSR sensitivity 
	commParam.fTXContinueOnXoff = TRUE;			// XOFF continues Tx 
	commParam.fOutX = FALSE;					// No XON/XOFF out flow control 
	commParam.fInX = FALSE;						// No XON/XOFF in flow control 
	commParam.fErrorChar = FALSE;				// Disable error replacement 
	commParam.fNull = FALSE;					// Disable null stripping 
	commParam.fRtsControl = RTS_CONTROL_ENABLE; 
	// RTS flow control 
	commParam.fAbortOnError = FALSE;			// 当串口发生错误，并不终止串口读写

	//设置串口参数
	if (!SetCommState(m_hComm, &commParam))
	{
		AfxMessageBox(_T("SetCommState error"));	
		//关闭串口
		CloseHandle (m_hComm);
		m_hComm = INVALID_HANDLE_VALUE;		
		return FALSE;
	}

	//设置串口读写时间
	COMMTIMEOUTS CommTimeOuts;
	GetCommTimeouts (m_hComm, &CommTimeOuts);
/*
	// 在整个串口的读写操作中， 存在着两种超时设置。一种是间隔超时， 一种是总超时。 这两种超时是独立存在，互不影响的。
	//间隔超时>只在读操作中存在。就是ReadIntervalTimeout。 当读操作中，前后两个字符之间的时间间隔超过时，读操作就结束了。
	举例来说，你一次读取8个字符，但是在你读取了第一个字符之后，在读取第二个字符时，间隔超时了，那么读操作就结束了， 
	这样整个操作就只读取了1个字节。&nbsp;即使， 你的总时间没有超时
	总超时>这里有一个公式 总的读/写超时时间 = Read(Write)TotalTimeoutMultiplier x 要读/写的字节数 + Read(Write)TotalTimeoutConstant. 
	这里要说明的一点，要读/写的字节数是从哪里来的。 这个是从ReadFile 或者WriteFile 函数中定义的。
	在读操作时， 若当前所花读取时间已经超过了总的超时设置， 则读操作就结束了。即使， 每两个字符之间的间隔没有超时。 
	举例来说， 若总共读取8个字节。 间隔设置为8ms， 总超时系数为3ms，总超时常数为3ms。 则总的超时时间为3*8+3=27ms。若每个字符读取的间隔为7ms， 
	则这次操作总共能读取4个字符。 就结束了。因为读取第5个字符时， 已经需要35ms， 超过总超时时间了。<br><br>下面来讨论一下这几个参数的设定：
	将ReadIntervalTimeout设置为MAXDWORD，将ReadTotalTimeoutMultiplier和ReadTotalTimeoutConstant设置为0，
	表示读操作将立即返回存放在输入缓冲区的字符。将ReadIntervalTimeout设置为MAXDWORD，将ReadTotalTimeoutMultiplier和ReadTotalTimeoutConstant设置为MAXDWORD， 
	表示读操作会一直等待直到所需要读取的字节数全部接收到为止。 （大家可以把MAXDWORD 认为是永远）
	将ReadIntervalTimeout设置为0， 则不使用间隔超时， 只考虑总超时设置。


	//两相邻字符之间最大的延时。当读串口数据时，一旦两个字符传输的时间间隔超过该时间，读函数将返回现有的数据。设置为0表示该参数不起作用。
	CommTimeOuts.ReadIntervalTimeout = 100;		
	//读操作总的超时事件的系数。&nbsp;这个变量是不能单独使用的。 必须和ReadTotalTimeoutConstant 一起使用才有效果
	CommTimeOuts.ReadTotalTimeoutMultiplier = 100;  
	//读操作总的超时时间的修正常量。 这个变量也是不能单独使用的。必须和ReadTotalTimeoutMultiplier一起使用才有效果。
	CommTimeOuts.ReadTotalTimeoutConstant = 150;    
	//写操作总的超时事件的系数。&nbsp;这个变量是不能单独使用的。 必须和WriteTotalTimeoutConstant 一起使用才有效果
	CommTimeOuts.WriteTotalTimeoutMultiplier = 10;  
	//写操作总的超时时间的修正常量。 这个变量也是不能单独使用的。必须和WriteTotalTimeoutMultiplier一起使用才有效果
	CommTimeOuts.WriteTotalTimeoutConstant = 100; */ 

	CommTimeOuts.ReadIntervalTimeout = MAXWORD;  
	CommTimeOuts.ReadTotalTimeoutMultiplier = 10;  
	CommTimeOuts.ReadTotalTimeoutConstant = 0;    
	CommTimeOuts.WriteTotalTimeoutMultiplier = 50;  
	CommTimeOuts.WriteTotalTimeoutConstant = 100;  

	if(!SetCommTimeouts( m_hComm, &CommTimeOuts ))
	{
		AfxMessageBox( _T("SetCommTimeouts") );
		//关闭串口
		CloseHandle (m_hComm);

		m_hComm = INVALID_HANDLE_VALUE;
		return FALSE;
	}

	//指定端口监测的事件集
	SetCommMask (m_hComm, EV_RXCHAR);
	//分配串口设备缓冲区
	SetupComm(m_hComm,1024,1024);
	//初始化缓冲区中的信息
	PurgeComm(m_hComm,PURGE_TXCLEAR|PURGE_RXCLEAR);


	m_bOpened = TRUE;
	return TRUE;
}

/*
*函数介绍：关闭串口
*/
void CSeries::ClosePort()
{	
	CloseReadThread();
	//表示串口还没有打开
	if (m_hComm == INVALID_HANDLE_VALUE)
	{
		return ;
	}
	//关闭串口
	CloseHandle (m_hComm);

	m_hComm = INVALID_HANDLE_VALUE;
	m_bOpened = FALSE;
}

//得到串口是否打开
BOOL CSeries::GetComOpened()
{
	return m_bOpened;
}

//清空串口
BOOL CSeries::fPurgeComm()
{
	PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR );
	return TRUE;
}

int CSeries::fRecv(BYTE * pBufRevc,WORD wOvertime)
{    
	BYTE * pBufReadData = new BYTE[1];
	ZeroMemory(pBufReadData,1);
	BYTE * pBufHeadData = new BYTE[4];
	ZeroMemory(pBufHeadData,4);
	
	DWORD dwTimeBegin = GetTickCount();
	DWORD dwActualReadLen = 0;	//每次读取到的实际长度
	DWORD dwRecvLen = 0;		//读取到的数据总长度
	DWORD dwDataLen = 0;		//数据帧长度

	ZeroMemory(pBufReadData,1);
	ZeroMemory(pBufHeadData,4);


	//读命令帧头
	while(TRUE)
	{
		if (GetTickCount() -  dwTimeBegin > 1000)
		{
			dwRecvLen = 0;
			break;
		}

		ReadFile(m_hComm, pBufReadData, 1, &dwActualReadLen,0);
		if (dwActualReadLen != 1)
			continue;
		m_OnSeriesRead(pBufReadData,1);

		pBufHeadData[0] = pBufHeadData[1];
		pBufHeadData[1] = pBufHeadData[2];
		pBufHeadData[2] = pBufHeadData[3]; 
		pBufHeadData[3] = pBufReadData[0];

		if (pBufHeadData[0] == 0x55 && pBufHeadData[1] == 0xAA &&
			pBufHeadData[2] == 0x81 && pBufHeadData[3] == 0x01)
		{
			memcpy(pBufRevc,pBufHeadData,4);
			dwRecvLen = 4;
			break;
		}						
	}

	//如果读数据帧失败，直接返回超时
	while(TRUE)
	{		
		if (GetTickCount() -  dwTimeBegin > 1000)
		{
			dwRecvLen = 0;
			break;
		}
		ReadFile(m_hComm, pBufReadData, 1, &dwActualReadLen,0);
		if (dwActualReadLen != 1)
			continue;

		//每次读取一个字节，并保存
		memcpy(pBufRevc + dwRecvLen,pBufReadData,1);
		dwRecvLen++;

		//计算数据帧长度
		if(dwRecvLen == 7)
			dwDataLen = pBufRevc[5]*0100 + pBufRevc[6];

		//读取完成退出循环
		if (dwRecvLen == 7+dwDataLen)
			break;		
	}

	delete pBufHeadData;
	pBufHeadData = NULL;

	delete pBufReadData;
	pBufReadData = NULL;

	return dwRecvLen;

}