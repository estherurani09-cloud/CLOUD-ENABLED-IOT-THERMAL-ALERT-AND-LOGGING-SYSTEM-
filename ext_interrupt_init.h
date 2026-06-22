//ext_interrupt_init.h
#include "types.h"

#ifndef  EXT_INTERRUPT_INIT_H
#define  EXT_INTERRUPT_INIT_H

// Function Prototype or Function Decleration of the Interrupt
void Init_EXT_Interrupt(void); 
// And Also Declaring  of the function ISR For the External_Interrupt
// Functon for Interrupt Service Routine (ISR)--FIQ Interrupt has the Fast and the First Priority

void LM35_THRESHOLD(void)__irq;

void  SHOW_SET_POINT(void)__irq;

#endif
