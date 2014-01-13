#include "StdAfx.h"
#include "Decrypt.h"

CDecrypt::CDecrypt(void)
{
}

CDecrypt::~CDecrypt(void)
{
}

//四字节循环移位
DWORD CDecrypt::ShiftLeft(DWORD dw, BYTE ln)
{
	return (dw<<ln)|(dw>>(32-ln));
}

//数据串解密
//addr: BIN文件中有效数据起始地址
//len : 数据长度
//strlen: 文件中指示的数据串长度
int CDecrypt::EncodeStr(BYTE * str, DWORD id , DWORD addr , WORD len )
{
	WORD i,j,ln,keyln;
	BYTE s[20],b,pos;
	BYTE key[] = {"RD:V-checker"};
	//WORD w;
	//DWORD *pd;
	//BYTE *pb;
	DWORD dw;


	//ID中有效字节与密钥相加
	ln = 0;
	for (i=0;i<4;i++){
		b = (id>>(i*8))&0x00FF;
		if (b>0) s[ln++]=b;
	}
	if (ln>0){
		pos = 0;
		for (i=0;;i++){
			if (key[i] == 0) break;
			key[i] = key[i] + s[pos++];
			if (pos >= ln) pos = 0;
		}
	}

	//DisplayBuffer(0,0,32,key,6);
	//地址中'1'的个数作为因子, 密钥四字节一组循环左移
	ln = 0;
	for (i=0;i<32;i++)
		if ((addr>>i)&0x00000001) ln++;
	for(i=0;i<3;i++){
		dw = 0;
		for(j=0;j<4;j++)
			dw = (dw << 8) + key[i*4+j];
		dw = ShiftLeft(dw,ln);
		for(j=0;j<4;j++) key[i*4+j] = (dw>>((3-j)*8))&0x00FF;
	}

	//长度与8取余加4
	keyln = (len % 8) + 4;

	pos = 0;
	for(i=0;i<len;i++){
		*str = (*str)^key[pos++];
		if (pos>=keyln) pos=0;
		str++;
	}

	return 0;
}

// 四个字节数据加密
int CDecrypt::fEncrypt_4Byte_To_4Byte(BYTE * Source_Str, BYTE * Target_Str)
{
	DWORD Source_A,Target_A;

	Source_A = Source_Str[3]*0x1000000 + Source_Str[1]*0x10000 + Source_Str[0]*0x100 + Source_Str[2];
	Source_A = (Source_A+0x26596413)^0x65978451;
	Target_A =	((Source_A&0x20000000)>>19) | ((Source_A&0x00000400)<<19) |
		((Source_A&0x04000000)>>22) | ((Source_A&0x00000010)<<22) |
		((Source_A&0x01000000)>>2) | ((Source_A&0x00400000)<<2) |
		((Source_A&0x00100000)>>1) | ((Source_A&0x00080000)<<1) |
		((Source_A&0x00020000)>>6) | ((Source_A&0x00000800)<<6) |
		((Source_A&0x00000200)>>8) | ((Source_A&0x00000020)<<8) |
		((Source_A&0x00040000)>>6) | ((Source_A&0x00001000)<<6) |
		((Source_A&0x00010000)>>14) | ((Source_A&0x00000004)<<14) |
		((Source_A&0x00004000)>>8) | ((Source_A&0x00000040)<<8);
	Target_A = Target_A | (Source_A & 0xdaa0a1a9);
	Target_A = (Target_A+0x24676324)^0x56732165;

	Target_Str[0] = (Target_A&0xff000000) >> 24;
	Target_Str[1] = (Target_A&0x00ff0000) >> 16;
	Target_Str[2] = (Target_A&0x0000ff00) >> 8;
	Target_Str[3] = (Target_A&0x000000ff);

	return 0;
}


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
//返回值：  (计算后的校验位
//************************************************************************************
BYTE CDecrypt::fCalcCheckBit(WORD wCmdID,WORD wLenByte2,BYTE * pbData,WORD wDataLen)
{	
	BYTE FrameCs = 0,FrameCs_CSWH = 0,FrameCs_CSWL = 0;
	WORD FrameCs_CSW = 0,FrameCs_CSW2 = 0,FrameCs_LN = 0,FrameCs_CSW3 = 0,Frame_CommandID  = 0;

	for (size_t i=0;i<wDataLen;i++)
	{
		FrameCs_CSW = FrameCs_CSW + pbData[i];	
	}

	//计算校验数据，判断是否正确
	Frame_CommandID = wCmdID;		//RecvFrame->FrameData.Command_ID1*0x100 + RecvFrame->FrameData.Command_ID2;
	FrameCs_CSW2 = FrameCs_CSW^Frame_CommandID;
	FrameCs_LN = wLenByte2%0x10;	//RecvFrame->FrameData.Lenth_Byte2
	//需要移动的数据a,长度N(8 16 32),移动的位数n
	//循环左移:(a<<n)|(a>>(N-n))
	//循环右移:(a>>n)|(a<<(N-n))
	FrameCs_CSW3 = (FrameCs_CSW2<<FrameCs_LN) | (FrameCs_CSW2>>(16-FrameCs_LN));
	FrameCs_CSWH = (FrameCs_CSW3&0xff00)>>8;
	FrameCs_CSWL = (FrameCs_CSW3&0x00ff);
	FrameCs = FrameCs_CSWH + FrameCs_CSWL;	

	return FrameCs;
}
