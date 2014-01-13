#pragma once
#include "Series.h"
#include "OBDPNDDlgBase.h"

#define DATASTREAM_GET_OVERTIME			10000			//��ȡ���ݳ�ʱ
#define INVALID_DATASTREAM_VALUE		123456			//��Ч����
#define INVALID_DATASTREAM_VALUE_STR	_T("------")	//��Ч�ı�

struct TagDataStreamValue
{
	CString strText;			//�����ı�
	float	fValue;				//����ֵ
	int		iDecLen;			//С��λ����
	CString strValue;			//�ַ���ʽ����ֵ
	DWORD	dwLastGetDataTime;	//���һ�λ�ȡ����ʱ��
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
	//���崮�ڽ������ݺ�������
	static void CALLBACK OnSerialRead(BYTE* buf,DWORD bufLen);
	void fCaculateData(BYTE * pBufData);
public:
	static COBDPNDCommunication * m_pThis;//��̬����ָ��
	// �����ô���
	int fCheckVaildSerialPort();
	// ���ڶ�ȡ
	BOOL fStartSeries(int iPort = -1);
	// ֹͣ��ȡ
	void fStopSerial();

	// ��ȡ����������
	CString fGetDataStreamValue(DWORD dwDsID);
	float fGetDataStreamValueF(DWORD dwDsID);
	CString fGetDataStreamText(DWORD dwDsID);
	void fCheckOverTime();
	map<DWORD,TagDtcInfo> fGetDtcList();
private:
	CSeries * m_pSeries;

	//��ȡ�����߳�
	DWORD	m_dwThreadReadSeries;
	HANDLE	m_hThreadReadSeries;

	BOOL	m_bIsVaildComm;

	map<DWORD,TagDataStreamValue>	m_mapDataStreamValue;		//������ֵ
	map<DWORD,TagDtcInfo>			m_mapDtcList;				//��������Ϣ

	WORD	m_wLastSpeed;	//�������ĳ���
};
