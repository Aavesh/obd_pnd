#include "StdAfx.h"
#include "Decrypt.h"

CDecrypt::CDecrypt(void)
{
}

CDecrypt::~CDecrypt(void)
{
}

//���ֽ�ѭ����λ
DWORD CDecrypt::ShiftLeft(DWORD dw, BYTE ln)
{
	return (dw<<ln)|(dw>>(32-ln));
}

//���ݴ�����
//addr: BIN�ļ�����Ч������ʼ��ַ
//len : ���ݳ���
//strlen: �ļ���ָʾ�����ݴ�����
int CDecrypt::EncodeStr(BYTE * str, DWORD id , DWORD addr , WORD len )
{
	WORD i,j,ln,keyln;
	BYTE s[20],b,pos;
	BYTE key[] = {"RD:V-checker"};
	//WORD w;
	//DWORD *pd;
	//BYTE *pb;
	DWORD dw;


	//ID����Ч�ֽ�����Կ���
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
	//��ַ��'1'�ĸ�����Ϊ����, ��Կ���ֽ�һ��ѭ������
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

	//������8ȡ���4
	keyln = (len % 8) + 4;

	pos = 0;
	for(i=0;i<len;i++){
		*str = (*str)^key[pos++];
		if (pos>=keyln) pos=0;
		str++;
	}

	return 0;
}

// �ĸ��ֽ����ݼ���
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
//�������ܣ���������ID�����ȵ��ֽ��Լ�����֡����Ч��λ����
//����У�飨Check���ķ�ʽ�������£�
//1.	������֡���ݣ�Data Bytes��������������ӣ�������λ���õ�һ����CSW��2��BYTE���� 
//2.	��CSW������֡�����֣�Command ID��������򣬵õ�CSW2��
//3.	����֡���ȵ��ֽڣ�Lenth Byte2����0x10ȡ��Ľ������LN����CSW2ѭ������LNλ�õ�CSW3��
//4.	CSW3�ĸ��ֽڼ���CSWH�����ֽڼ���CSWL��CSWH��CSWL��Ӳ�������λ��CS
//��ڲ�����wCmdID:����ID
//			LenByte2:����֡���ȵ��ֽ�
//			bData:����֡
//���ڲ�����(��)
//����ֵ��  (������У��λ
//************************************************************************************
BYTE CDecrypt::fCalcCheckBit(WORD wCmdID,WORD wLenByte2,BYTE * pbData,WORD wDataLen)
{	
	BYTE FrameCs = 0,FrameCs_CSWH = 0,FrameCs_CSWL = 0;
	WORD FrameCs_CSW = 0,FrameCs_CSW2 = 0,FrameCs_LN = 0,FrameCs_CSW3 = 0,Frame_CommandID  = 0;

	for (size_t i=0;i<wDataLen;i++)
	{
		FrameCs_CSW = FrameCs_CSW + pbData[i];	
	}

	//����У�����ݣ��ж��Ƿ���ȷ
	Frame_CommandID = wCmdID;		//RecvFrame->FrameData.Command_ID1*0x100 + RecvFrame->FrameData.Command_ID2;
	FrameCs_CSW2 = FrameCs_CSW^Frame_CommandID;
	FrameCs_LN = wLenByte2%0x10;	//RecvFrame->FrameData.Lenth_Byte2
	//��Ҫ�ƶ�������a,����N(8 16 32),�ƶ���λ��n
	//ѭ������:(a<<n)|(a>>(N-n))
	//ѭ������:(a>>n)|(a<<(N-n))
	FrameCs_CSW3 = (FrameCs_CSW2<<FrameCs_LN) | (FrameCs_CSW2>>(16-FrameCs_LN));
	FrameCs_CSWH = (FrameCs_CSW3&0xff00)>>8;
	FrameCs_CSWL = (FrameCs_CSW3&0x00ff);
	FrameCs = FrameCs_CSWH + FrameCs_CSWL;	

	return FrameCs;
}
