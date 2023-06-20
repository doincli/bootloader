/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.7
        Device            :  PIC16F18446
        Driver Version    :  2.00
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#include "mcc_generated_files/mcc.h"
#include "bootloader.h"
/*
                         Main application
 */
void tmr0();
//void fengming();
//void FLASH_Write_a_word(uint16_t writeAddr, uint16_t flashWordArray)        // write a word in a certain address
//{
////    uint16_t    blockStartAddr  = (uint16_t )(writeAddr & ((END_FLASH-1) ^ (ERASE_FLASH_BLOCKSIZE-1)));
//    uint8_t     GIEBitValue = INTCONbits.GIE;   // Save interrupt enable
////    uint8_t i;
//// Flash write must start at the beginning of a row     //neglect the determination of the blockstart addr
////    if( writeAddr != blockStartAddr )
////    {
////        return -1;
////    }
//    INTCONbits.GIE = 0;         // Disable interrupts
// //   Block erase sequence
// //   FLASH_EraseBlock(writeAddr);
//    // Block write sequence
//    NVMCON1bits.NVMREGS = 0;    // Deselect Configuration space
//    NVMCON1bits.WREN = 1;    // Enable wrties
//    NVMCON1bits.LWLO = 1;    // Only load write latches
//        // Load lower 8 bits of write address
//        NVMADRL = (writeAddr & 0xFF);
//        // Load upper 6 bits of write address
//        NVMADRH = ((writeAddr & 0xFF00) >> 8);
//	// Load data in current address
//        NVMDATL = flashWordArray;
//        NVMDATH = ((flashWordArray & 0xFF00) >> 8);
//            // Start Flash program memory write
//            NVMCON1bits.LWLO = 0;
//        NVMCON2 = 0x55;
//        NVMCON2 = 0xAA;
//        NVMCON1bits.WR = 1;
//        NOP();
//        NOP();
//    NVMCON1bits.WREN = 0;       // Disable writes
//    INTCONbits.GIE = GIEBitValue;   // Restore interrupt enable
//  //  return 0;
//}

void main(void)
{
    
    // initialize the device
    SYSTEM_Initialize();
    TMR1_SetInterruptHandler(tmr0);
    // When using interrupts, you need to set the Global and Peripheral Interrupt Enable bits
    // Use the following macros to:

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();
    uint16_t  b[32]= {0};
//   FLASH_WriteBlock(0x2000, b);
//    FLASH_Write_a_word(0x2000, 0xAAAA);
   while (1)
    {
//       fengming();
        // Add your application code
    }
}

void tmr0(void)
{
   // uint16_t a[32] = {0};
   //FLASH_WriteBlock(0x3000, a);
    IO_RA0_Toggle();
    IO_RA1_Toggle();
    IO_RB5_Toggle();
     DELAY_milliseconds(50);
}

//void fengming(void)
//{
//    IO_RB5_Toggle()  ;
//    DELAY_milliseconds(200);
//}
///**
// End of File
//*/
