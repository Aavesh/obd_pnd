/*
创建:	Yipb	2011-11-10
说明:	串口通讯类

*/
#pragma once

#define  DEFAULT_OVERTIME	5000		//默认超时时间（ms）
#define  MAX_CMD_SIZE		300			//指令最大字节数

//串口接收到数据
#define WM_SERIES_RECV			WM_USER + 0x00010	//接收到串口数据
#define WM_SERIES_RESTART		WM_USER + 0x00011	//重新开启串口

enum COMM_RET
{
	COMM_RET_OK,			//执行成功
	COMM_RET_OVERTIME,		//命令超时
	COMM_RET_REJECT,		//否定应答
	COMM_RET_CHECK_FAIL,	//效验失败
	COMM_RET_HEAD_ERR,		//数据头失败
	COMM_RET_DATA_ERR,		//数据段失败
};


//定义串口接收数据函数类型
typedef void (CALLBACK* ONSERIESREAD)(BYTE* buf  /*接收到的缓冲区*/
									  ,DWORD dwBufLen /*接收到的缓冲区长度*/);

class CSeries
{
private:
	//串口读线程函数
	static  DWORD WINAPI ReadThreadFunc(LPVOID lparam);
	//关闭读线程
	void CloseReadThread();
public:
	CSeries(void);
	~CSeries(void);

	//打开读线程
	void OpenReadThread();
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
	BOOL OpenPort( HWND hOwner,UINT portNo = 1,UINT baud = 115200,UINT parity = NOPARITY,UINT databits = 8,UINT stopbits = 0);
	//关闭串口
	void ClosePort();
	//得到串口是否打开
	BOOL GetComOpened();
	//清空串口
	BOOL fPurgeComm();
	
	int fRecv(BYTE * pBufRevc,WORD wOvertime = DEFAULT_OVERTIME);

private:	
	HANDLE	m_hComm;		//串口句柄
	BOOL	m_bOpened;		//串口是否打开

	//读线程句柄
	HANDLE m_hReadThread;
	//读线程ID标识
	DWORD m_dwReadThreadID;
	//读线程退出事件
	HANDLE m_hReadCloseEvent;

	HWND m_hOwner;

public:
	ONSERIESREAD m_OnSeriesRead; //串口读取回调函数

};
