/*
����:	Yipb	2011-11-10
˵��:	����ͨѶ��

*/
#pragma once

#define  DEFAULT_OVERTIME	5000		//Ĭ�ϳ�ʱʱ�䣨ms��
#define  MAX_CMD_SIZE		300			//ָ������ֽ���

//���ڽ��յ�����
#define WM_SERIES_RECV			WM_USER + 0x00010	//���յ���������
#define WM_SERIES_RESTART		WM_USER + 0x00011	//���¿�������

enum COMM_RET
{
	COMM_RET_OK,			//ִ�гɹ�
	COMM_RET_OVERTIME,		//���ʱ
	COMM_RET_REJECT,		//��Ӧ��
	COMM_RET_CHECK_FAIL,	//Ч��ʧ��
	COMM_RET_HEAD_ERR,		//����ͷʧ��
	COMM_RET_DATA_ERR,		//���ݶ�ʧ��
};


//���崮�ڽ������ݺ�������
typedef void (CALLBACK* ONSERIESREAD)(BYTE* buf  /*���յ��Ļ�����*/
									  ,DWORD dwBufLen /*���յ��Ļ���������*/);

class CSeries
{
private:
	//���ڶ��̺߳���
	static  DWORD WINAPI ReadThreadFunc(LPVOID lparam);
	//�رն��߳�
	void CloseReadThread();
public:
	CSeries(void);
	~CSeries(void);

	//�򿪶��߳�
	void OpenReadThread();
	//************************************************************************************
	//��    ����fSendCmd(BYTE * bufData)      
	//��    �ܣ��򿪴���                                                  
	//��    ����void* pOwner----------ָ��ָ��
	//			UINT portNo-----------���ں�		= 1,		
	//			UINT baud-------------������		= 115200,
	//			UINT parity-----------��żУ��		= NOPARITY,
	//			UINT databits---------����λ		= 8,
	//			UINT stopbits---------ֹͣλ		= 1
	//��    ��: �򿪴����Ƿ�ɹ�
	//************************************************************************************
	BOOL OpenPort( HWND hOwner,UINT portNo = 1,UINT baud = 115200,UINT parity = NOPARITY,UINT databits = 8,UINT stopbits = 0);
	//�رմ���
	void ClosePort();
	//�õ������Ƿ��
	BOOL GetComOpened();
	//��մ���
	BOOL fPurgeComm();
	
	int fRecv(BYTE * pBufRevc,WORD wOvertime = DEFAULT_OVERTIME);

private:	
	HANDLE	m_hComm;		//���ھ��
	BOOL	m_bOpened;		//�����Ƿ��

	//���߳̾��
	HANDLE m_hReadThread;
	//���߳�ID��ʶ
	DWORD m_dwReadThreadID;
	//���߳��˳��¼�
	HANDLE m_hReadCloseEvent;

	HWND m_hOwner;

public:
	ONSERIESREAD m_OnSeriesRead; //���ڶ�ȡ�ص�����

};
