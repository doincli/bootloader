#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <conio.h>
#include <tchar.h>
#include <time.h>


#define COM  "COM8"					//����ʵ�ʵ�485���ڶ˿�
#define sleep_time 150				//ÿһ֡���͵ļ��ʱ��
#define Baud 19200					//����������
#define polynomial 0x8005;			//CRC16У��Ķ���ʽ
typedef unsigned char u8;			//8bits
typedef unsigned short u16;			//16bits
HANDLE hCom;						//���崮�ھ��

int parse_hex();							//����hex�ļ���ת����Э��ĸ�ʽ���
void addr_parsing(unsigned char* p);		//���ַ�����ַ�����16���Ʋ�����2��ת�����ַ�����ʽ���
void CharTypedata_parsing(unsigned char* p, unsigned char num);   //���ַ�ת����10�������ݡ�
int Write_Data(unsigned char* p, unsigned char num);			  //������д�봮��			
u8 reverse_8(u8 k);												  // used to reverse the 8-bits data
u16 reverse_16(u16 k);											  // used to reverse the 16-bits data
u16 crc16_test(u8* addr, u8 num);								  // CRC16У�����




int main()
{
	hCom = CreateFile(_T(COM),//COM��
		GENERIC_READ | GENERIC_WRITE,//�������д
		0,//��ռ��ʽ
		NULL,
		OPEN_EXISTING,//�򿪶����Ǵ���
		0,			  //ͬ����ʽ
//      FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, //�첽��ʽ
NULL);
	if (hCom == (HANDLE)-1)
	{
		printf("��COMʧ��!\n");
		return FALSE;
	}
	else
	{
		printf("COM�򿪳ɹ���\n");
	}
	SetupComm(hCom, 20480, 20480);//���뻺����������������Ĵ�С����1024

	COMMTIMEOUTS TimeOuts;//�趨����ʱ    
	TimeOuts.ReadIntervalTimeout = 100;
	TimeOuts.ReadTotalTimeoutMultiplier = 100;
	TimeOuts.ReadTotalTimeoutConstant = 5000;
	TimeOuts.WriteTotalTimeoutMultiplier = 500;//�趨д��ʱ
	TimeOuts.WriteTotalTimeoutConstant = 2000;
	SetCommTimeouts(hCom, &TimeOuts);//���ó�ʱ

	DCB dcb1;
	GetCommState(hCom, &dcb1);
	dcb1.BaudRate = Baud;//������Ϊ9600
	dcb1.ByteSize = 8;//ÿ���ֽ���8λ
	dcb1.Parity = NOPARITY;//����żУ��λ
	dcb1.StopBits = 1;//1��ֹͣλ
	dcb1.fParity = FALSE;
	dcb1.fNull = FALSE;
	SetCommState(hCom, &dcb1);

	PurgeComm(hCom, PURGE_TXCLEAR | PURGE_RXCLEAR);//��ջ�����

	if (parse_hex() != 1)
	{
		printf("There is some error occurred");
	}


	return -99;
}



int parse_hex()						//updated version: directly parsing the hex file and then transmit to C_usart
{

	unsigned char array[64] = { 0 };
	unsigned char array_chardata[64] = { 0 };
	unsigned char Data_hex[64] = { 0 };
	unsigned char* p = &array[2];

	unsigned char jump_to_app[4] = { 0x1F , 0x1F, 0x48,0x48 };		//1F1F��У��ֵ��4848��
	unsigned char restart[4] = { 0x00 , 0x52, 0x80, 0x4d };			//0052��У��ֵ��4D80��

	u16 crc_word = 0;												//����16λ��1�����飬���ҽ�һ��8λָ��ָ������������С��ģʽ����λ���ڵ͵�ַ������λ��ߵ�ַ����
	u8* crc_byte = (u8 *)&crc_word;									//����һ��char���͵�ָ��ǿ��ָ��int����ָ�롣

	FILE* pt_read;													//�����ļ�ָ�룬ָ����Ҫ������hex�ļ�

	char ch = 0;
	unsigned char k = 0;

	div_t analyze;													//temp.quot:��     temp.rem������


//	char filename_read[] = "C://Users//wysji//MPLABXProjects//APP.X//dist//default//production//APP.X.production.hex";

	//char filename_read[] = "C://Users//doinc//Desktop//Micro_HW//APP.X//dist//default//production//APP.X.production.hex";
	char filename_read[] = "E://pic_code//fengmingqi.X//dist//default//production//fengmingqi.X.production.hex";
	errno_t err_open;

	if ((err_open = fopen_s(&pt_read, filename_read, "r")) != NULL) //���Բο�fopen_s��������ʽ
	{
		perror("The following error occurred");
		return -2;
	}

	while (ch != EOF)												//������ȡhex�ļ��е����ݣ�ֱ����ȡ����������־ EOF��
	{

		while ((ch != '\n') && (ch != EOF))							// \n �ǻ��з����ַ�����һ��һ�ж�ȡ
		{
			ch = fgetc(pt_read);
			if (ch != ':')
			{
				array[k] = ch;					// all bytes in this row has been completely read into array[].
				k++;							// k is the numer of Bytes including '\n'. So k-1 is the real number.

			}
		}

		if (array[6] == '0' && array[7] == '0')	//ignore when the function code is not 00;
		{

			addr_parsing(&array[2]);
			array_chardata[0] = '0';
			array_chardata[1] = '0';
			array_chardata[2] = '4';
			array_chardata[3] = '1';					//�����0041��Ҳ����Э���ʽ��Ҫ���

			for (int i = 0; i < 4; i++)
			{
				array_chardata[4 + i] = array[2 + i];
			}
			array_chardata[8] = array[0];				// 8 9λ������ݸ���
			array_chardata[9] = array[1];

			for (int i = 0; i < k - 11; i++)
			{
				array_chardata[10 + i] = array[8 + i];

			}
			analyze = div(k - 1, 2);
			if (analyze.rem != 0)							//hex�ļ�һ�е��ַ�����Ӧ��Ϊż��������һ��
			{
				printf("There is some error in reading the hex file since the number of data of one row is not even.\n");
				fclose(pt_read);
				return -4;
			}

			CharTypedata_parsing(array_chardata, k - 1);	//���ַ�ȫ��ת����ʮ���ơ� ���ԸĽ���ֻ��Ҫ���ͷ��0041��д��ʮ���ƴ�ţ������Ͳ���Ҫ�������ת���ˡ�

			for (int i = 0; i < (k - 1) / 2; i++)			//�������ֽ�������������һ���֡���Ϊ�������ʱ��һ�����һ����
			{
				Data_hex[i] = array_chardata[2 * i] * 16 + array_chardata[2 * i + 1];
			}

			crc_word = crc16_test(Data_hex, (k - 1) / 2);	//���У����


			if (Write_Data(Data_hex, (k - 1) / 2) != 1)		//���д�봮��ʱ�д��󣬹رմ��ڲ�����
			{
				fclose(pt_read);
				return FALSE;
			}
			else
			{
				Write_Data(crc_byte, 2);					//�ڽ�β����У����
			}


			Sleep(sleep_time);

			ch = 0;
			k = 0;
		}
		else
		{
			ch = EOF;
		}



	}


//	Write_Data(restart, 4);				// 0052  - restart bootloader app
	Write_Data(jump_to_app, 4);			// 1F1F  - Jump to APP

	fclose(pt_read);
	CloseHandle(hCom);

	return 1;

}

void addr_parsing(unsigned char* p)					//addr /2
{

	for (int i = 0; i < 4; i++)
	{
		if (p[i] <= '9' && p[i] >= '0')
		{
			p[i] = p[i] - 48;
		}

		else if (p[i] <= 'F' && p[i] >= 'A')
		{
			p[i] = p[i] - 55;
		}
	}



	int s = (p[0] * (16 * 16 * 16) + p[1] * (16 * 16) + p[2] * 16 + p[3]) / 2;

	div_t temp;								//temp.quot:��     temp.rem������
	temp.quot = s;

	for (int i = 3; temp.quot >= 16;)
	{
		temp = div(temp.quot, 16);
		p[i--] = temp.rem;
		if (temp.quot < 16)
		{
			p[0] = temp.quot;
		}
	}

	//	p[0] = temp.quot;
	for (int i = 0; i < 4; i++)
	{
		if (p[i] <= 9 && p[i] >= 0)
		{
			p[i] = p[i] + 48;
		}

		else if (p[i] <= 15 && p[i] >= 10)
		{
			p[i] = p[i] + 55;
		}
	}
}

void CharTypedata_parsing(unsigned char* p, unsigned char num)				// char type data exchanged to decimal.
{

	for (int i = 0; i < num; i++)
	{
		if (p[i] <= '9' && p[i] >= '0')
		{
			p[i] = p[i] - 48;
		}

		else if (p[i] <= 'F' && p[i] >= 'A')
		{
			p[i] = p[i] - 55;
		}
	}

}

int Write_Data(unsigned char* p, unsigned char num)
{

	DWORD x = 1;

	for (int i = 0; i < num; i++)
	{
		WriteFile(hCom, p++, x, &x, NULL);

	}
	PurgeComm(hCom, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);

	return TRUE;
}



u8 reverse_8(u8 k)
{
	int i = 0;
	u8 temp = 0;

	for (i; i < 8; i++)
	{
		temp |= (k >> i & 0x01) << (7 - i);
	}

	return temp;
}

u16 reverse_16(u16 k)
{
	int i = 0;
	u16 temp = 0;

	for (i; i < 15; i++)
	{
		temp |= (k >> i & 0x0001) << (15 - i);
	}

	return temp;
}


u16 crc16_test(u8* addr, u8 num)
{
	u8 data;

	u16 crc_reg = 0xFFFF;
	for (; num > 0; num--)
	{
		data = *(addr++);
		data = reverse_8(data);

		crc_reg ^= (data << 8);

		for (int i = 0; i < 8; i++)
		{
			if (crc_reg & 0x8000)
			{
				crc_reg = (crc_reg << 1) ^ polynomial;
			}
			else
				crc_reg <<= 1;
		}
	}

	crc_reg = reverse_16(crc_reg);
	crc_reg ^= 0x0000;
	return crc_reg;

}
