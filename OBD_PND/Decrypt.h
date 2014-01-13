#pragma once

class  __declspec(dllexport) CDecrypt
{
public:
	CDecrypt(void);
	~CDecrypt(void);

	//四字节循环移位
	DWORD ShiftLeft(DWORD dw, BYTE ln);
	//数据串解密
	//addr: BIN文件中有效数据起始地址
	//len : 数据长度
	//strlen: 文件中指示的数据串长度
	int EncodeStr(BYTE * str, DWORD id , DWORD addr , WORD len );

	// 模块四个字节数据加密
	int fEncrypt_4Byte_To_4Byte(BYTE * Source_Str, BYTE * Target_Str);

	//************************************************************************************
	//函数介绍：根据命令ID，长度低字节以及数据帧计算效验位数据
	//数据校验（Check）的方式步骤如下：
	//1.	将数据帧内容（Data Bytes）的所有数据相加，舍弃高位，得到一个字CSW（2个BYTE）。 
	//2.	将CSW与数据帧命令字（Command ID）进行异或，得到CSW2。
	//3.	数据帧长度低字节（Lenth Byte2）与0x10取余的结果记作LN，将CSW2循环左移LN位得到CSW3。
	//4.	CSW3的高字节记作CSWH，低字节记作CSWL，CSWH与CSWL相加并舍弃高位得CS
	//入口参数：wCmdID:命令ID
	//			LenByte2:数据帧长度低字节
	//			bData:数据帧
	//出口参数：(无)
	//返回值：  (计算后的校验位)
	//************************************************************************************
	BYTE fCalcCheckBit(WORD wCmdID,WORD wLenByte2,BYTE * pbData,WORD wDataLen);
};
