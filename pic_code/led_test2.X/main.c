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

/*
                         Main application
 */
void main(void)
{
    // initialize the device
    SYSTEM_Initialize();

    // When using interrupts, you need to set the Global and Peripheral Interrupt Enable bits
    // Use the following macros to:

    // Enable the Global Interrupts
    //INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

unsigned char  Music_Code[]={ 0x15,0x02, 0x15,0x02, 0x19,0x02, 0x19,0x02, 0x1A,0x02,
                                  0x1A,0x02, 0x19,0x01, 0x18,0x02, 0x18,0x02, 0x17,0x02,
                                  0x17,0x02, 0x16,0x02, 0x16,0x02, 0x15,0x01, 0x19,0x02,
                                  0x19,0x02, 0x18,0x02, 0x18,0x02, 0x17,0x02, 0x17,0x02,
                                  0x16,0x01, 0x19,0x02, 0x19,0x02, 0x18,0x02, 0x18,0x02,
                                  0x16,0x02, 0x15,0x01, 0x00,0x00 };
								  
								  
    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();
	
	unsigned int i;
 unsigned char sound_signal;//定义音符大小 
    void delay(unsigned char n);
    while (1)
    {
		unsigned int i=0;
		 sound_signal=Music_Code[i];
             i=i+1;
       IO_RB5_PORT=1;  
        delay(sound_signal);
        IO_RB5_PORT=0;  
         delay(sound_signal);
        // Add your application code
    }
}

void delay(unsigned char n)
{
   unsigned char i;
 while(n--)
 {
  for(i=0;i<100;i++);
 }
}
/**
 End of File
*/