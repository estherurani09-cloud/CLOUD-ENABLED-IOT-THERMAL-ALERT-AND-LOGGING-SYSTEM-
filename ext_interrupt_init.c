//ext_interrupt_init.c
// Here in the EXT_Interrupt_Init.c 
// Initalizing the External_Interrupt SFRS and Interrupts SFRS.

#include "cloud_fun_declarations.h"
#include "cloud_thermal_defines.h"
#include "types.h"
#include "ext_interrupt_init.h"
#include<LPC21xx.h>
extern  float setpoint;
extern volatile int isr_flag;
extern char uart_isr;
extern char setpoint_value[6];
void Init_EXT_Interrupt()
{
	CfgPortPinFunc(0,14,PIN_FUNC3); // Cnfiguring the on LPC2129 port 0.1 as the EXTERNAL INTERRUPT PIN.
	                                // THROUH PINSEL SFRS.
 
	CfgPortPinFunc(0,15,PIN_FUNC3); // Cnfiguring the on LPC2129 port 0.1 as the EXTERNAL INTERRUPT PIN.
	                                // THROUH PINSEL SFRS.
	
	 // Taking the Mode of the Interrupt is FIQ type. (FAST INTERRUPT REQUEST).
	
	 // Now Via or with Help of Vector Interrupt Controller (VIC) Registers (SFRS). Selcting and Enabling those SFRS for  Interrupt Intialization.
	
	 //VICIntSelect=1<<EINT3_CHNO;  // Making the EXTINT0 (14th CHANNEL OF VIC) as the FAST INTERRUPT REQUEST MODE.
	                              // FIQ MODE SELECTING.
	
	 VICIntEnable |=(1<<EINT3_CHNO|1<<EINT2_CHNO);// After Mode Selection we need to Enable the Interrupt Channel Number through VICIntEnable SFR.
	
	
	 VICVectCntl1  =  ((1<<5) | (EINT3_CHNO)) ;
	  
	 VICVectAddr1  =  (unsigned int)LM35_THRESHOLD;

	 VICVectCntl2 = ((1<<5)|(EINT2_CHNO));
	 VICVectAddr2 = (unsigned int)SHOW_SET_POINT;

	 // Now We Need to Configure the EXT_INTERRUPTS Registers(SFRS) For Enabling the Interrupt Signal to CPU.
	 
	 EXTINT=0; // We are Uisng Only the EINT0 only enabling this is Enough (but by default all are enable).
	 
	 EXTMODE=1<<1|1<<2; // Selection of MODE.
	                      // MODE means LEVEL TRIGGERING or EDGE TRIGGERING.
								        // By default in the Register EXTMODE all are LEVEL TRIGGERED.
								        // I am goin with the EDGE TRIGGERING the only EINT0 only.
	
	 //EXTPOLAR=1<<0; // By default all are Falling Edge.
	                // Here I am going with the Raising Edge,for EINT0 only.
									
	 /** THE COMPLETION OF INTIALIZATION OF INTERRUPT FUNCTION  **/
	
 }
void LM35_THRESHOLD(void)__irq
{
	
		TakeSesnsorData(setpoint_value);	
	    isr_flag=1;
    	EXTINT=1<<1;
        VICVectAddr=0;
	    CmdLCD(CLEAR_LCD);
	
}

void  SHOW_SET_POINT(void)__irq
{
	  CmdLCD(CLEAR_LCD);
	  Setcursor(1,0);
	  //CharLCD(0x2A);
	  StrLCD("SET POINT TEMP:");
	  Setcursor(2,0);
	  F32LCD(setpoint,2);
	  delay_Ms(1000);
	  CmdLCD(CLEAR_LCD);
	  
	  // Interrupt Related things.
	  EXTINT = 1<<2;
	  VICVectAddr=0;
}
