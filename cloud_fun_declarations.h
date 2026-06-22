#include "types.h"
#include <LPC21XX.h>
//adc.h
#ifndef  ADC_H
#define  ADC_H

void Init_ADC(void);
void Read_ADC(u32 chNo,u32 *adcDval,f32 *adcAval);

#endif


//alarm.h
#ifndef ALARAM_H
#define ALARAM_H

void Init_alaram(void);
void OFF(void);
void ON(void);

#endif


//delay.h
#ifndef DELAY_H
#define DELAY_H
// HERE IN THE DELAY.H HEADER FILE WE NEED TO DECLEARE THE FUNCTION PROTOTYPES
// BASCICALLY ONLY THE FUNCTION DECELERATIONS WE RIGHT HERE ITSELF 
// here i am usinf the typedef type usage so  i include my declared file as types.h which i called here
void delay_Us(u32 dlyUs);
void delay_Ms(u32 dlyMs);
void delay_s(u32 dlys);
#endif


//i2c_eeprom.h
#ifndef  I2C_EEPROM_H
#define  I2C_EEPROM_H

// Function Declerations or Prototypes.

// writing a byte into the EEPROM.
void I2C_EEPROM_BYTEWRITE(u8 SlaveAddr , u16 BuffAddr , u8 Data);

// Reading Byte from the EEPROM.
u8 I2C_EEPROM_RANDOMREAD(u8 SlaveAddr , u16 BuffAddr);

// Writing the Page (8 bytes = 1 page in 24C01 MODEL) into the EEPROM.
void  I2C_EEPROM_PAGEWRITE(u8 SlaveAddr ,u16 BuffStartAddr ,s8 *p , u8 Nbytes);

// Contiously Reading from the SLAVE.
void  I2C_EEPROM_SEQREAD(u8 SlaveAddr , u16 BuffStartAddr, s8 *p , u8 Nbytes);

#endif

//i2c_peripheral.h
#ifndef   I2C_PERIPHERAL_H
#define   I2C_PERIPHERAL_H

// Function Declerations of I2C peripherals

// Initialzing the I2C.
   void  Init_I2C(void);        

// I2C Start.
   void  I2C_START(void);
	
// I2C Stop.
   void  I2C_STOP(void);
	
// I2C RESTART.(Reading from the SLVAE Purposes).
   void  I2C_RESTART(void);
	 
// I2C WRITE
   void  I2C_WRITE(u8);
	 
// I2C NACK
   u8  I2C_NACK(void);
	
// I2C MACK    (MASTER ACK While Reading from the SLAVE).
   u8  I2C_MACK(void);

#endif

//init_devices.h
#ifndef INIT_DEVICES_H
#define INIT_DEVICES_H

void Initailize(void);
void BeforeWhile(void);
void myftoa(float );
float myatof(char *);
unsigned int mylen(char *);
#endif

//kpm.h
// FUNCTION DECLERATIONS OF THE KEYPAD MATRIX KEY SCANINNG
#ifndef  KPM_H
#define  KPM_H
// Initialization
void Init_Keypad(void);
// column Scan whether the key is pressed or not
u32 Colscan(void);
// checking the row where the key is pressed
u32 Rowcheck(void);
// checking that in specic row which column is pressed
u32 Columncheck(void);
// finally Scaning the value which key is preesed and what's the value of that key
u8 KeyScan(void);
// For Reading the Keys (numeric keypad taken here)
float ReadNum(float *num ,char *ch);
void TakeSesnsorData(char *);

#endif

//lcd.h
#ifndef  LCD_H
#define  LCD_H
// HERE FUNCTION DECLEARATION OR PROTITYPING OF THE LCD USED FUNCTIONS

void WriteLCD(u8 byte); // write to LCD either Command or Data or Mode Selection through this only we need to write
void CmdLCD(u8 CmdByte); // writing the paticular command and this function writelcd will takes place
void Init_LCD(void); // intitilization of the LCD pins number and Direction Register Setting
void CharLCD(u8 ascival); // writing character on the LCD internally it has the writeLCD will take place
void StrLCD(s8*); // for the string display
void U32LCD(u32); // displaying the integer value--only unsigned integer
void BuildCGRAM(u8*,u8);// OWN CHARACTERS DISPLAYING
void Setcursor(char mode,char pos);
void F32LCD(f32 fnum,u32 nDP);
void Display_temperature_on_LCD(void);
#endif

//lm35.h
#ifndef LM35_H
#define LM35_H
// function decleration.

void Read_Temperature(s8 TempType ,f32 *temp);

#endif

//pin_connect_block.h
#ifndef  PIN_CONNECT_BLOCK_H
#define  PIN_CONNECT_BLOCK_H
void CfgPortPinFunc(u32 portNo,u32 pinNo,u32 pinFunc);

#endif

//retrotech.h
// Company Logo.
#ifndef  RETROTECH_H
#define  RETROTECH_H

void RetroTech(void);

#endif


//rtc.h
#ifndef  RTC_H
#define  RTC_H

// RTC_INITIALIAZATION

void Init_RTC(void);  // Initiliazation of the RTC with Help of SFR'S (CCR).

// Reading From the SFR'S data and copying to GLOBAL VARIABLE in the program for the Application.
void  GetRTCTimeInfo(s32 *,s32 *,s32*);
void  GetRTCDateInfo(s32 *,s32 *,s32 *);
void  GetRTCDay(s32 *);
void  GetRTCHour(s32 *);
void  GetRTCMin(s32 *);
void  GetRTCSec(s32 *);
void  GetRTCDate(s32*);
void  GetRTCMonth(s32*);
void  GetRTCYear(s32*);
void  GetDay(s32*);

// Displaying them on the LCD Display through the functions.
// Displaying from the GLOBAL Varaibles.
void  DisplayRTCTime(u32 ,u32,u32);
void  DisplayRTCDate(u32,u32,u32);
void  DisplayRTCDay(s8);

// Setting the TIME , DATE , DAY , HOUR , MIN , SEC , DATE , MONTH , YEAR Directly into SFR'S
void  SetRTCTimeInfo(u32,u32,u32);
void  SetRTCDateInfo(u32,u32,u32);
void  SetRTCDay(u32);
void  SetRTCMonth(u32);
void  SetRTCYear(u32);
void  SetRTCDate(u32);
void  SetRTCHour(u32);
void  SetRTCMin(u32);
void  SetRTCSec(u32);

/*************** THIS FUNCTIONS DEFINITION USES THE RTC SFR'S like
           HOUR , MIN ,SEC , MONTH , YEAR , DOW(Day of week) , DOM(date of month) and all....
					 ***/
#endif

//uart.h
#ifndef  UART_H
#define  UART_H

void  Init_UART(void);
void UART_TxChar(unsigned char TxByte);
void UART_TxString(char *str);
void UART_U32(unsigned int num);
void UART_S32(int num);
void UART_F32(float fnum,unsigned char Ndp);
unsigned char UART_RxChar(void);
void  UART_RxString(char *str,unsigned MaxLen);
void UART_ISR(void)__irq;
#endif
