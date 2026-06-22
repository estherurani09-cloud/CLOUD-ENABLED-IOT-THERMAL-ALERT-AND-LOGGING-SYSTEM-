//headers.h
#ifndef HEADERS_H
#define HEADERS_H

/**  REQUIRED GLOBAL VARAIBLES IN THE PROJECT **/ 

// For the Setting the Temperature Threshold and storing it into the EEPROM.
float setpoint;

// For Every 2 Minutes Tracking the temperature and then sending into cloud and as well as displaying on the LCD.
float temperature;

// RTC Time Displaying on the LCD.
struct time
{
	 int hour;
	 int min;
	 int sec;
};
short int k;
// Taking set Point as the String and converting it into the ASCII and then storing back into the Character array.



char sensor_value[6];
char setpoint_value[6];
char setpoint_value_cloud[6];

// **  ISR FLAG  **/

volatile int isr_flag;
char uart_isr;
char restart;
// **  For the 2 Minutes after 2 Minutes Reading the temperature purpose  ** //

struct time v;
int min,sec;
char flag=1;

//  ** temperature  and the setpoint ** //

 //float temperature;
 // float setpoint;

//  ** FROM THE CLOUD SET POINT ** //
float setpoint_cloud;

#endif
