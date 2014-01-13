
#include "StdAfx.h"
#include "SeriesGPS.h"

HANDLE theGPSMutex = NULL;
//���캯��
CSeriesGPS::CSeriesGPS()
{
	//��ʼ���ڲ�����
	m_hComm = INVALID_HANDLE_VALUE;
	m_OnSeriesRead = NULL;
	m_bOpened = 0;

	theGPSMutex = CreateMutex(NULL,FALSE,NULL);
	m_fCompassValue = 0;
	m_bUpdateDateTimeByGps = FALSE;
}

//��������
CSeriesGPS::~CSeriesGPS()
{
	if (m_bOpened)
	{
		//�رմ���
		ClosePort();
	}
}

//���ڶ��̺߳���
DWORD CSeriesGPS::ReadThreadFunc(LPVOID lparam)
{
	CSeriesGPS *ceSeries = (CSeriesGPS*)lparam;
	
	DWORD	evtMask;
	BYTE * readBuf = NULL;//��ȡ���ֽ�
	DWORD actualReadLen=0;//ʵ�ʶ�ȡ���ֽ���
	DWORD willReadLen;
	
	DWORD dwReadErrors;
	COMSTAT	cmState;
	
	// ��ջ��壬����鴮���Ƿ�򿪡�
	ASSERT(ceSeries->m_hComm !=INVALID_HANDLE_VALUE); 
	
	
	//��մ���
	PurgeComm(ceSeries->m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR );
	
	SetCommMask (ceSeries->m_hComm, EV_RXCHAR | EV_CTS | EV_DSR );
	while (TRUE)
	{   	
		if (WaitCommEvent(ceSeries->m_hComm,&evtMask,0))
		{			
			SetCommMask (ceSeries->m_hComm, EV_RXCHAR | EV_CTS | EV_DSR );
			//��ʾ�����յ��ַ�		
			if (evtMask & EV_RXCHAR) 
			{
				ClearCommError(ceSeries->m_hComm,&dwReadErrors,&cmState);
				willReadLen = cmState.cbInQue ;
				if (willReadLen <= 0)
				{
					continue;
				}
				
				//�����ڴ�
				readBuf = new BYTE[willReadLen];
				ZeroMemory(readBuf,willReadLen);
				//��ȡ��������
				ReadFile(ceSeries->m_hComm, readBuf, willReadLen, &actualReadLen,0);

				//�����ȡ�����ݴ���0��
				if (actualReadLen>0)
				{
					CString strTemp;
					CString strText;
					CString strGpsInfo;

					for (int i=0;i<actualReadLen;i++)
					{
						strTemp.Format(_T("%c"),readBuf[i]);
						strGpsInfo += strTemp;
					}

					vector<CString> vecGPRMCInfo;
					vector<CString> vecGPGGAInfo;

					WORD wBegin = 0;
					WORD wEnd = 0;

					wBegin = strGpsInfo.Find(_T("$GPRMC"),0);
					wEnd = 0;
					if (wBegin >=0)
					{
						wEnd = strGpsInfo.Find(_T("\r\n"),wBegin);
						strText = strGpsInfo.Mid(wBegin,wEnd - wBegin);

						//GPS��Ϣ
						vecGPRMCInfo = fCommStrSplit(strText,',');

						if (vecGPRMCInfo[2] == _T("V"))
						{if (!ceSeries->m_bUpdateDateTimeByGps)
						{		
							//����ϵͳʱ��
							if (vecGPRMCInfo[2] != _T("") && vecGPRMCInfo[9] != _T(""))
							{
								SYSTEMTIME st;
								st.wYear = (WORD)_ttoi(vecGPRMCInfo[9].Mid(4,2));
								st.wMonth = (WORD)_ttoi(vecGPRMCInfo[9].Mid(2,2));
								st.wDay = (WORD)_ttoi(vecGPRMCInfo[9].Mid(0,2));

								st.wHour = (WORD)_ttoi(vecGPRMCInfo[1].Mid(0,2));
								st.wMinute = (WORD)_ttoi(vecGPRMCInfo[1].Mid(2,2));
								st.wSecond = (WORD)_ttoi(vecGPRMCInfo[1].Mid(4,2));

								SetSystemTime(&st);	
								ceSeries->m_bUpdateDateTimeByGps = TRUE;
							}
						}
							//��ָ�뷽��
							if (vecGPRMCInfo.size()>=8)
							{
								WaitForSingleObject(theGPSMutex,INFINITE);
								ceSeries->m_fCompassValue = 180+wcstod(vecGPRMCInfo[8],0);
								ReleaseMutex(theGPSMutex);
							}
						}
					}
				}

				//�ͷ��ڴ�
				delete[] readBuf;
				readBuf = NULL;
			}
		}
		//����յ����߳��˳��źţ����˳��߳�
		if (WaitForSingleObject(ceSeries->m_hReadCloseEvent,5000) == WAIT_OBJECT_0)
		{
			break;
		}
	}
	return 0;
}


float CSeriesGPS::fGetComPassValue()
{
	float fValue = 0;
	WaitForSingleObject(theGPSMutex,INFINITE);
	fValue = m_fCompassValue;
	ReleaseMutex(theGPSMutex);
	return fValue;
}

//�رն��߳�
void CSeriesGPS::CloseReadThread()
{
	SetEvent(m_hReadCloseEvent);
	//���������¼���Ч��Ч
	SetCommMask(m_hComm, 0);
	//������н�Ҫ��������
    PurgeComm( m_hComm,  PURGE_RXCLEAR );
    //�ȴ�4�룬������߳�û���˳�����ǿ���˳�
    if (WaitForSingleObject(m_hReadThread,4000) == WAIT_TIMEOUT)
	{
		TerminateThread(m_hReadThread,0);
	}
	m_hReadThread = NULL;
}

/*
*�������ܣ��򿪴���
*��ڲ�����pPortOwner	:ʹ�ô˴�����Ĵ�����
		   portNo		:���ں�
		   baud			:������
		   parity		:��żУ��
		   databits		:����λ
		   stopbits		:ֹͣλ
*���ڲ�����(��)
*����ֵ��TRUE:�ɹ��򿪴���;FALSE:�򿪴���ʧ��
*/
BOOL CSeriesGPS::OpenPort(HWND pOwner,
						 UINT portNo	,			/*���ں�*/
						 UINT baud		,			/*������*/
						 UINT parity	,			/*��żУ��*/
						 UINT databits	,			/*����λ*/
						 UINT stopbits			   /*ֹͣλ*/
						 )
{
	DCB commParam;
	TCHAR szPort[15];	

	ASSERT(pOwner!=NULL);
	m_hOwner = pOwner;
	
	// �Ѿ��򿪵Ļ���ֱ�ӷ���
	if (m_hComm != INVALID_HANDLE_VALUE)
	{
		return TRUE;
	}
	
	//���ô�����
	wsprintf(szPort, L"COM%d:", portNo);
	//�򿪴���
	m_hComm = CreateFile(
		szPort,
		GENERIC_READ | GENERIC_WRITE,	//�������д
		0,								//��ռ��ʽ������ģʽ��
		NULL,
		OPEN_EXISTING,					//�򿪶����Ǵ�����������ʽ��
		0,
		NULL 
		);
	
	if (m_hComm == INVALID_HANDLE_VALUE)
	{
		// ��Ч���,���ء�		
		TRACE(_T("CreateFile ������Ч���\n"));
		return FALSE;
		
	}
	
	// �õ��򿪴��ڵĵ�ǰ���Բ������޸ĺ����������ô��ڡ�
	if (!GetCommState(m_hComm,&commParam))
	{		
		//�رմ���
		CloseHandle (m_hComm);
		m_hComm = INVALID_HANDLE_VALUE;
		return FALSE;
	}
	
	//���ô��ڲ���
	commParam.BaudRate = baud;					// ���ò����� 
	commParam.fBinary = TRUE;					// ���ö�����ģʽ���˴���������TRUE
	commParam.fParity = TRUE;					// ֧����żУ�� 
	commParam.ByteSize = databits;				// ����λ,��Χ:4-8 
	commParam.Parity = parity;				// У��ģʽ
	commParam.StopBits = stopbits;				// ֹͣλ 
	
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
	commParam.fAbortOnError = FALSE;			// �����ڷ������󣬲�����ֹ���ڶ�д
	
	//���ô��ڲ��� 
	if (!SetCommState(m_hComm, &commParam))
	{
		TRACE(_T("SetCommState error"));	
		//�رմ���
		CloseHandle (m_hComm);
		m_hComm = INVALID_HANDLE_VALUE;		
		return FALSE;
	}
	
	//���ô��ڶ�дʱ��
	COMMTIMEOUTS CommTimeOuts;
	GetCommTimeouts (m_hComm, &CommTimeOuts);
	CommTimeOuts.ReadIntervalTimeout = MAXDWORD;  
	CommTimeOuts.ReadTotalTimeoutMultiplier = 100;  
	CommTimeOuts.ReadTotalTimeoutConstant = 0;    
	CommTimeOuts.WriteTotalTimeoutMultiplier = 10;  
	CommTimeOuts.WriteTotalTimeoutConstant = 1000;  
	if(!SetCommTimeouts( m_hComm, &CommTimeOuts ))
	{
		TRACE( _T("SetCommTimeouts ���ش���") );
		//�رմ���
		CloseHandle (m_hComm);

		m_hComm = INVALID_HANDLE_VALUE;
		return FALSE;
	}
	
	//ָ���˿ڼ����¼���
	SetCommMask (m_hComm, EV_RXCHAR);
	//���䴮���豸������
	SetupComm(m_hComm,512,512);
	//��ʼ���������е���Ϣ
	PurgeComm(m_hComm,PURGE_TXCLEAR|PURGE_RXCLEAR);
	
	CString strEvent;
	strEvent.Format(L"Com_ReadCloseEvent%d",portNo);
	m_hReadCloseEvent = CreateEvent(NULL,TRUE,FALSE,strEvent);

	//�������ڶ����ݼ����߳�
	m_hReadThread = CreateThread(NULL,0,ReadThreadFunc,this,0,&m_dwReadThreadID);
	
	TRACE(_T("���ڴ򿪳ɹ�"));
	m_bOpened = TRUE;
	return TRUE;
}

/*
*�������ܣ��رմ���
*��ڲ�����(��)
*���ڲ�����(��)
*����ֵ��  (��)
*/
void CSeriesGPS::ClosePort()
{	
	//��ʾ���ڻ�û�д�
	if (m_hComm == INVALID_HANDLE_VALUE)
	{
		return ;
	}
	
	//�رն��߳�
	CloseReadThread();
	
	//�رմ���
	CloseHandle (m_hComm);
	//�ر��¼�
	CloseHandle(m_hReadCloseEvent);

	m_hComm = INVALID_HANDLE_VALUE;
	m_bOpened = FALSE;
}

/*
*�������ܣ�������д������
*��ڲ�����buf ����д�����ݻ�����
	       bufLen : ��д�뻺��������
*���ڲ�����(��)
*����ֵ��TRUE:���óɹ�;FALSE:����ʧ��
*/
BOOL CSeriesGPS::WriteSyncPort(const BYTE*buf , DWORD bufLen)
{
	DWORD dwNumBytesWritten;
	DWORD dwHaveNumWritten =0 ; //�Ѿ�д�����
	
	int iInc = 0; //���3��д�벻�ɹ�������FALSE
	ASSERT(m_hComm != INVALID_HANDLE_VALUE);
	do
	{
		if (WriteFile (m_hComm,					//���ھ�� 
			buf+dwHaveNumWritten,				//��д���ݻ����� 
			bufLen - dwHaveNumWritten,          //��д���ݻ�������С
			&dwNumBytesWritten,					//����ִ�гɹ��󣬷���ʵ���򴮿�д�ĸ���	
			NULL))								//�˴���������NULL
		{
			dwHaveNumWritten = dwHaveNumWritten + dwNumBytesWritten;
			//д�����
			if (dwHaveNumWritten == bufLen)
			{
				break;
			}
			iInc++;
			if (iInc >= 3)
			{
				return FALSE;
			}
			Sleep(10);
		}
		else
		{
			return FALSE;
		}
	}while (TRUE);
	
	return TRUE;		
}

/*
*�������ܣ����ô��ڶ�ȡ��д�볬ʱ
*��ڲ�����CommTimeOuts : ָ��COMMTIMEOUTS�ṹ
*���ڲ�����(��)
*����ֵ��TRUE:���óɹ�;FALSE:����ʧ��
*/
BOOL CSeriesGPS::SetSeriesTimeouts(COMMTIMEOUTS CommTimeOuts)
{
	ASSERT(m_hComm != INVALID_HANDLE_VALUE);
	return SetCommTimeouts(m_hComm,&CommTimeOuts);
}


//�õ������Ƿ��
BOOL CSeriesGPS::GetComOpened()
{
	return m_bOpened;
}