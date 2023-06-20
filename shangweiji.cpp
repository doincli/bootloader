#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <conio.h>
#include <tchar.h>
#include <time.h>


#define COM  "COM8"					//配置实际的485串口端口
#define sleep_time 150				//每一帧发送的间隔时间
#define Baud 19200					//波特率设置
#define polynomial 0x8005;			//CRC16校验的多项式
typedef unsigned char u8;			//8bits
typedef unsigned short u16;			//16bits
HANDLE hCom;						//定义串口句柄

int parse_hex();							//解析hex文件并转换成协议的格式输出
void addr_parsing(unsigned char* p);		//将字符串地址读入成16进制并除以2再转化成字符串格式输出
void CharTypedata_parsing(unsigned char* p, unsigned char num);   //将字符转换成10进制数据。
int Write_Data(unsigned char* p, unsigned char num);			  //将数据写入串口			
u8 reverse_8(u8 k);												  // used to reverse the 8-bits data
u16 reverse_16(u16 k);											  // used to reverse the 16-bits data
u16 crc16_test(u8* addr, u8 num);								  // CRC16校验计算




int main()
{
	hCom = CreateFile(_T(COM),//COM口
		GENERIC_READ | GENERIC_WRITE,//允许读和写
		0,//独占方式
		NULL,
		OPEN_EXISTING,//打开而不是创建
		0,			  //同步方式
//      FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, //异步方式
NULL);
	if (hCom == (HANDLE)-1)
	{
		printf("打开COM失败!\n");
		return FALSE;
	}
	else
	{
		printf("COM打开成功！\n");
	}
	SetupComm(hCom, 20480, 20480);//输入缓冲区和输出缓冲区的大小都是1024

	COMMTIMEOUTS TimeOuts;//设定读超时    
	TimeOuts.ReadIntervalTimeout = 100;
	TimeOuts.ReadTotalTimeoutMultiplier = 100;
	TimeOuts.ReadTotalTimeoutConstant = 5000;
	TimeOuts.WriteTotalTimeoutMultiplier = 500;//设定写超时
	TimeOuts.WriteTotalTimeoutConstant = 2000;
	SetCommTimeouts(hCom, &TimeOuts);//设置超时

	DCB dcb1;
	GetCommState(hCom, &dcb1);
	dcb1.BaudRate = Baud;//波特率为9600
	dcb1.ByteSize = 8;//每个字节有8位
	dcb1.Parity = NOPARITY;//无奇偶校验位
	dcb1.StopBits = 1;//1个停止位
	dcb1.fParity = FALSE;
	dcb1.fNull = FALSE;
	SetCommState(hCom, &dcb1);

	PurgeComm(hCom, PURGE_TXCLEAR | PURGE_RXCLEAR);//清空缓冲区

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

	unsigned char jump_to_app[4] = { 0x1F , 0x1F, 0x48,0x48 };		//1F1F的校验值是4848；
	unsigned char restart[4] = { 0x00 , 0x52, 0x80, 0x4d };			//0052的校验值是4D80；

	u16 crc_word = 0;												//定义16位的1个数组，并且将一个8位指针指向它。由于是小端模式，低位存在低地址处，高位存高地址处。
	u8* crc_byte = (u8 *)&crc_word;									//定义一个char类型的指针强制指向int类型指针。

	FILE* pt_read;													//定义文件指针，指向需要解析的hex文件

	char ch = 0;
	unsigned char k = 0;

	div_t analyze;													//temp.quot:商     temp.rem：余数


//	char filename_read[] = "C://Users//wysji//MPLABXProjects//APP.X//dist//default//production//APP.X.production.hex";

	//char filename_read[] = "C://Users//doinc//Desktop//Micro_HW//APP.X//dist//default//production//APP.X.production.hex";
	char filename_read[] = "E://pic_code//fengmingqi.X//dist//default//production//fengmingqi.X.production.hex";
	errno_t err_open;

	if ((err_open = fopen_s(&pt_read, filename_read, "r")) != NULL) //可以参考fopen_s函数的形式
	{
		perror("The following error occurred");
		return -2;
	}

	while (ch != EOF)												//持续读取hex文件中的数据，直到读取到结束符标志 EOF。
	{

		while ((ch != '\n') && (ch != EOF))							// \n 是换行符的字符串，一行一行读取
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
			array_chardata[3] = '1';					//先输出0041，也就是协议格式中要求的

			for (int i = 0; i < 4; i++)
			{
				array_chardata[4 + i] = array[2 + i];
			}
			array_chardata[8] = array[0];				// 8 9位存放数据个数
			array_chardata[9] = array[1];

			for (int i = 0; i < k - 11; i++)
			{
				array_chardata[10 + i] = array[8 + i];

			}
			analyze = div(k - 1, 2);
			if (analyze.rem != 0)							//hex文件一行的字符个数应该为偶数，检验一下
			{
				printf("There is some error in reading the hex file since the number of data of one row is not even.\n");
				fclose(pt_read);
				return -4;
			}

			CharTypedata_parsing(array_chardata, k - 1);	//将字符全部转换成十进制。 可以改进，只需要把最开头的0041，写成十进制存放，这样就不需要这个函数转换了。

			for (int i = 0; i < (k - 1) / 2; i++)			//将两个字节组合起来，组成一个字。因为串口输出时，一次输出一个字
			{
				Data_hex[i] = array_chardata[2 * i] * 16 + array_chardata[2 * i + 1];
			}

			crc_word = crc16_test(Data_hex, (k - 1) / 2);	//算出校验码


			if (Write_Data(Data_hex, (k - 1) / 2) != 1)		//如果写入串口时有错误，关闭串口并返回
			{
				fclose(pt_read);
				return FALSE;
			}
			else
			{
				Write_Data(crc_byte, 2);					//在结尾加上校验码
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

	div_t temp;								//temp.quot:商     temp.rem：余数
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
