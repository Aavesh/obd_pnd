#include "StdAfx.h"
#include "OBDPNDCommunication.h"
#include "OBD_PNDDlg.h"

HANDLE theMutex = NULL;
COBDPNDCommunication* COBDPNDCommunication::m_pThis=NULL;

COBDPNDCommunication::COBDPNDCommunication(void)
{	
	m_pSeries = new CSeries();
	m_pSeries->m_OnSeriesRead = OnSerialRead; //	
	theMutex=CreateMutex(NULL,FALSE,NULL);
	m_pThis = this;

	m_wLastSpeed = 0;
}

COBDPNDCommunication::~COBDPNDCommunication(void)
{
}


CString COBDPNDCommunication::fGetDataStreamValue(DWORD dwDsID)
{
	CString strRet = _T("------");

	WaitForSingleObject(theMutex,INFINITE);
	if (m_mapDataStreamValue.find(dwDsID) != m_mapDataStreamValue.end())
	{
		strRet = m_mapDataStreamValue[dwDsID].strValue;
	}
	ReleaseMutex(theMutex);

	return strRet;
}

float COBDPNDCommunication::fGetDataStreamValueF(DWORD dwDsID)
{
	float fRet = INVALID_DATASTREAM_VALUE;

	WaitForSingleObject(theMutex,INFINITE);
	if (m_mapDataStreamValue.find(dwDsID) != m_mapDataStreamValue.end())
	{
		fRet = m_mapDataStreamValue[dwDsID].fValue;
	}
	ReleaseMutex(theMutex);

	return fRet;
}

CString COBDPNDCommunication::fGetDataStreamText(DWORD dwDsID)
{
	CString strRet = INVALID_DATASTREAM_VALUE_STR;

	WaitForSingleObject(theMutex,INFINITE);
	if (m_mapDataStreamValue.find(dwDsID) != m_mapDataStreamValue.end())
	{
		strRet = m_mapDataStreamValue[dwDsID].strText;
	}
	ReleaseMutex(theMutex);

	return strRet;
}
//
map<DWORD,TagDtcInfo> COBDPNDCommunication::fGetDtcList()
{
	map<DWORD,TagDtcInfo> mapDtcList;

	WaitForSingleObject(theMutex,INFINITE);
	mapDtcList = m_mapDtcList;
	ReleaseMutex(theMutex);
		
	return mapDtcList;
}

void COBDPNDCommunication::fCheckOverTime()
{
	WaitForSingleObject(theMutex,INFINITE);
	// 超时X秒未收到数据，显示为无效数据
	for (map<DWORD,TagDataStreamValue>::iterator it = m_mapDataStreamValue.begin();it!=m_mapDataStreamValue.end();it++)
	{
		if ((it->second.dwLastGetDataTime != 0) && (GetTickCount() - it->second.dwLastGetDataTime)>DATASTREAM_GET_OVERTIME)
		{
			it->second.fValue = INVALID_DATASTREAM_VALUE;
			it->second.strValue = INVALID_DATASTREAM_VALUE_STR;
		}
	}
	ReleaseMutex(theMutex);
}
void COBDPNDCommunication::fCaculateData(BYTE * pBufData)
{
	WaitForSingleObject(theMutex,INFINITE);

	//命令ID
	DWORD dwCmdID = pBufData[7]*0x100 + pBufData[8];
	switch (dwCmdID)			
	{
	case 0x3102:	//0x3102下位机主动向上位机发送：数据文本号（DS_ID）及该数据文本的计算结果
		/*
		DS_ID_1  DS_ID_2  DS_ID_3  DS_ID_4  DsValueType  Len  Value_1 …. Value_N
		数据文本号：DS_ID
		计算结果数据类型: DsValueType 0x00:IEEE-754格式的单精度4字节浮点数   0x01:字符串文本（Unicode的编码格式）
		计算结果数据的长度：Len
		计算结果数据的内容：Value_1 …. Value_N
		例如：数据类型（0x00）：12.39
		55 AA 81 01 5C 00 XX 31 02 FF 01 00 0B 00 04 41 46 66 66
		例如：数据类型（0x01）：开环
		55 AA 81 01 5C 00 XX 31 02 FF 01 00 0B 01 04 BF AA BB B7
		*/
		{

			DWORD	dwDsID = pBufData[9]*0x1000000+pBufData[10]*0x10000+pBufData[11]*0x100+pBufData[12];
			int		iDsValueType = pBufData[13];
			int		iDsValueLen	= pBufData[14];

			//
			switch (iDsValueType)
			{
			case 0:	//IEEE745 浮点数
				{
					DWORD	dwDsValue = pBufData[15]*0x1000000+pBufData[16]*0x10000+pBufData[17]*0x100+pBufData[18];
					float	fDsValue = fIEEE745ToFloat(dwDsValue);

					CString strDsValue = _T("------");			
					if (m_mapDataStreamValue.find(dwDsID) == m_mapDataStreamValue.end())
					{
						TagDataStreamValue tagDsValue;

						CString strText = theMainDlg->fGetBinText(dwDsID);
						int iDecLen = _ttoi(strText.Right(1));

						tagDsValue.strText = strText.Left(strText.Find(_T(" ")));
						tagDsValue.iDecLen = iDecLen;
						tagDsValue.fValue = fDsValue;
						tagDsValue.strValue = strDsValue;

						m_mapDataStreamValue.insert(pair<DWORD,TagDataStreamValue>(dwDsID,tagDsValue));
					}

					//自定义数据
					vector<DWORD> vecUserDataList;
					vecUserDataList.push_back(0x00020001);	//本次行驶油费
					vecUserDataList.push_back(0x00020002);	//本次每公里花费
					vecUserDataList.push_back(0x00020003);	//总行车油费
					vecUserDataList.push_back(0x00020004);	//累计每公里花费
					for (DWORD i=0;i<vecUserDataList.size();i++)
					{
						if (m_mapDataStreamValue.find(vecUserDataList[i]) == m_mapDataStreamValue.end())
						{
							TagDataStreamValue tagDsValue;

							CString strText = theMainDlg->fGetBinText(vecUserDataList[i]);
							int iDecLen = _ttoi(strText.Right(1));

							tagDsValue.strText = strText.Left(strText.Find(_T(" ")));
							tagDsValue.iDecLen = iDecLen;
							tagDsValue.fValue = fDsValue;
							tagDsValue.strValue = strDsValue;

							m_mapDataStreamValue.insert(pair<DWORD,TagDataStreamValue>(vecUserDataList[i],tagDsValue));
						}
					}
					
					//对需要特殊处理的数据进行处理
					
					//修正油耗系数 默认 即时油耗=331，平均油耗=116
					//即时油耗1(动态)/即时油耗1(静态)/即时油耗2(动态)/即时油耗2(静态)/
					if ((dwDsID == 0xFF010001 ||dwDsID == 0xFF010002 || dwDsID == 0xFF010003 || dwDsID == 0xFF010004 ||
						dwDsID == 0xFF010005 ||dwDsID == 0xFF010006) && m_mapDataStreamValue.find(dwDsID) != m_mapDataStreamValue.end() )
					{
						fDsValue = (float)(fDsValue/331.0*theMainDlg->m_pConfigReadWrite->fGetNowFCC());
					}
					//平均油耗(历史)/平均油耗(本次行驶)/本次耗油量/累计耗油量/
					if ((dwDsID == 0xFF010005 ||dwDsID == 0xFF010006 || dwDsID == 0xFF01000E || dwDsID == 0xFF01000F) &&
						m_mapDataStreamValue.find(dwDsID) != m_mapDataStreamValue.end() )
					{
						fDsValue = (float)(fDsValue/116.0*theMainDlg->m_pConfigReadWrite->fGetAvgFCC());
					}

					// 本次行驶里程（Km）
					if (dwDsID == 0xFF010008 && m_mapDataStreamValue.find(0xFF010008) != m_mapDataStreamValue.end())
					{
						m_mapDataStreamValue[0xFF010008].fValue = fDsValue/1000;
						m_mapDataStreamValue[0xFF010008].strValue.Format(_T("%0.1f"),m_mapDataStreamValue[0xFF010008].fValue);
					}
					// 本次耗油量L
					else if (dwDsID == 0xFF01000E && m_mapDataStreamValue.find(0xFF01000E) != m_mapDataStreamValue.end())
					{
						m_mapDataStreamValue[0xFF01000E].fValue = (float)(fDsValue/331.0*theMainDlg->m_pConfigReadWrite->fGetNowFCC());
						m_mapDataStreamValue[0xFF01000E].strValue.Format(_T("%0.1f"),m_mapDataStreamValue[0xFF01000E].fValue);

						// 本次行驶油费(本次耗油量L*油价)
						m_mapDataStreamValue[0x00020001].fValue = (float)(theMainDlg->m_pConfigReadWrite->fGetFuelPrice()*m_mapDataStreamValue[0xFF01000E].fValue) ;
						m_mapDataStreamValue[0x00020001].strValue.Format(_T("%0.1f"),m_mapDataStreamValue[0x00020001].fValue);
						m_mapDataStreamValue[0x00020001].dwLastGetDataTime = m_mapDataStreamValue[0xFF01000E].dwLastGetDataTime;

						// 本次每公里花费(本次耗油量L*油价)/本次里程
						if (m_mapDataStreamValue.find(0xFF010008) != m_mapDataStreamValue.end() &&
							m_mapDataStreamValue[0xFF010008].fValue != 0 && m_mapDataStreamValue[0xFF010008].fValue != INVALID_DATASTREAM_VALUE)
						{
							m_mapDataStreamValue[0x00020002].fValue = (float)(theMainDlg->m_pConfigReadWrite->fGetFuelPrice()*m_mapDataStreamValue[0xFF01000E].fValue/m_mapDataStreamValue[0xFF010008].fValue);
							m_mapDataStreamValue[0x00020002].strValue.Format(_T("%0.1f"),m_mapDataStreamValue[0x00020002].fValue);
						}
						else
						{
							m_mapDataStreamValue[0x00020002].fValue = 0;
							m_mapDataStreamValue[0x00020002].strValue.Format(_T("0"));
						}
						m_mapDataStreamValue[0x00020002].dwLastGetDataTime = m_mapDataStreamValue[0xFF01000E].dwLastGetDataTime;
					} 
					// 累计耗油量L
					else if (dwDsID == 0xFF01000F && m_mapDataStreamValue.find(0xFF01000F) != m_mapDataStreamValue.end()) 
					{
						m_mapDataStreamValue[0xFF01000F].fValue = (float)(fDsValue/331.0*theMainDlg->m_pConfigReadWrite->fGetNowFCC());
						m_mapDataStreamValue[0xFF01000F].strValue.Format(_T("%0.1f"),m_mapDataStreamValue[0xFF01000F].fValue);

						// 总行车油费(总行车油费L*油价)
						m_mapDataStreamValue[0x00020003].fValue = (float)(theMainDlg->m_pConfigReadWrite->fGetFuelPrice()*m_mapDataStreamValue[0xFF01000F].fValue);
						m_mapDataStreamValue[0x00020003].strValue.Format(_T("%0.1f"),m_mapDataStreamValue[0x00020003].fValue);
						m_mapDataStreamValue[0x00020003].dwLastGetDataTime = m_mapDataStreamValue[0xFF01000F].dwLastGetDataTime;

						// 累计每公里花费(总行车油费L*油价)/累计里程
						if (m_mapDataStreamValue.find(0xFF010008) != m_mapDataStreamValue.end() &&
							m_mapDataStreamValue[0xFF01000A].fValue != 0 && m_mapDataStreamValue[0xFF01000A].fValue != INVALID_DATASTREAM_VALUE) 
						{
							m_mapDataStreamValue[0x00020004].fValue = (float)(theMainDlg->m_pConfigReadWrite->fGetFuelPrice()*m_mapDataStreamValue[0xFF01000F].fValue/m_mapDataStreamValue[0xFF01000A].fValue);
							m_mapDataStreamValue[0x00020004].strValue.Format(_T("%0.1f"),m_mapDataStreamValue[0x00020004].fValue);
						}
						else
						{
							m_mapDataStreamValue[0x00020004].fValue = 0;
							m_mapDataStreamValue[0x00020004].strValue.Format(_T("0"));
						}
						m_mapDataStreamValue[0x00020004].dwLastGetDataTime = m_mapDataStreamValue[0xFF01000F].dwLastGetDataTime;
					}
					// 本次行驶时间
					else if (dwDsID == 0xFF010007 && m_mapDataStreamValue.find(0xFF010007) != m_mapDataStreamValue.end()) 
					{
						m_mapDataStreamValue[0xFF010007].fValue = fDsValue;
						DWORD dwMinute = ((DWORD)fDsValue)/60; 
						m_mapDataStreamValue[0xFF010007].strValue.Format(_T("%02d:%02d"),dwMinute/60,dwMinute%60);
					}
					else
					{
						//根据小数位格式化字符数据
						if (fDsValue>999)	//整数位超过3为，默认不显示小数
						{
							strDsValue.Format(_T("%0.0f"),m_mapDataStreamValue[dwDsID].fValue);
						}
						else
						{
							switch (m_mapDataStreamValue[dwDsID].iDecLen)
							{
							case 0:
								strDsValue.Format(_T("%0.0f"),fDsValue);
								break;
							case 1:
								strDsValue.Format(_T("%0.1f"),fDsValue);
								break;
							case 2:
								strDsValue.Format(_T("%0.2f"),fDsValue);
								break;
							case 3:
								strDsValue.Format(_T("%0.3f"),fDsValue);
								break;
							}

						}
						m_mapDataStreamValue[dwDsID].fValue = fDsValue; 
						m_mapDataStreamValue[dwDsID].strValue = strDsValue;

					}

					//根据车速切换面板
					if (!theMainDlg->m_bShowLog && 
						dwDsID == 0x00000D00 && m_mapDataStreamValue.find(0x00000D00) != m_mapDataStreamValue.end()) 
					{
						if ((m_wLastSpeed>0 && m_mapDataStreamValue[0x00000D00].fValue == 0) ||
							(m_wLastSpeed==0 && m_mapDataStreamValue[0x00000D00].fValue > 0))
						{
							if (m_mapDataStreamValue[0x00000D00].fValue > 0)
							{				
								if(theMainDlg->fGetCurrPanelIndex() != OBDPND_PANEL_RACE && theMainDlg->fGetCurrPanelIndex() != OBDPND_PANEL_TOUR  )
								{				
									if (theMainDlg->fGetBeforPanelIndex() == OBDPND_PANEL_RACE)
									{
										theMainDlg->fSetCurrPanelIndex(OBDPND_PANEL_RACE);
									}
									else
									{
										theMainDlg->fSetCurrPanelIndex(OBDPND_PANEL_TOUR);
									}			
								}			
							}
							else
							{
								if(theMainDlg->fGetCurrPanelIndex() != OBDPND_PANEL_IDLE)
								{				
									theMainDlg->fSetCurrPanelIndex(OBDPND_PANEL_IDLE);
								}	
							}
						}	

						m_wLastSpeed = (WORD)m_mapDataStreamValue[0x00000D00].fValue;
					}
				}
				break;
			case 1:	//UNICODE 字符串
				{
					CString strDsValue = _T("");

					BYTE * pBufUnicodeData = new BYTE[iDsValueLen];
					ZeroMemory(pBufUnicodeData,iDsValueLen);
					memcpy(pBufUnicodeData,pBufData+15,iDsValueLen);
					strDsValue = CString((WCHAR*)pBufUnicodeData);
					delete pBufUnicodeData;
					pBufUnicodeData = NULL;

					if (m_mapDataStreamValue.find(dwDsID) == m_mapDataStreamValue.end())
					{
						TagDataStreamValue tagDsValue;

						CString strText = theMainDlg->fGetBinText(dwDsID);
						int iDecLen = _ttoi(strText.Right(1));

						tagDsValue.strText = strText.Left(strText.Find(_T(" ")));
						tagDsValue.iDecLen = iDecLen;

						m_mapDataStreamValue.insert(pair<DWORD,TagDataStreamValue>(dwDsID,tagDsValue));
					}

					m_mapDataStreamValue[dwDsID].fValue = 0;
					if (strDsValue = _T("XXX.X"))
						strDsValue = _T("XX.X");
					m_mapDataStreamValue[dwDsID].strValue = strDsValue;
				}
				break;
			}

			// 更新最后一次获取数据时间
			m_mapDataStreamValue[dwDsID].dwLastGetDataTime = GetTickCount();

		}
		break;
	case 0x3103:	//0x3102下位机主动向上位机发送：电控系统（ECU_ID号）、故障信息文本ID号（DTC_ID号）、故障信息文本ID号对应的文本信息
		/*
		ECU_ID_1 ECU_ID_2 ECU_ID_3 ECU_ID_4 DTC_ID_1 DTC_ID_2 DTC_ID_3  DTC_ID_4  Len1  Len2  Text_1 …. Text_N
		电控系统号：ECU_ID
		ECU_ID_1*0x1000000 +  ECU_ID_2*0x10000 + ECU_ID_3*0x100 +ECU_ID_4
		故障信息文本ID号：DTC_ID
		DTC_ID _1*0x1000000 + DTC_ID _2*0x10000 + DTC_ID _3*0x100 + DTC_ID_4
		故障信息文本的长度：Len1*0x100 + Len2
		故障信息文本的内容：Text_1 …. Text_N
		例如：55 AA 81 01 5C 00 XX 31 03 01 00 00 00 00 00 00 01 2A 00 50 00 30 00 30 00 30 00 31 00 2e 00 C3 71 B9 6C 03 8C 82 82 82 A7 63 36 52 35 75 EF 8D 00 5F EF 8D 0D 00 0A 00 30 00 78 00 00
		电控系统号=01 00 00 00
		故障信息文本ID号=00 00 00 01
		故障信息文本的长度=2A 00
		故障信息文本的内容=00 50 00 30 00 30 00 30 00 31 00 2e 00 C3 71 B9 6C 03 8C 82 82 82 A7 63 36 52 35 75 EF 8D 00 5F EF 8D 0D 00 0A 00 30 00 78 00 00
		*/
		{
			DWORD dwEcuID = pBufData[9]*0x1000000 + pBufData[10]*0x10000 + pBufData[11]*0x100 + pBufData[12];
			DWORD dwDtcID = pBufData[13]*0x1000000 + pBufData[14]*0x10000 + pBufData[15]*0x100 + pBufData[16];

			int iDtcLen = pBufData[17]*0x100 + pBufData[18];

			CString strDtcText = _T("");

			BYTE * pBufDtcText = new BYTE[iDtcLen];
			ZeroMemory(pBufDtcText,iDtcLen);
			memcpy(pBufDtcText,pBufData+19,iDtcLen);
			strDtcText = CString((WCHAR*)pBufDtcText);
			delete pBufDtcText;
			pBufDtcText = NULL;

			if(m_mapDtcList.find(dwDtcID) == m_mapDtcList.end())
			{
				TagDtcInfo tagDtcInfo;
				tagDtcInfo.dwEcuID = dwEcuID;
				tagDtcInfo.dwDtcID = dwDtcID;
				tagDtcInfo.strDtcText = strDtcText;

				m_mapDtcList.insert(pair<DWORD,TagDtcInfo>(dwDtcID,tagDtcInfo));
			}
			
			theMainDlg->fShowDtcInfo();
		}
		break;
	}

	ReleaseMutex(theMutex);
}
//定义串口接收数据函数类型
void CALLBACK COBDPNDCommunication::OnSerialRead(BYTE* buf,DWORD bufLen)
{
	//theMainDlg->SendMessage(WM_SERIES_RECV,WPARAM(buf),bufLen);	


	if(m_pThis==NULL)
		return ;

	//接收数据日志
	if (theMainDlg->m_pConfigReadWrite->fGetSaveErrLogStatus())
	{
		CString strText;
		CString strTemp;
		strText.Format(_T("[R][%d]%d="),GetTickCount(),bufLen);
		for (DWORD i=0;i<bufLen;i++)
		{
			strTemp.Format(_T("%02X,"),buf[i]);
			strText = strText + strTemp;
		}
		theMainDlg->fAddLog(strText,LOG_TYPE_OBD_DATA);
	}

	m_pThis->fCaculateData(buf);//回调函数中调用非静态成员函数

	//BYTE * pBufData = new BYTE[bufLen];
	//memcpy(pBufData,(BYTE*)buf,bufLen);
	//theMainDlg->SendMessage(WM_SERIES_RECV,WPARAM(pRecvBuf),bufLen);	
	//delete pBufData;
	//pBufData = NULL;

	//BYTE *pRecvBuf = NULL; //接收缓冲区
	////得到父对象指针
	////将接收的缓冲区拷贝到pRecvBuf种
	//pRecvBuf = new BYTE[bufLen];
	//CopyMemory(pRecvBuf,buf,bufLen);

	//theMainDlg->PostMessage(WM_SERIES_RECV,WPARAM(pRecvBuf),bufLen);	
	//theMainDlg->OnSerialRecvData(WPARAM(pRecvBuf),bufLen);	

	//BYTE * pFrameData = new BYTE[MAX_CMD_SIZE];
	//for (int i=0;i<bufLen;)
	//{
	//	ZeroMemory(pFrameData,MAX_CMD_SIZE);
	//	DWORD dwFrameLen = pRecvBuf[i+5]*0x100 + pRecvBuf[i+6] + 4 + 1 + 2;
	//	memcpy(pFrameData,pRecvBuf+i,dwFrameLen);

	//	theMainDlg->SendMessage(WM_SERIES_RECV,WPARAM(pFrameData),dwFrameLen);

	//	i+=dwFrameLen;
	//}

	//delete pFrameData;
	//pFrameData = NULL;
	//delete pRecvBuf;
	//pRecvBuf = NULL;
}

// 检测可用串口
int COBDPNDCommunication::fCheckVaildSerialPort()
{
	int iVaildPort = -1;

	CString strText;
	for (int i=2;i<9;i++)
	{
		strText.Format(_T("正在扫描串口:%d..."),i);
		
		theMainDlg->fAddLog(strText);
		if(fStartSeries(i))
		{
			iVaildPort = i;
			break;
		}
	}
	if (iVaildPort == -1)
	{
		theMainDlg->fAddLog(_T("未检测到可用串口！"));		
	}
	return iVaildPort;
}

// 读取串口
BOOL COBDPNDCommunication::fStartSeries(int iPort)
{
	if (iPort<0)
		return FALSE;

	BOOL bRet = FALSE;

	CString strText;
	CString strTemp;
	BYTE * pBufRecv = new BYTE[MAX_CMD_SIZE];
	int iRecvLen = 0;

	if (m_pSeries->OpenPort(theMainDlg->m_hWnd,iPort))
	{			
		iRecvLen = 0; 
		strText.Format(_T("正在读取%d数据..."),iPort);
		theMainDlg->fAddLog(strText);
		ZeroMemory(pBufRecv,MAX_CMD_SIZE);
		iRecvLen = m_pSeries->fRecv(pBufRecv);

		//效验收到的数据
		if (iRecvLen > 0)
		{
			strText.Format(_T("[%08d]%d="),GetTickCount(),iRecvLen);
			for (int j=0;j<iRecvLen;j++)
			{
				strTemp.Format(_T("%02X,"),pBufRecv[j]);
				strText+=strTemp;
			}
			theMainDlg->fAddLog(strText); 

			if (pBufRecv[0] == 0x55 && pBufRecv[1] == 0xAA &&
				pBufRecv[2] == 0x81 && pBufRecv[3] == 0x01)
			{
				//如果效验正确
				theMainDlg->fAddLog(_T("读取数据成功"));
				bRet = TRUE;

				//开启读取线程
				m_pSeries->OpenReadThread();
			}
		}
	}			

	delete pBufRecv;
	pBufRecv = NULL;
	
	return bRet;
}

// 停止读取
void COBDPNDCommunication::fStopSerial()
{
	m_pSeries->ClosePort();
}