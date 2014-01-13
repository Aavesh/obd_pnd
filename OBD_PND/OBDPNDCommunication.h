#pragma once
#include "Series.h"
#include "OBDPNDDlgBase.h"

#define DATASTREAM_GET_OVERTIME			10000			//获取数据超时
#define INVALID_DATASTREAM_VALUE		123456			//无效数据
#define INVALID_DATASTREAM_VALUE_STR	_T("------")	//无效文本

struct TagDataStreamValue
{
	CString strText;			//数据文本
	float	fValue;				//数据值
	int		iDecLen;			//小数位长度
	CString strValue;			//字符格式数据值
	DWORD	dwLastGetDataTime;	//最后一次获取数据时间
	TagDataStreamValue()
	{
		strText = _T("");
		fValue = INVALID_DATASTREAM_VALUE;
		iDecLen = 0;
		dwLastGetDataTime = 0;
		strValue = _T("------");
	}
};

struct TagDtcInfo 
{
	DWORD dwEcuID;
	DWORD dwDtcID;
	CString strDtcText;
};
class COBDPNDCommunication
{
public:
	COBDPNDCommunication(void);
	~COBDPNDCommunication(void);
private:
	//定义串口接收数据函数类型
	static void CALLBACK OnSerialRead(BYTE* buf,DWORD bufLen);
	void fCaculateData(BYTE * pBufData);
public:
	static COBDPNDCommunication * m_pThis;//静态对象指针
	// 检测可用串口
	int fCheckVaildSerialPort();
	// 串口读取
	BOOL fStartSeries(int iPort = -1);
	// 停止读取
	void fStopSerial();

	// 获取数据流数据
	CString fGetDataStreamValue(DWORD dwDsID);
	float fGetDataStreamValueF(DWORD dwDsID);
	CString fGetDataStreamText(DWORD dwDsID);
	void fCheckOverTime();
	map<DWORD,TagDtcInfo> fGetDtcList();
private:
	CSeries * m_pSeries;

	//读取串口线程
	DWORD	m_dwThreadReadSeries;
	HANDLE	m_hThreadReadSeries;

	BOOL	m_bIsVaildComm;

	map<DWORD,TagDataStreamValue>	m_mapDataStreamValue;		//数据流值
	map<DWORD,TagDtcInfo>			m_mapDtcList;				//故障码信息

	WORD	m_wLastSpeed;	//保存最后的车速
};
