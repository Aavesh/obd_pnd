#pragma once

class  __declspec(dllexport) CDecrypt
{
public:
	CDecrypt(void);
	~CDecrypt(void);

	//���ֽ�ѭ����λ
	DWORD ShiftLeft(DWORD dw, BYTE ln);
	//���ݴ�����
	//addr: BIN�ļ�����Ч������ʼ��ַ
	//len : ���ݳ���
	//strlen: �ļ���ָʾ�����ݴ�����
	int EncodeStr(BYTE * str, DWORD id , DWORD addr , WORD len );

	// ģ���ĸ��ֽ����ݼ���
	int fEncrypt_4Byte_To_4Byte(BYTE * Source_Str, BYTE * Target_Str);

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
	//����ֵ��  (������У��λ)
	//************************************************************************************
	BYTE fCalcCheckBit(WORD wCmdID,WORD wLenByte2,BYTE * pbData,WORD wDataLen);
};
