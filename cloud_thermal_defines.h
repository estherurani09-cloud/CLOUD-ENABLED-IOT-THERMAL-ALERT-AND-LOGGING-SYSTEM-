// adc_defines.h
#define FOSC 12000000     // Frequency Oscillator
#define CCLK (FOSC*5)     // C.P.U Clock
#define PCLK  (CCLK/4)    // Pheripheral Clock
#define ADCCLK  3000000   // ADC CLOCK UPTO 4.5MHz We can Give 
#define CLKDIV ((PCLK/ADCCLK)-1) // CLOCK DIVISION
// defining for ADCR Special Function Register (SFR)
#define CLKDIV_BITS 8 // To Load the CLKDIV value to the ADCR Register
#define PWDN_BIT 21 // Power Down Bit Whenever u want use the ADC pheriperal u need to set the powerdown bit
#define CONV_START_BIT 24 // Same here we need to set before starting the conversion
// Configure ADDR S.F.R Register
#define RESULT_BITS 6
#define DONE_BIT 31 // if the bit contains 1 Data Conversion is happen

//defines.h
// MACROS IS TO DECREASE THE SIZE OF THE CODE AND ALOS EASY READABILITY
#ifndef  DEFINES_H
#define  DEFINES_H
// >>>>>>>>>>>>>>>>>>>>>>>>.........................
#define SETBIT(WORD,BIT) (WORD|=1<<BIT)  ///--TO SET THE BIT TO 1
#define CLRBIT(WORD,BIT) (WORD&=~(1<<BIT)) // ---TO CLEAR PARTICULAR BIT
#define READBIT(WORD,BIT) ((WORD>>BIT)&1) // BIT STATUS
#define CPLBIT(WORD,BITPOS) (WORD^=(1<<BITPOS)) // TOGGLE OR COMPLIMENT BIT
#define WRITEBIT(WORD,BITPOS,BITLEVEL) \
(BITLEVEL?(WORD|=1<<BITPOS):(WORD&=~(1<<BITPOS))) // WRITING ONE BIT DATA TO ANOTHER PIN

// READING BIT AND WRITING SAME BIT TO THE ANOTHER BIT
#define READWRITEBIT(DWORD,DBIT,SWORD,SBIT)\
        (DWORD=((DWORD&~(1<<DBIT))|(((SWORD>>SBIT)&1)<<DBIT)))
// WRITNG TO THE BYTE WITHOUT AFFECTING THE OTHER BITS
/*#define WRITEBYTE(WORD,BITSTARTPOS,BYTE)\
        (WORD=(WORD&~(0xFF<<BITSTARTPOS))|(BYTE<<BITSTARTPOS))*/
#define WRITEBYTE(WORD, BITSTARTPOS, BYTE) \
    do { \
        WORD = (WORD & ~(0xFFUL << (BITSTARTPOS))) | ((BYTE & 0xFFUL) << (BITSTARTPOS)); \
    } while (0)
// NIBBLE RELATED MACROS
#define WRITENIBBLE(WORD,NIBPOS,NIBBLE)\
         (WORD=(WORD&~(0xF << (NIBPOS)))|(NIBBLE<<NIBPOS))

#define READNIBBLE(WORD,NIBPOS)  ((WORD>>NIBPOS)&(0xF))

#endif


//ext_interrupt_defines.h		
#ifndef  EXT_INTERRUPT_DEFINES_H
#define  EXT_INTERRUPT_DEFINES_H
// Decleraing the MACROS For the External Interrupts Intialization purpose.
// And Also the ISR definintion.
#define EINT3_CHNO 15  // We using only one Interrupt.
                       // Taking One External Interrupt Channel from "Vector Interrupt Controller" (VIC).
#define EINT2_CHNO 16  
#endif

		
//eeprom_device_address.h
#ifndef   EEPROM_DEVICE_ADDRESS_H
#define   EEPROM_DEVICE_ADDRESS_H

// 24C01 EEPROM DEVICE ADDRESSES.
/* In the EEPROM IC We have the 3 pins for this address selection,
   NOTE: Some EEPROMS only.
   so we can use multiple SLAVES with  the Single EEPROM/.
   multiple SLAVES means 2^3= 8 SLAVES Can we Use.
   It Depends Upon the EEPOMS Type and the company some may give more than 3 and less than 3
*/   
   // And here in 3 pins mode they give the 5 BIT Device ADDERSS as same.
   // Based on that we can connect the hardware.

#define  ADDR1     0x50    // 0101 0000      actually 1010 real address part.
#define  ADDR2     0x51    // 0101 0001
#define  ADDR3     0x52    // 0101 0010
#define  ADDR4     0x53    // 0101 0011
#define  ADDR5     0x54    // 0101 0100
#define  ADDR6     0x55    // 0101 0101
#define  ADDR7     0x56    // 0101 0110
#define  ADDR8     0x57    // 0101 0111

#define  EEPROM_SENSOR   ADDR1 // (Testing Purpose).

#endif



//i2c_defines.h		
#ifndef  I2C_DEFINES_H
#define  I2C_DEFINES_H

// I2C - Inter Integerated Circuit.
// Peripheral Method.
// So we to Provide Speed of the I2C BUS And Also Provide the Clock to the Perpheral is also necsseccary.

/*#define    FOSC          12000000               // Frequecny Oscillator.
#define    CCLK          (5*FOSC)               // LPC2129 & LPC2148 Operationg Frequecny.
#define    PCLK          (CCLK/4)   */            // Peripheral Clock Frequecny.
#define    I2C_SPEED     100000                // In Hz's.
#define    I2C_DIVIDER   ((PCLK/I2C_SPEED)/2)   // CLOCK GENERATION.

// BIT NAMES DEFINING ACCORDING TO THE DATA SHEET.

//  I2CONSET I2C SFR (8BIT Register)  same as the IOSET0.

#define    AA_BIT     2   // Accert Acknolwdegment Bit. AA - 0 Need More Bytes to Read.
                          // AA - 1 Master accknowledge that enough no of bytes.
#define    SI_BIT     3     // Serial Interrupt Bit.
                          /* 
													   The SI bit is tell that after every event or action like start,ack,nack,
														 and stop conditions and data soo on. whether it is done succsefully or not.
														 SI - 0 Not Successufull. 
														 SI - 1 Successufully Completed the event.
                          */
#define    STO_BIT    4   //  LOW to HIGH Pulse Generation.
#define    STA_BIT    5   //  HIGH to LOW Pulse Generation.
#define    I2CENABLE  6   // if we want to start or make a I2C Communcication only connections are not enough.
                          // We need to set this bit as 1.
													// Then only the I2C Peripheral Can Understand that the use need me. so in that way it will Does the Work.
													
//  I2CONCLR I2C SFR (8BIT Register)  same as the IOCLR0.	
//  For Clearing Purpose we use this I2CONCLR  Register so  bit names (MACRO Names ) we are changing.
// C ---> Indicates that Clearing Purposes.

#define    AAC_BIT    2    // Acert ACK Bit For Clearing Purpose.
#define    SIC_BIT    3    // Serial Interrupt.
#define    STAC_BIT   5    // START BIT. (for stoping the pulse) as HIGH to LOW.
#define    I2CDISABLE 6    // To stop the BUS as the I2C Communication.
#endif

//kpm_defines.h
#ifndef  KPM_DEFINES_H
#define  KPM_DEFINES_H
// Connecting These Pins To PORT1 Of a Microcontroller.
// PIN number (p 1.16 to p 1.23)
// DEFINING THE PINS THAT TO BE CONNECTED TO THE KEYPAD MATRIX.
// ROWS AND THE COLOUMS.
// ROWS
#define ROW0 16
#define ROW1 17
#define ROW2 18
#define ROW3 19

// COLUMNS
#define COL0 20
#define COL1 21
#define COL2 22
#define COL3 23
#endif

//lcd_defines.h
#ifndef  LCD_DEFINES_H
#define  LCD_DEFINES_H
// HERE THE MODES OF OPERATIONS ARE DEFINED AS THE MACROS RATHER WRITING DIRECTLY PREFERING THIS WAY IS BETTER OPTION
//>>>>>
#define CLEAR_LCD          0x01
#define RET_CUR_HOME       0x02
#define SHIFT_CUR_RIGHT    0x06
#define SHIFT_CUR_LEFT     0x07
#define DISP_OFF           0x08
#define DISP_ON_CUR_OFF    0x0C
#define DISP_ON_CUR_ON     0x0E
#define DISP_ON_CUR_BLINK  0x0F
#define SHIFT_DISP_RIGHT   0x14
#define SHIFT_DISP_LEFT    0x10
#define MODE_8BIT_1LINE    0x30
#define MODE_8BIT_2LINE    0x38
#define GOTO_LINE1_POS0    0x80
#define GOTO_LINE2_POS0    0xC0
//// CGRAM COMMAND 
#define GOTO_CGRAM_START    0x40
#endif


//pin_function_defines.h
#ifndef   PIN_FUNCTION_DEFINES_H
#define   PIN_FUNCTION_DEFINES_H

// Here we are defining the pin functions what this pin should like that we are defining 
//it in the macros
#define PIN_FUNC1 0
#define PIN_FUNC2 1
#define PIN_FUNC3 2
#define PIN_FUNC4 3
/*           I2C PINFUNCTIONS ON LPC2129 and LP2124 (Proteous Purpose)  */
#define  SDA_PORT0_PIN3    PIN_FUNC2
#define  SCK_PORT0_PIN4    PIN_FUNC2

#endif


//rtc_defines.h
// RTC (REAL TIME CONNECTION RELATED MACROS)
// rtc_defines.h
#ifndef RTC_DEFINES_H
#define RTC_DEFINES_H

// System Clock and Peripheral Clock Genration.

/*#define  FOSC         12000000     // Frequency Oscillator. FOSC - 12MHz.
#define  CCLK         (FOSC*5)     // CPU Clock is 60MHz.
#define  PCLK         (CCLK/4)     // Peripheral CLOCK 15MHz.*/

// RTC PREINT and PREFRAC Registers Data Loading.
// Formula's are Taken from the Data Sheet Only.
// RTC Crystal required frequecny is 32.768KHz.

#define  PREINT_VAL   ((PCLK/32768)-1) 
#define  PREFRAC_VAL  (PCLK-(PREINT_VAL+1)*32768)

// RTC Peripheral Registers Configuring the Register through the MACROS For more READABILITIY purpose.
// Clock Control Register.(CCR).

#define  RTC_ENABLE   (1<<0)       // ENABLING the BIT 0 of CCR Register.(RTC) Time Counters are Enabled.
#define  RTC_RESET    (1<<1)       // RESETING the Time Counters to Zero, means it again start from Begining.

// For LPC2148 We have two RTC'S ON Chip and the External RTC.
// To configure the  PRESCALER Register we have to set the single bit only.

#define  RTC_CLKSRC   (1<<4)
//#define LPC2148_MODE
#endif


//uart_defines.h
// UART_DEFINES

#ifndef  UART_DEFINES
#define  UART_DEFINES

/*#define   FOSC        12000000
#define   CCLK        (FOSC*5)
#define   PCLK        (CCLK/4)*/
#define   BAUD        9600
#define   DIVISOR     (PCLK/(16*BAUD))

#define   WORD_LENGTH  3             /* Need to Load in the U0LCR Register.
                                        
                                        8 BIT Word Length Selecting - 11(0th and 1st BIT).
                                     */																				


#define   DLAB_BIT     7             /* Setting it as 1 and 0
                                         
																				 1 - Load the DIVISOR Value in the U0DLM and U0DLL.
																				 0 - Enable the usage of U0THR and the U0RBR Buffer Registers.
																		 */
#define  RDR           0             /* Recivere Data Ready Bit In the U0LSR (Line status Register).
                                        
                                          1 - U0RBR Contains Valid Data 
                                          0 - U0RBR Is empty.(Means still not recived full length Data).
                                     */
#define  TEMT          6            /*    Tramsmitter empty Bit.
                                          
																					1 - U0THR is empty (means it is totally transmitted the full data).
																					0 - UOTHR is contains valid Data ( still Not transmitted) means transmission is in Progress.
																		*/													
#endif
