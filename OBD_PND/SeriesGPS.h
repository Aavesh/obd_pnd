
#pragma once

#define WM_COMM_GPS WM_USER + 301

//���崮�ڽ������ݺ�������
typedef void (CALLBACK* ONSERIESREADGPS)(void * pOwner /*������ָ��*/
									  ,BYTE* buf  /*���յ��Ļ�����*/
									  ,DWORD dwBufLen /*���յ��Ļ���������*/);

class CSeriesGPS
{
public:
	CSeriesGPS(void);
	~CSeriesGPS(void);
public:
	//�򿪴���
	BOOL OpenPort(HWND pOwner,/*ָ��ָ��*/
				  UINT portNo	= 1,		/*���ں�*/
				  UINT baud		= 9600,	/*������*/
				  UINT parity	= NOPARITY, /*��żУ��*/
				  UINT databits	= 8,		/*����λ*/
				  UINT stopbits	= 0        /*ֹͣλ*/
				  );
	//�رմ���
	void ClosePort();
	//ͬ��д������
	BOOL WriteSyncPort(const BYTE*buf , DWORD bufLen);
	//���ô��ڶ�ȡ��д�볬ʱ
	BOOL SetSeriesTimeouts(COMMTIMEOUTS CommTimeOuts);
	//�õ������Ƿ��
	BOOL GetComOpened();
	
	float fGetComPassValue();
private:
    //���ڶ��̺߳���
    static  DWORD WINAPI ReadThreadFunc(LPVOID lparam);
private:
	//�رն��߳�
	void CloseReadThread();
private:
    //�Ѵ򿪵Ĵ��ھ��
	HANDLE	m_hComm;
	//���߳̾��
	HANDLE m_hReadThread;
	//���߳�ID��ʶ
	DWORD m_dwReadThreadID;
	//���߳��˳��¼�
	HANDLE m_hReadCloseEvent;
	BOOL m_bOpened; //�����Ƿ��

	HWND m_hOwner; //ָ��������ָ��

	float	m_fCompassValue;
	BOOL	m_bUpdateDateTimeByGps;
public:
	ONSERIESREADGPS m_OnSeriesRead; //���ڶ�ȡ�ص�����
};
