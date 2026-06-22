/**  CLOUD ENABLED IOT THERMAL ALERT AND LOGGING SYSTEM **/

#include  "headers.h"
#include "cloud_thermal_defines.h"
#include  "cloud_fun_declarations.h"
#include  "init_devices.h"
#include "ext_interrupt_init.h"
#include "esp01.h"

int main()

{

	  char read[5];

	   
	Initialize();                // ALL INTERFACECS INTIALIZING.

	   BeforeWhile();               // INITIAL SETPOINT TO CLOUD AND ALSO TO THE EEPROM.

     // min = MIN + 2;

     while(1)

     {
                         //to dispaly RTC time in LCD
		                 GetRTCTimeInfo(&v.hour,&v.min,&v.sec);
					  
			             	DisplayRTCTime(v.hour,v.min,v.sec);
                           //reading temperature values from lm35 and adc
				 						 Read_Temperature('c',&temperature);

				  						 

			              Display_temperature_on_LCD(); 
					     		 delay_Ms(500);

			              // INTERRUPT RELATED FLAG.

                          // SETPOINT BY MANUALLY.			 
						if(isr_flag)

						{
							   isr_flag = 0;
							  
				
							   esp01_sendsetpoint(setpoint_value);

							   Setcursor(1,0);
								StrLCD(setpoint_value);
							   delay_Ms(3000);
							//writing initial set point into eeprom
							   I2C_EEPROM_PAGEWRITE(EEPROM_SENSOR ,0x0010 ,setpoint_value , 5);
							   CmdLCD(0x01);
							   Setcursor(1,0);
							   StrLCD(read);
							   //while(1);
			                  setpoint = myatof(setpoint_value) + 0.00001f;
							   StrLCD("Updating......");

							   delay_s(3);

							   CmdLCD(CLEAR_LCD);
							   Setcursor(1,0);
							   F32LCD(setpoint ,6);
							   delay_s(10);
						}
                      //Flag related to time for cloud to send for every 2 minutes
						if(flag)

						{
						             I2C_EEPROM_SEQREAD(EEPROM_SENSOR ,0x0010 ,setpoint_value , 5);
																		 
						             setpoint = myatof(setpoint_value) + 0.00001f;

							         flag=0;

							         CmdLCD(CLEAR_LCD);

							         Setcursor(1,0);

							         CharLCD(0x23);

							         StrLCD("Reading LM35..");

								     Setcursor(2,0);

							

								 for(k=0;k<16;k++)

								 {

									 CharLCD(0x5F);

									 delay_Ms(10);

								 }

								     delay_Ms(300);

							         Read_Temperature('c',&temperature);

							         myftoa(temperature);

								 CmdLCD(CLEAR_LCD);

								 Setcursor(1,0);

								 StrLCD("Reading Temperature");

								 delay_Ms(200);

								 CmdLCD(CLEAR_LCD);

								 Setcursor(1,0);

							     StrLCD("sending to cloud.");

								 Setcursor(2,0);

								

								 for(k=0;k<16;k++)

								 {

									 CharLCD(0x5F);

									 delay_Ms(10);

								 }

							         esp01_sendToThingspeak(sensor_value);

                                     min = MIN + 2;

							       if(min > 60)
	    					     {
     									 min = min - 60;
								     }
                     sec = SEC;	
								 if(sec > 60)

								 {

									 sec = sec - 60;

								 }

								 

								 CmdLCD(CLEAR_LCD);

							         StrLCD("Reading Setpoint.");

								 Setcursor(2,0);

								

								 for(k=0;k<16;k++)

								 {

									 CharLCD(0x5F);

									 delay_Ms(10);

								 }

                                 ReadSetpointFromESP_01(setpoint_value_cloud);

								 CmdLCD(CLEAR_LCD);

			                     setpoint_cloud = myatof(setpoint_value_cloud)+0.00001f;      // STORING THE SETPOINT FROM THE CLOUD WHICH IS RECENTLY CHANGED AND EVERYTIME CHECKING.

                                 setpoint = setpoint_cloud;
								 I2C_EEPROM_PAGEWRITE(EEPROM_SENSOR ,0x0010 ,setpoint_value_cloud , 5);

							// NOW REPLACE THE SETPOINT WITH NEW ONE.

					     	//	setpoint = myatof(setpoint_value_cloud) +  0.00001f;

																	 

						}
						// NOW  COMPARING IT WITH EEPROM RELATED SETPOINT.

			            // IF EQUAL FINE OTHERWISE WE NEED TO ASSIGN THE CLOUD SETPOINT.
						// STEP1: NEED TO CHANGE THE EEPROM SETPOINT WITH CLOUD SETPOINT.
						if((MIN == min && sec <= SEC ))
						{
						 flag = 1; 
						}

						if(temperature >= setpoint)

						{
							  IOSET0 = 1<<16;

						}
						else
							{ 
							  IOCLR0 = 1<<16;
						}							
		 }	
}		 


