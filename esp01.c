//espo1.c
#include <string.h>
#include "cloud_fun_declarations.h"
#include "cloud_thermal_defines.h"
#include "types.h"

extern char buff[200];
extern unsigned char i;
extern char restart;
extern char uart_isr;
extern float setpoint;
void esp01_connectAP(void)
{
    CmdLCD(0x01);
	CmdLCD(0x80);
	StrLCD("AT");
	delay_Ms(1000);
	UART_TxString("AT\r\n");
	i=0;memset(buff,'\0',200);
	while(i<4);
	delay_Ms(500);
	buff[i] = '\0';
	CmdLCD(0x01);
	CmdLCD(0x80);
	StrLCD(buff);
	delay_Ms(2000);
	if(strstr(buff,"OK"))
	{
		CmdLCD(0xC0);
		StrLCD("OK");
		delay_Ms(1000);
	}
	else
	{
		CmdLCD(0xC0);
		StrLCD("ERROR");
		delay_Ms(1000);
		return;
	}


	CmdLCD(0x01);
	CmdLCD(0x80);
	StrLCD("ATE0");
	delay_Ms(1000);
	UART_TxString("ATE0\r\n");
	i=0;memset(buff,'\0',200);
	while(i<4);
	delay_Ms(500);
	buff[i] = '\0';
	CmdLCD(0x01);
	CmdLCD(0x80);
	StrLCD(buff);
	delay_Ms(2000);
	if(strstr(buff,"OK"))
	{
		CmdLCD(0xC0);
		StrLCD("OK");
		delay_Ms(1000);
	}
	else
	{
		CmdLCD(0xC0);
		StrLCD("ERROR");
		delay_Ms(1000);
		return;
	}


	CmdLCD(0x01);
	CmdLCD(0x80);
	StrLCD("AT+CIPMUX");
	delay_Ms(1000);
	UART_TxString("AT+CIPMUX=0\r\n");
	i=0;memset(buff,'\0',200);
	while(i<4);
	delay_Ms(500);
	buff[i] = '\0';
	CmdLCD(0x01);
	CmdLCD(0x80);
	StrLCD(buff);
	delay_Ms(2000);
	if(strstr(buff,"OK"))
	{
		CmdLCD(0xC0);
		StrLCD("OK");
		delay_Ms(1000);
	}
	else
	{
		CmdLCD(0xC0);
		StrLCD("ERROR");
		delay_Ms(1000);
		return;
	}

	CmdLCD(0x01);
	CmdLCD(0x80);
	StrLCD("AT+CWQAP");
	delay_Ms(1000);
	UART_TxString("AT+CWQAP\r\n");
	i=0;memset(buff,'\0',200);
	while(i<4);
	delay_Ms(1500);
	buff[i] = '\0';
	CmdLCD(0x01);
	CmdLCD(0x80);
	StrLCD(buff);
	delay_Ms(2000);
	if(strstr(buff,"OK"))
	{
		CmdLCD(0xC0);
		StrLCD("OK");
		delay_Ms(1000);
	}
	else
	{
		CmdLCD(0xC0);
		StrLCD("ERROR");
		delay_Ms(1000);
		return;
	}



	CmdLCD(0x01);
	CmdLCD(0x80);
	StrLCD("AT+CWJAP");
	delay_Ms(1000);
	//need to change the wifi network name and password
	UART_TxString("AT+CWJAP=\"Estheru\",\"Rani102003\"\r\n");
	i=0;memset(buff,'\0',200);
	while(i<4);
	delay_Ms(2500);
	buff[i] = '\0';
	CmdLCD(0x01);
	CmdLCD(0x80);
	StrLCD(buff);
	delay_Ms(2000);
	if(strstr(buff,"WIFI CONNECTED"))
	{
		CmdLCD(0xC0);
		StrLCD("OK");
		delay_Ms(1000);
	}
	else
	{
		CmdLCD(0xC0);
		StrLCD("ERROR");
		delay_Ms(1000);
		return;
	}

}

void esp01_sendToThingspeak(char *val)
{
	uart_isr = 0;
	//CmdLCD(0x01);
	//CmdLCD(0x80);
	START_AGAIN:	//StrLCD("AT+CIPSTART");
	//delay_Ms(1000);
	UART_TxString("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n");
	i=0;memset(buff,'\0',200);
	while(i<5);
	delay_Ms(2500);
	buff[i] = '\0';
	//CmdLCD(0x01);
	//CmdLCD(0x80);
	//StrLCD(buff);
	//delay_Ms(2000);
	if(restart)
	{
    restart = 0;		
		goto START_AGAIN;
	}
	if(strstr(buff,"CONNECT") || strstr(buff,"ALREADY CONNECTED"))
	{
		//CmdLCD(0xC0);
		//StrLCD("OK");
		//delay_Ms(1000);

		//CmdLCD(0x01);
		//CmdLCD(0x80);
		//StrLCD("AT+CIPSEND");
		//delay_Ms(1000);
		UART_TxString("AT+CIPSEND=53\r\n");
		i=0;memset(buff,'\0',200);
		//while(buff[i] != '>');
		delay_Ms(500);
		
		UART_TxString("GET /update?api_key=4LNX7BZRUSF0DN8S&field1=");
		
		UART_TxString(val);
		UART_TxString("\r\n\r\n");
		delay_Ms(5000);
		delay_Ms(5000);
		buff[i] = '\0';
		//CmdLCD(0x01);
		//CmdLCD(0x80);
		//StrLCD(buff);
		//delay_Ms(2000);
		if(strstr(buff,"SEND OK"))
		{
			Setcursor(1,0);
			StrLCD("                   ");
			Setcursor(1,0);
			StrLCD("DATA UPDATED");
			delay_Ms(1000);
		//CmdLCD(0x01);
		}
		else
		{
			Setcursor(1,0);
			StrLCD("                   ");
			Setcursor(1,0);
			StrLCD("DATA NOT UPDATED");
			delay_Ms(1000);
			if(restart)
	    {
        restart = 0;		
		    goto START_AGAIN;
	    }
		}

	}
	else
	{
		  if(restart)
	    {
        restart = 0;		
		    goto START_AGAIN;
	    }
		//CmdLCD(0xC0);
		//StrLCD("ERROR");
		//delay_Ms(1000);
		//CmdLCD(0x01);
		//return;
	}

  uart_isr = 1;


}



void esp01_sendsetpoint(char *val)
{
	uart_isr = 0;
	CmdLCD(0x01);
	CmdLCD(0x80);
	StrLCD("AT+CIPSTART");
	delay_Ms(1000);
	UART_TxString("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n");
	i=0;memset(buff,'\0',200);
	while(i<5);
	delay_Ms(2500);
	buff[i] = '\0';
	CmdLCD(0x01);
	CmdLCD(0x80);
	StrLCD(buff);
	delay_Ms(2000);
	if(strstr(buff,"CONNECT") || strstr(buff,"ALREADY CONNECTED"))
	{
		CmdLCD(0xC0);
		StrLCD("OK");
		delay_Ms(1000);

		CmdLCD(0x01);
		CmdLCD(0x80);
		StrLCD("AT+CIPSEND");
		delay_Ms(1000);
		UART_TxString("AT+CIPSEND=53\r\n");
		i=0;memset(buff,'\0',200);
		//while(buff[i] != '>');
		delay_Ms(500);
		
		UART_TxString("GET /update?api_key=JI9MG9B8AHEEVO0V&field1=");
		UART_TxString(val);
		UART_TxString("\r\n\r\n");
		delay_Ms(5000);
		delay_Ms(5000);
		buff[i] = '\0';
		CmdLCD(0x01);
		CmdLCD(0x80);
		StrLCD(buff);
		delay_Ms(2000);
		if(strstr(buff,"SEND OK"))
		{
			CmdLCD(0x01);
			StrLCD("DATA UPDATED");
			delay_Ms(1000);
			CmdLCD(0x01);
		}
		else
		{
				
			CmdLCD(0x01);
			StrLCD("DATA NOT UPDATED");
			delay_Ms(1000);
			CmdLCD(0x01);
		}

	}
	else
	{
			
		CmdLCD(0xC0);
		StrLCD("ERROR");
		delay_Ms(1000);
		return;
	}

							  

}

void ReadSetpointFromESP_01(char *p)
{
	  
	 char *q = NULL;
	 short int j=0;
	// CmdLCD(CLEAR_LCD);
	 uart_isr = 0;
	 //Setcursor(1,0);
	START_AGAIN_2: //StrLCD("AT+CIPSTART");
	 //delay_Ms(1000);
	 UART_TxString("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n");
	 i=0;
	 memset(buff,'\0',200);  // Placing NULL fully.
	 while(i<5); // Waits for the UART Interrupt Entries.
   delay_Ms(2500);
   buff[i]='\0'; //already will be NULL But Safety.
  // CmdLCD(CLEAR_LCD);
   //Setcursor(1,0);
	 //StrLCD(buff);
	 //delay_Ms(2000);
	 if(restart)
	    {
        restart = 0;		
		    goto START_AGAIN_2;
	    }  
	// checking that what ESP 01 Has sends to us.
	
	if(strstr(buff,"CONNECT")|| strstr(buff,"ALREADY CONNECTED"))
	{
		   //Setcursor(2,0);
		   //StrLCD("OK..");
		   //delay_Ms(1000);
		   //CmdLCD(CLEAR_LCD);
		   //Setcursor(1,0);
		  // Next procedure to give the commands.
		  
		   //StrLCD("AT+CIPSEND");
		  // delay_Ms(1000);
		   UART_TxString("AT+CIPSEND=66\r\n");
		   // again making buffer as all NULL Characters.
		   i=0;
		   memset(buff,'\0',200);
		   delay_Ms(500);
		   UART_TxString("GET /channels/2308650/fields/1/last.txt?api_key=JI9MG9B8AHEEVO0V\r\n");
		  // UART_TxString("https://api.thingspeak.com/channels/2308650/fields/1/last.json?api_key=F5WDIS8FL7W3YYY5\r\n");
		   // wait some more time.
       delay_Ms(10000);
		   // now keep the NULL For safety.
		   buff[i]= '\0';
		   // CLEAR THE LCD.
		   //CmdLCD(CLEAR_LCD);
			 // setting the cursor.
		/* Setcursor(1,0);
			 StrLCD(buff);                 DEBUGGIN PURPOSE ONLY.
			 delay_Ms(2000);*/
			 
		   if((q=strchr(buff,':'))!=NULL)
			 {
				  q++;
					while( ( ((*q >= '0') && (*q <= '9')) || (*q == '.') ) && (j < 5) )
					{
						  p[j] = (*q);
						  j++;
						  q++;
						  
					}
					 p[j]='\0';
					
					 CmdLCD(CLEAR_LCD);
					 Setcursor(1,0);
					 StrLCD("DATA READ IS:");
					 Setcursor(2,0);
					 StrLCD(p);
					 delay_Ms(2000);
					 CmdLCD(CLEAR_LCD);
			 }
			 else
			 {
				    if(restart)
	          {
              restart = 0;		
		          goto START_AGAIN_2;
	          }	 
						
				    CmdLCD(CLEAR_LCD);
					  Setcursor(1,0);
					  StrLCD("DATA NOT READ");
				    delay_Ms(3000);
				    CmdLCD(CLEAR_LCD);
			 }
     }
			else
			{ 
							if(restart)
							{
              restart = 0;		
							goto START_AGAIN_2;
							}
				    Setcursor(2,0);
				    StrLCD("ERROR...");
				    delay_Ms(2000);
				    CmdLCD(CLEAR_LCD);
				    return;
			}
			  uart_isr = 1;
	}
