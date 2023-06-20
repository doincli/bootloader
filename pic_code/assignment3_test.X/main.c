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
     uint8_t tmp=0;
//     int a=5;
    // initialize the device
    SYSTEM_Initialize();
//     IO_RC7_SetLow();
//   tmp = EUSART1_Read();

//    // When using interrupts, you need to set the Global and Peripheral Interrupt Enable bits
//    // Use the following macros to:

    // Enable the Global Interrupts
    //INTERRUPT_GlobalInterruptEnable();
      IO_RC7_SetHigh();
      int i;
      for(i=0;i<100;i++)
      {
          printf("%d",i);
      }
    // Enable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();
//     
//    while (1)
//    {
//        
//         IO_RC7_SetLow()  ;
//         tmp = EUSART1_Read();
//         if(tmp!=0)
//         {
//         IO_RC7_SetHigh();
//         printf("Ssss");
//         EUSART1_Write(tmp+1);
//         printf("%d",tmp);
//         
//         }
//    }
////         int a=5;
////         if(0!=tmp)
////         {
////            
//             IO_RC7_SetHigh();
//              printf("%d\n",a);
////             tmp=tmp+1;
////          EUSART1_Write(tmp);
////          printf("%d",tmp);
////         }
//          
//    }
//while(1)
//        {
//            // Logic to echo received data
//            if(EUSART1_is_rx_ready())
//            {    
//                IO_RC7_SetLow()  ;
//                tmp = EUSART1_Read();
//                if(EUSART1_is_tx_ready())
//                {
//                    IO_RC7_SetHigh();
//                    EUSART1_Write(tmp);
//                }
//            }
//        }
    
    
    
}
/**
 End of File
*/