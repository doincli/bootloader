
#ifndef VARIABLES_H
#define	VARIABLES_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* VARIABLES_H */


uint8_t k[32]  __at(0x900);
uint8_t frame[32] __at(0x950);          // Write data from buffer to this array.
uint8_t i __at(0x9A0);  
uint8_t flag_erase __at(0x9AF);

//void TMR0_Int(void) ;
//void erase(void) ;           // Erase function
//void write(void) ;           // Write function
//void FLASH_Write_a_word(uint16_t writeAddr, uint16_t flashWordArray) ;
//uint8_t reverse_8(uint8_t k) ;
//uint16_t reverse_16(uint16_t k) ;
//uint16_t crc_test(uint8_t *addr, int num);
//void EUSART_receive(void) ;


void TMR0_Int(void) __at(0xA00);
void erase(void) __at(0xC00);           // Erase function
void write(void) __at(0xCA0);           // Write function
void FLASH_Write_a_word(uint16_t writeAddr, uint16_t flashWordArray) __at(0xD00);
uint8_t reverse_8(uint8_t k) __at(0xD40);
uint16_t reverse_16(uint16_t k) __at(0xDA0);
uint16_t crc_test(uint8_t *addr, int num) __at(0xE00);
void EUSART_receive(void) __at(0xF00);