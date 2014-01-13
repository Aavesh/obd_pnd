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

//���ڶ��̺߳���
DWORD CSeries::ReadThreadFunc(LPVOID lparam)
{
	CSeries *ceSeries = (CSeries*)lparam;

	BYTE * pBufReadData = new BYTE[1];
	ZeroMemory(pBufReadData,1);
	BYTE * pBufHeadData = new BYTE[4];
	ZeroMemory(pBufHeadData,4);

	BYTE * pBufRecv = new BYTE[MAX_CMD_SIZE];
	ZeroMemory(pBufRecv,MAX_CMD_SIZE);

	DWORD dwActualReadLen = 0;	//ÿ�ζ�ȡ����ʵ�ʳ���
	DWORD dwRecvLen = 0;		//��ȡ���������ܳ���
	DWORD dwDataLen = 0;		//����֡����

	DWORD dwBeginTime = GetTickCount();		//��ȡ��ʼʱ��
	BOOL bReadOverTime = FALSE;

	while (TRUE)
	{
		ZeroMemory(pBufReadData,1);
		ZeroMemory(pBufHeadData,4);
		ZeroMemory(pBufRecv,MAX_CMD_SIZE);

		dwActualReadLen = 0;	//ÿ�ζ�ȡ����ʵ�ʳ���
		dwRecvLen = 0;		//��ȡ���������ܳ���
		dwDataLen = 0;		//����֡����

		dwBeginTime = GetTickCount();		//��ȡ��ʼʱ��
		bReadOverTime = FALSE;
		//
		while(TRUE)
		{
			////��ʱ�˳�
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

		//��ȡͷ��ɣ���ȡ���ݶ�
		if (!bReadOverTime)
		{
			//���������֡ʧ�ܣ�ֱ�ӷ��س�ʱ
			while(TRUE)
			{	
				////��ʱ�˳�
				//if((GetTickCount() - dwBeginTime) > DEFAULT_OVERTIME)
				//{
				//	dwRecvLen = 0;
				//	bReadOverTime = TRUE;
				//	break;
				//}

				ReadFile(ceSeries->m_hComm, pBufReadData, 1, &dwActualReadLen,0);
				if (dwActualReadLen != 1)
					continue;

				//ÿ�ζ�ȡһ���ֽڣ�������
				memcpy(pBufRecv + dwRecvLen,pBufReadData,1);
				dwRecvLen++;

				//ceSeries->m_OnSeriesRead(pBufRecv,dwRecvLen);

				//��������֡����
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

				//��ȡ����˳�ѭ��
				if (dwRecvLen == 7+dwDataLen)
					break;		
			}

		}

		if (!bReadOverTime)
		{
			if (dwRecvLen > 0)
			{
				//�Խ��յ����ݽ���Ч��λ�ж�
				CDecrypt cDecrypt;
				DWORD dwCmdID = pBufRecv[7]*0x100 + pBufRecv[8];	//����ID
				WORD  wLen =  pBufRecv[5]*0x100 + pBufRecv[6];		//����֡+����ID�ĳ���
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

//�ش򿪶��߳�
void CSeries::OpenReadThread()
{
	CloseReadThread();
	//�������ڶ����ݼ����߳�
	m_hReadThread = CreateThread(NULL,0,ReadThreadFunc,this,0,&m_dwReadThreadID);
}

//�رն��߳�
void CSeries::CloseReadThread()
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
BOOL CSeries::OpenPort(HWND hOwner,UINT portNo,UINT baud,UINT parity,UINT databits,UINT stopbits)
{
	DCB commParam;
	TCHAR szPort[15];	

	//ASSERT(pOwner!=NULL);
	m_hOwner = hOwner;

	// �Ѿ��򿪵Ļ���ֱ�ӹر�
	if (m_hComm != INVALID_HANDLE_VALUE)
	{
		//�رմ���
		CloseHandle (m_hComm);

		m_hComm = INVALID_HANDLE_VALUE;
		m_bOpened = FALSE;
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
		FILE_FLAG_OVERLAPPED,
		NULL 
		);

	if (m_hComm == INVALID_HANDLE_VALUE)
	{
		// ��Ч���,���ء�		
		//theMainDlg->fAddLog(_T("CreateFile ������Ч���\n"));
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
	commParam.Parity = parity;				    // У��ģʽ
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
		AfxMessageBox(_T("SetCommState error"));	
		//�رմ���
		CloseHandle (m_hComm);
		m_hComm = INVALID_HANDLE_VALUE;		
		return FALSE;
	}

	//���ô��ڶ�дʱ��
	COMMTIMEOUTS CommTimeOuts;
	GetCommTimeouts (m_hComm, &CommTimeOuts);
/*
	// ���������ڵĶ�д�����У� ���������ֳ�ʱ���á�һ���Ǽ����ʱ�� һ�����ܳ�ʱ�� �����ֳ�ʱ�Ƕ������ڣ�����Ӱ��ġ�
	//�����ʱ>ֻ�ڶ������д��ڡ�����ReadIntervalTimeout�� ���������У�ǰ�������ַ�֮���ʱ��������ʱ���������ͽ����ˡ�
	������˵����һ�ζ�ȡ8���ַ������������ȡ�˵�һ���ַ�֮���ڶ�ȡ�ڶ����ַ�ʱ�������ʱ�ˣ���ô�������ͽ����ˣ� 
	��������������ֻ��ȡ��1���ֽڡ�&nbsp;��ʹ�� �����ʱ��û�г�ʱ
	�ܳ�ʱ>������һ����ʽ �ܵĶ�/д��ʱʱ�� = Read(Write)TotalTimeoutMultiplier x Ҫ��/д���ֽ��� + Read(Write)TotalTimeoutConstant. 
	����Ҫ˵����һ�㣬Ҫ��/д���ֽ����Ǵ��������ġ� ����Ǵ�ReadFile ����WriteFile �����ж���ġ�
	�ڶ�����ʱ�� ����ǰ������ȡʱ���Ѿ��������ܵĳ�ʱ���ã� ��������ͽ����ˡ���ʹ�� ÿ�����ַ�֮��ļ��û�г�ʱ�� 
	������˵�� ���ܹ���ȡ8���ֽڡ� �������Ϊ8ms�� �ܳ�ʱϵ��Ϊ3ms���ܳ�ʱ����Ϊ3ms�� ���ܵĳ�ʱʱ��Ϊ3*8+3=27ms����ÿ���ַ���ȡ�ļ��Ϊ7ms�� 
	����β����ܹ��ܶ�ȡ4���ַ��� �ͽ����ˡ���Ϊ��ȡ��5���ַ�ʱ�� �Ѿ���Ҫ35ms�� �����ܳ�ʱʱ���ˡ�<br><br>����������һ���⼸���������趨��
	��ReadIntervalTimeout����ΪMAXDWORD����ReadTotalTimeoutMultiplier��ReadTotalTimeoutConstant����Ϊ0��
	��ʾ���������������ش�������뻺�������ַ�����ReadIntervalTimeout����ΪMAXDWORD����ReadTotalTimeoutMultiplier��ReadTotalTimeoutConstant����ΪMAXDWORD�� 
	��ʾ��������һֱ�ȴ�ֱ������Ҫ��ȡ���ֽ���ȫ�����յ�Ϊֹ�� ����ҿ��԰�MAXDWORD ��Ϊ����Զ��
	��ReadIntervalTimeout����Ϊ0�� ��ʹ�ü����ʱ�� ֻ�����ܳ�ʱ���á�


	//�������ַ�֮��������ʱ��������������ʱ��һ�������ַ������ʱ����������ʱ�䣬���������������е����ݡ�����Ϊ0��ʾ�ò����������á�
	CommTimeOuts.ReadIntervalTimeout = 100;		
	//�������ܵĳ�ʱ�¼���ϵ����&nbsp;��������ǲ��ܵ���ʹ�õġ� �����ReadTotalTimeoutConstant һ��ʹ�ò���Ч��
	CommTimeOuts.ReadTotalTimeoutMultiplier = 100;  
	//�������ܵĳ�ʱʱ������������� �������Ҳ�ǲ��ܵ���ʹ�õġ������ReadTotalTimeoutMultiplierһ��ʹ�ò���Ч����
	CommTimeOuts.ReadTotalTimeoutConstant = 150;    
	//д�����ܵĳ�ʱ�¼���ϵ����&nbsp;��������ǲ��ܵ���ʹ�õġ� �����WriteTotalTimeoutConstant һ��ʹ�ò���Ч��
	CommTimeOuts.WriteTotalTimeoutMultiplier = 10;  
	//д�����ܵĳ�ʱʱ������������� �������Ҳ�ǲ��ܵ���ʹ�õġ������WriteTotalTimeoutMultiplierһ��ʹ�ò���Ч��
	CommTimeOuts.WriteTotalTimeoutConstant = 100; */ 

	CommTimeOuts.ReadIntervalTimeout = MAXWORD;  
	CommTimeOuts.ReadTotalTimeoutMultiplier = 10;  
	CommTimeOuts.ReadTotalTimeoutConstant = 0;    
	CommTimeOuts.WriteTotalTimeoutMultiplier = 50;  
	CommTimeOuts.WriteTotalTimeoutConstant = 100;  

	if(!SetCommTimeouts( m_hComm, &CommTimeOuts ))
	{
		AfxMessageBox( _T("SetCommTimeouts") );
		//�رմ���
		CloseHandle (m_hComm);

		m_hComm = INVALID_HANDLE_VALUE;
		return FALSE;
	}

	//ָ���˿ڼ����¼���
	SetCommMask (m_hComm, EV_RXCHAR);
	//���䴮���豸������
	SetupComm(m_hComm,1024,1024);
	//��ʼ���������е���Ϣ
	PurgeComm(m_hComm,PURGE_TXCLEAR|PURGE_RXCLEAR);


	m_bOpened = TRUE;
	return TRUE;
}

/*
*�������ܣ��رմ���
*/
void CSeries::ClosePort()
{	
	CloseReadThread();
	//��ʾ���ڻ�û�д�
	if (m_hComm == INVALID_HANDLE_VALUE)
	{
		return ;
	}
	//�رմ���
	CloseHandle (m_hComm);

	m_hComm = INVALID_HANDLE_VALUE;
	m_bOpened = FALSE;
}

//�õ������Ƿ��
BOOL CSeries::GetComOpened()
{
	return m_bOpened;
}

//��մ���
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
	DWORD dwActualReadLen = 0;	//ÿ�ζ�ȡ����ʵ�ʳ���
	DWORD dwRecvLen = 0;		//��ȡ���������ܳ���
	DWORD dwDataLen = 0;		//����֡����

	ZeroMemory(pBufReadData,1);
	ZeroMemory(pBufHeadData,4);


	//������֡ͷ
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

	//���������֡ʧ�ܣ�ֱ�ӷ��س�ʱ
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

		//ÿ�ζ�ȡһ���ֽڣ�������
		memcpy(pBufRevc + dwRecvLen,pBufReadData,1);
		dwRecvLen++;

		//��������֡����
		if(dwRecvLen == 7)
			dwDataLen = pBufRevc[5]*0100 + pBufRevc[6];

		//��ȡ����˳�ѭ��
		if (dwRecvLen == 7+dwDataLen)
			break;		
	}

	delete pBufHeadData;
	pBufHeadData = NULL;

	delete pBufReadData;
	pBufReadData = NULL;

	return dwRecvLen;

}