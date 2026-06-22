//#include  "headers.h"
#include "cloud_thermal_defines.h"
#include  "init_devices.h"
#include  "cloud_fun_declarations.h"
#include "ext_interrupt_init.h"
#include "esp01.h"
//init_devices.c
// Function definition.
extern float setpoint;
extern unsigned char CGRAM[40];
extern  char sensor_value[6];
extern char setpoint_value[6];


void Initialize(void)
{    
	 Init_UART(); 
   	Init_I2C();
		Init_LCD();
		Init_Keypad();
		Init_ADC();
		Init_RTC();
		Init_EXT_Interrupt();
		SETBIT(IODIR0,16);
		return;
	 // Init_alaram();
}
void  BeforeWhile(void)
{
	  char anime[16]="CLOUD IOT SYS...";
	  char boot[10]="BOOTING...";
	  //char rtc[14]=" RTC INIT OK..";
	  char ready[15]="SYSTEM READY...";
	  int a;
	  
	  RetroTech();
	  for(a=0;a<15;a++)
	  {
		Setcursor(1,a);
		CharLCD(anime[a]);
		delay_Ms(150);
		}
		for(a=0;a<10;a++)
		{
		Setcursor(2,a);
		CharLCD(boot[a]);
		delay_Ms(150);
		}
		delay_Ms(500);
		CmdLCD(CLEAR_LCD);
	  Setcursor(1,0);
		CharLCD(0x3E);
		delay_Ms(150);
		for(a=0;a<14;a++)
		{
			CharLCD(ready[a]);
			delay_Ms(150);
		}
		delay_Ms(500);
		CmdLCD(CLEAR_LCD);
		Setcursor(1,0);
		CharLCD(0x3E);
		StrLCD("ESP CONNECCTING");
		Setcursor(2,0);
		StrLCD("WIFI...");
		delay_Ms(300);
	  SetRTCTimeInfo(16, 0 ,0);
	  BuildCGRAM(CGRAM,64);
	 //setpoint = 16.23;
	 // strcpy(setpoint_value,"16.23");
	
	  // SENDING THE SETPOINT INTO THE EEPROM
//	  I2C_EEPROM_PAGEWRITE(EEPROM_SENSOR ,0x0010 ,setpoint_value , 5);
	  I2C_EEPROM_SEQREAD(EEPROM_SENSOR ,0x0010 ,setpoint_value , 5);
	  Setcursor(1,0);
	  /*CmdLCD(CLEAR_LCD);
	StrLCD(setpoint_value);
	while(1);				  */

	 
	  setpoint = myatof(setpoint_value)+ 0.00001f;
	 /*CmdLCD(CLEAR_LCD);
	  F32LCD(setpoint , 6);
	  while(1);*/

	  // CONNECTING TO THE ESP01 .
	  esp01_connectAP();
	
	  // NOW SENDING THE SETPOINT INTO THE CLOUD.
	  esp01_sendsetpoint(setpoint_value);
	  
}

unsigned int mylen(char *p)
{
	unsigned int len = 0;
	while(*p)
	{
		len++;
		p++;
	}
	return len;
}


/**    FLOAT TO ASCII CONVERSION AND VIA THIS SENDING IT IN TO THE CLOUD   **/

/**    FROM THE LM35 FLOAT VARIABLE TO TO SENDING TO THE CLOUD             **/

/**        SENSOR_VALUE ( String type ) = TEMPERATURE ( float type )       **/


void myftoa(float f_sensor)
{
	  int num1,num2,i=0,j;
	  num1 = f_sensor;
	  f_sensor =(f_sensor - num1)*100 + 0.5;
	  num2 = f_sensor;
	  if(num2 == 0)
		{
			sensor_value[i++] = 0 + 48;
			sensor_value[i++] = 0 + 48;
		}
	  while(num2)
		 {
			 sensor_value[i++] = num2%10 + 48;            // Numeric to ASCII Conversion.
			 num2/=10;
 		 }
		 sensor_value[i++] = 46;
		 while(num1)
		 {
			 sensor_value[i++] = num1%10 + 48;            // Numeric to ASCII Conversion.
			 num1/=10;
		 }
		 sensor_value[i]='\0';
		 i=0;
     j=mylen(sensor_value) -1;
		 while(i<j)
		 {
			 sensor_value[i]^=sensor_value[j];
			 sensor_value[j]^=sensor_value[i];
			 sensor_value[i]^=sensor_value[j];
			 i++;
			 j--;
		 }
}

float myatof(char *p)
{
	float dec = 0.1;
	float f_sensor=0;
	char f=0;
	while(*p)
	{
		if(*p==46)
		{
			f=1;
			p++;
			continue;
		}
		if(*p>='0' && *p<='9' && f==0)
		{
			f_sensor = f_sensor*10 + ((*p)-'0');
		}
		else if(f)
		{
			f_sensor = f_sensor + dec*((*p)-'0');
			dec*=0.1f;
		}
		p++;
	}
	return f_sensor;
}

