#include "mcc_generated_files/mcc.h"
#define polynomial 0x8005
#include "variables.h"


void main(void)
{
    SYSTEM_Initialize();
    TMR0_SetInterruptHandler(TMR0_Int);    //TMR0 interrupt function
    EUSART1_SetRxInterruptHandler(EUSART_receive);//EUSART interrupt function
    
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();
    TMR0_StopTimer();
    IO_RA0_SetLow();
    IO_RA1_SetLow();
    IO_RC7_SetLow();                        // enable receiver
    i = 0;
    WDTCON0bits.SEN = 0;                    // disable watchdog;
    flag_erase = 0;
    while (1)                               
    {   

        IO_RA0_Toggle();
        IO_RA1_Toggle();
        DELAY_milliseconds(200);
    }
   
    
}

void EUSART_receive(void)
{
                TMR0_StopTimer();
                frame[i++] = RC1REG;
                TMR0_Reload(0xE9);
                TMR0_StartTimer(); 
}


void TMR0_Int(void)
{
    uint8_t num = 0;        //define the data length in byte
    num = frame[4]/2;
    uint16_t addr;
    i = 0;
    uint16_t array[8] ;
    uint16_t CRC;           //CRC check
    
    uint16_t crc_word = 0;	// CRC check contains two bytes.						
	uint8_t *crc_byte = (uint8_t *)&crc_word;
    
    
    TMR0_Reload(0xE9);
    TMR0_StopTimer();
    

    if(frame[0] == 0x00 )
    {
           
            if(frame[1] == 0x41)
            {
                CRC = ((frame[6+num*2]|0x0000)<<8)| frame[5+num*2];
                crc_word = crc_test(frame, 2*num + 5);

                if( crc_word == CRC )
                {
                    addr = ((frame[2]|0x0000)<<8)|frame[3];
                
                    for(int j = 0; j< num; j++)
                    {
                        array[j] = ((frame[6+2*j]|0x0000)<<8)| frame[5+2*j];
                    }
                
                    for(int j = 0; j<num ; j++ )
                    {
                        FLASH_Write_a_word(addr++, array[j]);         //write data to designated address
                    }
                }
            }
            else if( frame[1] == 0x52)                               //0052, restart.
            {
                crc_word = ((frame[3]|0x0000)<<8)| frame[2];
                if( (crc_word) == crc_test(frame, 2))
                {
                    WDTCON0bits.SEN = 1;                           //enable watchdog
                    INTERRUPT_GlobalInterruptDisable();
                    INTERRUPT_PeripheralInterruptDisable();
                    while(1);
                }
            }
        
            else if(frame[1] == 0x54)                               //0054, erase.
            {
                addr = ((frame[3]|0x0000)<<8)|frame[2];
                if(addr >= 0x1000)
                    {
                        flag_erase = 1;
                        FLASH_EraseBlock(addr); 
                    }
                
            }
    }
    
    if(frame[0] == 0x1F && frame[1] ==0x1F)                         //1F1F, goto 0x1000
        {
            crc_word = ((frame[3]|0x0000)<<8)| frame[2];
            if( crc_word == crc_test(frame, 2))
            {
                if(FLASH_ReadWord(0x1000)  != 0x3FFF)
                {
                    asm("MOVLP 0x10");
                    asm("GOTO 0x00");
                }
            }
        }
    

}



void FLASH_Write_a_word(uint16_t writeAddr, uint16_t flashWordArray)        // write a word in a certain address
{
//    uint16_t    blockStartAddr  = (uint16_t )(writeAddr & ((END_FLASH-1) ^ (ERASE_FLASH_BLOCKSIZE-1)));
    uint8_t     GIEBitValue = INTCONbits.GIE;   // Save interrupt enable
//    uint8_t i;
// Flash write must start at the beginning of a row     //neglect the determination of the blockstart addr
//    if( writeAddr != blockStartAddr )
//    {
//        return -1;
//    }
    INTCONbits.GIE = 0;         // Disable interrupts
 //   Block erase sequence
 //   FLASH_EraseBlock(writeAddr);
    // Block write sequence
    NVMCON1bits.NVMREGS = 0;    // Deselect Configuration space
    NVMCON1bits.WREN = 1;    // Enable wrties
    NVMCON1bits.LWLO = 1;    // Only load write latches
        // Load lower 8 bits of write address
        NVMADRL = (writeAddr & 0xFF);
        // Load upper 6 bits of write address
        NVMADRH = ((writeAddr & 0xFF00) >> 8);
	// Load data in current address
        NVMDATL = flashWordArray;
        NVMDATH = ((flashWordArray & 0xFF00) >> 8);
            // Start Flash program memory write
            NVMCON1bits.LWLO = 0;
        NVMCON2 = 0x55;
        NVMCON2 = 0xAA;
        NVMCON1bits.WR = 1;
        NOP();
        NOP();
    NVMCON1bits.WREN = 0;       // Disable writes
    INTCONbits.GIE = GIEBitValue;   // Restore interrupt enable
  //  return 0;
}

uint8_t reverse_8(uint8_t k)
{
//    int i = 0;
	uint8_t temp = 0;

	for (int i = 0; i < 8; i++)
        {
            temp |= (k >> i & 0x01) << (7 - i);
        }

	return temp;


}

uint16_t reverse_16(uint16_t k)
{
//	int i = 0;
	uint16_t temp = 0;

	for (int i = 0; i < 15; i++)
	{
		temp |= (k >> i & 0x0001) << (15 - i);
	}

	return temp;
}

uint16_t crc_test(uint8_t *addr, int num)
{
	uint8_t data;
    uint16_t Data = 0x0000;
	uint16_t crc_reg = 0xFFFF;

	for (; num > 0; num--)
	{
		data = *addr++;
		data = reverse_8(data);                     //
        Data = 0x0000;
        Data = (Data | data) <<8;
		crc_reg = crc_reg ^ Data;			//
		for (int i = 0; i < 8; i++)					//cylce
		{
			if (crc_reg & 0x8000)					//if the highest bit is 1, then left shift and xor with poly
			{
				crc_reg = (crc_reg << 1) ^ polynomial;
			}
			else                                    //left shift directly
			{			
			crc_reg <<= 1;
			}
		}
	}

	crc_reg = reverse_16(crc_reg);
	crc_reg = crc_reg ^ 0x0000;	                        //xor with 0x0000
	return crc_reg;


}