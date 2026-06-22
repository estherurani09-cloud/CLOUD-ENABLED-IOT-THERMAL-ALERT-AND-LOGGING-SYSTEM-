#include <string.h>
#include "cloud_fun_declarations.h"
#include "cloud_thermal_defines.h"
#include "types.h"
//adc.c
void Init_ADC(void)
{
	// Configure ADCR
	CfgPortPinFunc(0,28,PIN_FUNC2);
	ADCR|=((1<<PWDN_BIT)|(CLKDIV<<CLKDIV_BITS));
}

void Read_ADC(u32 chNo,u32 *adcDval,f32 *adcAval)
{
	// intially clear the channel
	ADCR=(ADCR&(0xFFFFFF00));
	ADCR|=((1<<CONV_START_BIT)|(1<<chNo));
	delay_Us(3);
	// check until the conversion is success
	 ADCR &= ~(1 << CONV_START_BIT);
	while(((ADDR>>DONE_BIT)&1)==0);
	*adcDval=((ADDR>>RESULT_BITS)&0x3FF);
	*adcAval=(*adcDval*(3.3/1024));
}


//alarm.c
// Function defintions.
void  Init_alaram(void)
{
	 // BUZZER.  
	 IODIR0 |= 1<< 13;
	
	 // LED'S.
   WRITEBYTE(IODIR1 , 24 , 0xFF);
	   
}

void ON(void)
{
	    IOSET0 = 1<< 13;
	  
}
void OFF(void)
{
	  IOCLR0 = 1<< 13;
	  
}

//delay.c
// NOW HERE I AM USING THIS FILE AS THE DELAY 
// MEANS ITS AN PERMENANET ONE I CANT WANT TO CHNAGE 
// THIS DELAY FILE I WILL USE EVERY TIME I WANT IN MY TASK

// No need of function declerations and prototypes because we will declare it in the seperate header file

/* >>Microsecond << */
void delay_Us(u32 dlyUs) 
{
	for(dlyUs*=12;dlyUs>0;dlyUs--); // I prefering the for loop which less instructtion while converting it in Aessembly compare to while loop
}
/* >> Milli second <<*/
void delay_Ms(u32 dlyMs)
{

	for(dlyMs*=12000;dlyMs>0;dlyMs--);
}
/* >> For generating the second directly <<*/
void delay_s(u32 dlys)
{
	for(dlys*=12000000;dlys>0;dlys--);
}

//i2c_eeprom.c
// Function Definitions.
void I2C_EEPROM_BYTEWRITE(u8 SlaveAddr , u16 BuffAddr , u8 Data)
{
	  // Initiate the Start Conditon.
	  I2C_START();
	
	  // Slave Address + R/W(BIT) and Take ACK
	  I2C_WRITE(SlaveAddr << 1);               // WRITE = 0 
	
	  // BuffAdrress & Take ACK.
	  I2C_WRITE(BuffAddr>>8);
	  I2C_WRITE(BuffAddr);
	  // Data and Take ACK.
	  I2C_WRITE(Data);
	 
	  // Stop the Communication by issuing the stop() function.
	  I2C_STOP();
	
	  // Cycle Delay.
	  delay_Ms(10);
}

u8 I2C_EEPROM_RANDOMREAD(u8 SlaveAddr , u16 BuffAddr)
{
	  u8 Data;
	  // Start Condition
	  I2C_START();
	  
	  // Slave Address + Write and Take ACK.
	  I2C_WRITE(SlaveAddr << 1);
	  
    // Buffer Address & Take ACK.
	  I2C_WRITE(BuffAddr>>8);
	  I2C_WRITE(BuffAddr);

	  
	  // Restart the coomunication while in the Reading Stage Only.
	  I2C_RESTART();
	 
	  // Slave address + read and Take ACK.
	  I2C_WRITE(SlaveAddr << 1 | 1);             // READ = 1
	 
	  // Read Byte From the Slave and Give NACK From the MASTER.
	  // Which means enough no of bytes.
	  Data = I2C_NACK();
	  
	  // Stop Condition.
		I2C_STOP();
		
		// Return the Data Content.
		return Data;
}
	
void  I2C_EEPROM_PAGEWRITE(u8 SlaveAddr ,u16 BuffStartAddr ,s8 *p , u8 Nbytes)
{
	  u8 i;
	  // Start Condition.
	  I2C_START();
	 
	  // Slave Address + w.
	  I2C_WRITE(SlaveAddr << 1);
	 
	  // BuffSatrt & take ACK
	  I2C_WRITE(BuffStartAddr>>8);
	  I2C_WRITE(BuffStartAddr);
	  
	  // Write n bytes , For Each Byte take the ACK.
	  
	  for(i=0; i<Nbytes ; i++)
	  {
			 // Write Data and Take ACK.
			 I2C_WRITE(p[i]);
		}
		
		// Stop Condition.
		I2C_STOP();
		
		// Write Cycle Delay.
		delay_Ms(10);
}

void  I2C_EEPROM_SEQREAD(u8 SlaveAddr , u16 BuffStartAddr, s8 *p , u8 Nbytes)
{
	 u8 i;
	 // Start Condition.
	 I2C_START();
	
	 // Slave Address + w and Take ACK.
	 I2C_WRITE(SlaveAddr << 1);
	
	 // Buffer Start Address and take ACK.
	 I2C_WRITE(BuffStartAddr>>8);
     I2C_WRITE(BuffStartAddr);	
	 // Restart the Communication.
	 I2C_RESTART();
	 
	 // Slave Address + READ
	 I2C_WRITE(SlaveAddr << 1 | 1);
	
	 // Read only N Bytes minus one.
	 for(i=0 ;i <Nbytes-1; i++)
	 {
		 p[i] = I2C_MACK();
	 }
	 
	 // Read the Last Byte and give NACK.
	 p[i] = I2C_NACK();
	 
	 // Stop Condition.
	 I2C_STOP();
 }

//i2c_peripheral.c
// Function Definitons are Here.
// Including the Required Header files.

void   Init_I2C(void)
{
	    // Configure p0.3 pin as the SDA Line.
	    CfgPortPinFunc(0 ,3 , SDA_PORT0_PIN3);
	    // Configure p0.4 pin as the SCK Line.
	    CfgPortPinFunc(0 ,2 , SCK_PORT0_PIN4);
	  
	    // Confugure the Speed for the I2C Serial Communication.
	    I2SCLL = I2C_DIVIDER;  // LOWR PART.
	    I2SCLH = I2C_DIVIDER;  // HIGHER PART.
	
	    // To enable the I2C Communication we need to set the I2CENABLE Bit using I2CONSET Register.
  	    I2CONSET = 1 << I2CENABLE;
}

void  I2C_START(void)
{
	    // START CONDITION.
	    I2CONSET = 1 << STA_BIT;
	    // Wait for the Event is Success or not.
	    while(((I2CONSET >> SI_BIT)&1)==0);        // Until SI_BIT - 1 PC  stay here only.
	    // The Start Conditon Does the CLOCK Geration as HIGH to LOW from the SDA pin so Communication Begins.
	    // After that we need to stop the HIGH to LOW Pulse Generation.
      	    // Because we need to start real Communication Such as DATA.

      	    // Clear STOP Condition.
            I2CONCLR = 1 << STAC_BIT;      // you can use Same Bit Also STA_BIT Both Bit Postions are same.
}	

void  I2C_RESTART(void)
{
	    // It is same as the I2C_START Only.
	    // We just Want the SI_BIT because in some where function the SI_BIT is 1 means successfull Event.
	    // so here we are performing the another event right.
	    // previous 1 we are trying to start another event so we need to clear the bit and start the eevent and  succesfull status will be there as 1.
	    // otherwise if the event is not done also it will shows it done.
	    
	    // Start Condition.
	    I2CONSET = 1 << STA_BIT;
	    // Clear the Previous Event success status bit the SI_BIT.
	    I2CONCLR = 1 << SIC_BIT;
	    // Wait for the Event is Success or not.
	    while(((I2CONSET >> SI_BIT)&1)==0);        // Until SI_BIT - 1 PC  stay here only.
	    // Clear STOP Condition.
      	    I2CONCLR = 1 << STAC_BIT;
}

void  I2C_WRITE(u8 Data)
{
	   // We are Writing into the EEPROM.
	   // We have the Register to hold our Data.
	    
	   I2DAT = Data;
	   // Remember every time we need to clear the SI_BIT if any event you are performing.
	   // so clear the SI_BIT;
	   I2CONCLR = 1 << SIC_BIT;
	   // Now wait for the status of the SI_BIT because in above 2 lines you are writing the data right we need to confirm right whether the event is success or not.
	   while(((I2CONSET >> SI_BIT)&1)==0);
}

unsigned char  I2C_NACK(void)
{
	   // NOACK  Stop Reading after 1 byte of Data.
	   // By default in the I2CONSET Register AA_BIT is 0.
	   // So no need to write the any to it.
	   
	   // Clear the previous event.
	   I2CONCLR = 1 << SIC_BIT; 
	   // Now wait for the data to collect from the I2DAT Register.
	   while(((I2CONSET >> SI_BIT)&1)==0);
     // Now return the data.
     return I2DAT;
}

unsigned char  I2C_MACK(void)
{
	  // While it is the READ mode.
	  // Master ACK.
	  // Whether the Master need the More Bytes or not.
	  // By setting AA_BIT 1 we can read the byte continously. util AA_BIT is set to 0.
	  
    /* In the MASTER Reciver Mode.
	     
	       Master must give the Acknowledgment whether he want to read another byte or not.
	      
	       ->  Yes , He want's Another Byte.
	 
	           SDA      -  LOW
	           AA_BIT   -  1
	           at the 9th Clock Pulse.
	 
         -> No , he Doesn't need.
	 
	           SDA      -  HIGH
	           AA_BIT   -  0
	           at the 9th Clock Pulse.
	  */
		// Assert ACK.
		I2CONSET = 1 << AA_BIT;   
		// Clear the Previous event status.
		I2CONCLR = 1 << SIC_BIT;
		// Wait for your event.
		while(((I2CONSET >> SI_BIT)&1)==0);
		// Clear the Assert Acknowledgment Bit (AAC_BIT) In I2CONCLR Register.
		I2CONCLR = 1 << AAC_BIT;
		// return the DATA That avaialbe in the I2DAT Register(internally in Controller it will Happens).
		return I2DAT;
	}

void  I2C_STOP(void)
{
	  // I2C STOP Conditon.(LOW To HIGH CLOCK PULSE GENERATION).
	  I2CONSET = 1 << STO_BIT;
	  // Clear the Previous Event (if any).
	  I2CONCLR = 1 << SIC_BIT;
    // STOP Will Be cleared Automatically.
}



//kpm.c
// Function Definitions
extern float setpoint;
int cnt;
// 4 x 4 matrix is using here
const unsigned char  keys[4][4]=
{
	{'1','2','3','U'},
	{'4','5','6','D'},                       // LOOK UP TABLE
	{'7','8','9','-'},
	{'O','0','.','E'}
};

//  4 x 4  KEYAD  INITILIAZATION.
void Init_Keypad(void)
{
	 // Setting the row pins as the output through the direction register.
	 WRITENIBBLE(IODIR1,ROW0,0x0F);
	 // No need to set for the columns bydefault the input pins are zero's only.
	 //WRITENIBBLE(IODIR0,COL0,0);
	 
}
// Column Scaning whether any key is pressed or not.
u32 Colscan(void)
{
 u32 status;
	status=READNIBBLE(IOPIN1,COL0);
	return status < 15 ? 0 : 1;
}
// Finding From Which Row the Key is Pressed.
u32 Rowcheck(void)
{
u32 row;
	for(row=0;row<=3;row++)
	{
		 WRITENIBBLE(IOPIN1,ROW0,~(1<<row)); // masking the bit one by one 
		 /* r=0 1110
		    r=1 1101
		    r=2 1011
		    r=3 0111
		*/
		 // now check if the key is pressed in that particular row or not
		if(Colscan()==0)
		{
			 break;  // after pressing the key we will check the Rowcheck before we not checking here
		}
	}
		
		WRITENIBBLE(IOPIN1,ROW0,0); // RESETIING ALL THE 4 BITS AS ZERO For the next presses Detection
		// need to return the value of the row on which it was pressed
		return row;

}


// After That From With Respect to ROW Which Column Key is Pressed.
u32 Columncheck(void)
{
u32  col;
	 for(col=0;col<=3;col++)
	 {
		  if((READBIT(IOPIN1,COL0 + col))== 0) // if the certain bit or column is zero means
				    break;                       // that key is pressed so returning the col value
			
	 }
	 return col;
 }

// With the Help of the Above Three Functions we will Get the ROW and the Column Value.
// From the Keypad Look up Table(A 2D Array) we are Returning the Some Key Value.
unsigned char KeyScan(void)
{
u32 row,col;
unsigned char KeyVal;
	// check if the key is pressed or not
	while(Colscan()); // if it pressed means the PC go to below statement
	                  // else it will in infinte loop only because from "colscan" function
	row=Rowcheck();
	col=Columncheck();
	//KeyVal=kpm_values[row][col];
	KeyVal=keys[row][col];
	return KeyVal;
}


void TakeSesnsorData(char *Data)
{
	        char flag=0,c=2;
          int cnt,i;
					char decimal;
					unsigned char KeyVal;
	        int k;
	  
AGAIN: 	CmdLCD(CLEAR_LCD);
				Setcursor(1,0);
				StrLCD("SET POINT <= 150");
				Setcursor(2,0);
				CmdLCD(DISP_ON_CUR_ON);
				cnt=0;
				i=0,decimal=0,flag=0;
	      
				
	while(1)
	{
		  KeyVal = KeyScan();
		  delay_Ms(250);
		 
		  
		  if((KeyVal >= '0' && KeyVal <= '9') || (KeyVal == '.'))
			{
				cnt++;
				if(KeyVal == 46)
				{
					decimal++;
					if(decimal >1)
						 goto AGAIN;
				}
				if(i==0 && KeyVal == 46)
					 goto AGAIN;
				if((KeyVal!=46)&&(KeyVal!='E'))
					 flag++;
				if((KeyVal == 46) && (cnt==4))
				{
					
					CmdLCD(CLEAR_LCD);
					Setcursor(1,0);
					//CharLCD(0x7E);
					StrLCD(" WRONG SETPOINT");
					Setcursor(2,0);
					StrLCD("ENTER VALID ONE");
					delay_Ms(500);
					goto AGAIN;
				}
				if(i<=4)
				{
				Setcursor(2,cnt-1);
				CharLCD(KeyVal);
				Data[i++] = KeyVal;
				}
				else
					continue;
			}
			else if((KeyVal == 'E') && (cnt>=1))
			{
				  if(flag>=5)
					{

				    CmdLCD(CLEAR_LCD);
					  Setcursor(1,0);
					  //CharLCD(0x7E);
					  StrLCD(" WRONG SETPOINT");
					  Setcursor(2,0);
					  StrLCD("ENTER VALID ONE");
					  delay_Ms(200);
						flag=0;
					  goto AGAIN;


					}
					for(k=0;k<i;k++)
						{
							  if(Data[k]=='.')
								{
									 c=0;
									 break;
								}
								c=1;
						}
						if(c)
						{
							 if(Data[0] > '1' && i==3)
							 {
								   CmdLCD(CLEAR_LCD);
									 Setcursor(1,0);
									 //CharLCD(0x7E);
									 StrLCD(" WRONG SETPOINT");
								   Setcursor(2,0);
								   StrLCD("ENTER VALID ONE");
								   delay_Ms(200);
					
								   goto AGAIN;
							 }
							
							  Data[i++]='.';
						}
					while(i<5)
					{
						 Data[i++]=48;
					}
				  
					Data[i] = '\0';
					setpoint = myatof(Data);//+0.00001f;
				if(setpoint > 150)
					{
									 CmdLCD(CLEAR_LCD);
									 Setcursor(1,0);
									 //CharLCD(0x7E);
									 StrLCD(" OVERFLOW VALUE");
								   Setcursor(2,0);
								   StrLCD("ENTER VALID ONE");
								   delay_Ms(200);
					
								   goto AGAIN;
					}
					break;
			}
		
	}
	        		
}


//lcd.c
// Function definitions
//>>> pin number Descrption
#define LCD_DATA  5
#define LCD_RS    19 // REGISTER SELECT PIN EITHER COMMAND -'0' OR "DATA - '1'.
#define LCD_RW    18    // READ - '1' and WRITE -'0'.
#define LCD_EN    20// FOR HIGH TO LOW PULSE GERNERATION WAITING PERIOD CAN WE SAY.

extern float temperature;
// Intialization function
// CGRAM CREATION.

unsigned char  CGRAM[64]=
{
	    0x1F,0x11,0x11,0x17,0x17,0x17,0x1F,0x1F,
	    0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x1F,
	    0x1F,0x10,0x10,0x10,0x10,0x10,0x10,0x10,
	    0x1F,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
	    0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x1F,
	    0x1F,0x11,0x1B,0x1B,0x1B,0x1B,0x1F,0x1F,
	    0x1F,0x11,0x17,0x11,0x17,0x11,0x1F,0x1F,
      0x1F,0X11,0X11,0X15,0x15,0x15,0x1F,0x1F
	    
};

void Display_temperature_on_LCD(void)
{
	 Setcursor(2,0);
     CharLCD(1);
     Setcursor(2,2);	
	 CharLCD(5);
	 CharLCD(6);
	 CharLCD(7);
	 CharLCD(0);
	 CharLCD(0x3A);
   
	 F32LCD(temperature,1);
	 CharLCD(0xDF);
	 CharLCD('c');
	 Setcursor(1,15);
	 CharLCD(3);
	 Setcursor(2,15);
	 CharLCD(4);
	
}

void Init_LCD(void)
{
	// DIRECTION SETTING
	WRITEBYTE(IODIR0,LCD_DATA,0xFF);
	SETBIT(IODIR0,LCD_RS);           // REGISTER SELECT PIN
	SETBIT(IODIR0,LCD_RW);           // READ/WRITE PIN
	SETBIT(IODIR0,LCD_EN);           // ENABLE PIN
	
	// 8 Bit MODE Parallel Connection Model
	delay_Ms(15); // for 8bit mode we need to give its the data sheet tell to us
	CmdLCD(0x30); // function set for 8bit mode
	delay_Ms(4); // ITS AN RULE WE NEED TO FOLLOW THAT'S IT
	CmdLCD(0x30); // same
	delay_Us(100);
	CmdLCD(0x30);
	CmdLCD(MODE_8BIT_2LINE); // USING THE 2 LINES IN THE 16X2 DISPLAY
	CmdLCD(DISP_ON_CUR_BLINK);
	CmdLCD(CLEAR_LCD); // Intially clear the total LCD
	CmdLCD(SHIFT_CUR_RIGHT); // moving the cursor position to right side
	CmdLCD(DISP_ON_CUR_OFF);
}
// sending the command to write in the LCD
void CmdLCD(u8 CmdByte)
{
	 // writing the command 
	 // we need to select the RS Pins as "LOW" RS-0
	 // SO WE NEED TO CLEAR OR SEND THE 0 TO THE RS PIN
	 IOCLR0=1<<LCD_RS;
	// NOW WE NEED TO WRITE INTO THE LCD WHATHERVER WE WANT WRITE EITHER COMMAND OR DATA
	WriteLCD(CmdByte);
}
// Writing the Data or Command to the LCD Driver or to the Module 
void WriteLCD(u8 byte)
{
	// while writing into the LCD we need to Clear the "pin RW-0" then it takes the data from us
	IOCLR0=1<<LCD_RW;
  // using the writebyte macro i am writing into it
	WRITEBYTE(IOPIN0,LCD_DATA,byte);
	// aftering writing into it we need to enable "EN" pin as high to low pulse 
	// minimum "450ns" we need to wait for next data writing 
	// so we are performimg some high to low pulse as "1 micro second which is more thsn enough"
	IOSET0=1<<LCD_EN;
	delay_Us(1); // for high to low pulse generation
	IOCLR0=1<<LCD_EN; 
	// after wait some to write it on the LCD
	delay_Ms(2); // wait some milli second here "2ms" I hava Taken
}
// character Displaying on the LCD 
void CharLCD(u8 asciival)
{
	// writing into the LCD 
	 // So RS pin set because we are writing the data into it 
	// RS-1 COMMAND REGISTER
	// RS-0 DATA REGISTER
	IOSET0=1<<LCD_RS;
	WriteLCD(asciival);
}
// String Displaying on the LCD
void StrLCD(s8*p)
{
	while(*p)
	{
		CharLCD(*p);
		p++;
	}
}
// NOW FUNCTION FOR DISPLAYING THE INTEGER VALUE
// U32LCD
// FIRST WE NEED TO KNEW THE RANGE OF unsigned int is 0 to 4,294,967,295-->>which is 10 Digit number
// so array of 10 characters in the ascii format is sufficient 
void U32LCD(u32 num)
{
	u8 lnum[11]; // 10 digits so 10 characters size is enough
	s32 i=0;
	if(num==0)
	{
		CharLCD('0');
		return;
	}
	while(num)
	{
		
		lnum[i++]=(num%10)+'0'; // converting it into the ascii value integer to ascii conversion
		
		num=num/10;
	}
	lnum[i]='\0';
	for(--i;i>=0;i--)
	{
		CharLCD(lnum[i]);
	}
}
// FOR DISPLAYING THE CUSTOM CHARACTERS
void BuildCGRAM(u8*p,u8 nBytes)
{
	u32 i;
	// Send Command to start the CGRAM 
	CmdLCD(GOTO_CGRAM_START);
	// Select The DATA REGISTER
	IOSET0=1<<LCD_RS;
	for(i=0;i<nBytes;i++)
	{
		WriteLCD(p[i]); //writing to the CGRAM own symbol character by character
		delay_Us(50);
	} // USING THE INDDEX TRAVERSAL WE HAVE DONE IT 
	  //>>>INPUT ARRAY BASE ADDRESS COLLECTED THROUGH THE character pointer and then index wise displaying one by one character
	  // Again Point Back to DDRAM start Display
	  //CmdLCD(GOTO_LINE1_POS0);
}

void Setcursor(char mode,char pos)
{
	if(mode==1)
	{
		CmdLCD(GOTO_LINE1_POS0+pos);
	}
	else if(mode==2)
	{
		CmdLCD(GOTO_LINE2_POS0+pos);
	}
}

void F32LCD(f32 fnum,u32 nDP)
{
	u32 num,i;
	if(fnum<0.0)
	{
		CharLCD('-');
		fnum=-fnum;
	}
	num=fnum;
	U32LCD(num);
	CharLCD('.');
	for(i=0;i<nDP;i++)
	{
		 fnum=(fnum-num)*10;
		 num=fnum;
		 CharLCD(num+48);
	}
}


//lm35.c
void Read_Temperature(s8 TempType ,f32 *temp)
{
        u32 adcDVal;
        f32 adcAVal;
        f32 degC,degF;

        Read_ADC(1,&adcDVal,&adcAVal);

        degC = adcAVal*100;
        degF = (degC *1.8) + 32;

        if(TempType == 'c')
        {
             *temp = degC;
				}
        else if(TempType == 'F')
        {
            *temp = degF;
        }
}

//pin_connect_block.c
void CfgPortPinFunc(u32 portNo,u32 pinNo,u32 pinFunc)
{
	if(portNo==0)
	{
		if(pinNo<=15)
		{
			PINSEL0=((PINSEL0 &~(3<<((pinNo)*2)))|(((pinFunc)<<(pinNo*2))));
		}
		else if(pinNo>=16 && pinNo<=31)
		{
			PINSEL1=((PINSEL1 &~(3<<((pinNo-16)*2)))|(((pinFunc)<<((pinNo-16)*2))));
		}
	}
		else if(portNo==1)
		{
			if(pinNo>=16 &&pinNo<=31)
			{
				PINSEL2=((PINSEL2 &~(3<<((pinNo-16)*2)))|(((pinFunc)<<((pinNo-16)*2))));
			}
		}
	}

	
//retrotech.c
	void RetroTech(void)
{
	  Setcursor(1,4);
	  StrLCD("RetroTech");
	  Setcursor(2,2);
	  StrLCD("Welcomes you.");
	  delay_Ms(300);
	  CmdLCD(CLEAR_LCD);
}

//rtc.c
// FUNCTION DEFINITIONS OF THE RTC PERIPHERAL 
// First Step---->  For Day of WEEK we need the Days Names ,So going to take an 2D-ARRAY.
// In LINITED LENGTH of String of 4 BYTES.

 char DayOfWeek[][4]={"SUN","MON","TUE","WED","THU","FRI","SAT"};

// Now Function Definitions 

//---------> Initilazing the RTC

void  Init_RTC(void)
{
	  // RESET THE RTC THROUGH CCR (CLOCK CONTROL REGISTER).
	  CCR = RTC_RESET;
	  // We are Doing this Project on Two Microcontroller's LPC2129 and the LP2148
	  // LPC2129 --> RTC runs ON-CHIP using PCLK (needs PREINT and PREFRAC Setup).
	  // LPC2148 --> RTC run uisng the External 32.768KHz Crystal (selected with CLKSRC).
	  // LPC2129 --> No Seprate VBAT Circuit.
	  // LPC2148 --> Has VBAT Circuit for Internal Registers Running Purpose even there is no power.
	
	  // so I am writing the program for to BOARD so using the Conditional Statements for this.
	  
	  #ifdef  LPC2148_MODE
	          // we need to select the CLOCK SOURCE and we need to ENABLE the RTC.
	          CCR = RTC_ENABLE | RTC_CLKSRC;
	  #else 
	          // For LPC2129 MODE or Controller.
	          PREINT  = PREINT_VAL;
	          PREFRAC = PREFRAC_VAL;
	          // Now need to Enable the RTC through CCR register.
	          CCR = RTC_ENABLE;
    #endif
}

void  GetRTCTimeInfo(s32 *Hour , s32 *Minute , s32 *Second)
{
	       // LOADING From SFR To the GLOBAL Variables.
	       (*Hour)   =  HOUR; 
	       (*Minute) =  MIN;
	       (*Second) =  SEC;
}

void  GetRTCDateInfo(s32 *Date , s32 *Month , s32 * Year)
{
	       // LOADING From SFR To the GLOBAL Variables.
	       (*Date)   =  DOM;
	       (*Month)  =  MONTH;
	       (*Year)   =  YEAR;
}

void  GetRTCDay(s32 *Day)
{
         // LOADING From SFR To the GLOBAL Variables.	
         (*Day)    =  DOW;
}

void  GetRTCHour(s32 *Hour)
{
	       // LOADING From SFR To the GLOBAL Variables.
	       (*Hour)   =  HOUR;
}

void  GetRTCMin(s32 *Minute)
{        
	       // LOADING From SFR To the GLOBAL Variables.
	       (*Minute) =  MIN;
}

void  GetRTCSec(s32 *Second)
{
	       // LOADING From SFR To the GLOBAL Variables.
	       (*Second) =  SEC;
}

void  GetRTCDate(s32 *Date)
{
	       // LOADING From SFR To the GLOBAL Variables.
	       (*Date)   =  DOM;
}

void  GetRTCMonth(s32 *Month)
{
	       // LOADING From SFR To the GLOBAL Variables.
	       (*Month)  =  MONTH;
}

void  GetRTCYear(s32 *Year)
{
	       // LOADING From SFR To the GLOBAL Variables.
	       (*Year)   =  YEAR;
}

void  SetRTCTimeInfo(u32 Hour , u32 Minute ,u32 Second)
{
	      // Assigning Directing the value to the SFR'S.
	       HOUR      =  Hour;
	       MIN       =  Minute;
	       SEC       =  Second;
}

void  SetRTCDateInfo(u32 Date , u32 Month , u32 Year)
{
	       // Assigning Directing the value to the SFR'S.
	       DOM       =  Date;
	       MONTH     =  Month;
	       YEAR      =  Year;
}

void  SetRTCDay(u32 Day)
{
	       // Assigning Directing the value to the SFR'S.
	       DOW       =  Day;
}

void  SetRTCHour(u32 Hour)
{
	       // Assigning Directing the value to the SFR'S.
	       HOUR      =  Hour;
}

void  SetRTCMin(u32 Minute)
{
	       // Assigning Directing the value to the SFR'S.
	       MIN       =  Minute;
}

void  SetRTCSec(u32 Second)
{
	       // Assigning Directing the value to the SFR'S.
	       SEC       =  Second;
}

void  SetRTCDate(u32 Date)
{
	       // Assigning Directing the value to the SFR'S.
	       DOM       =  Date;
}

void  SetRTCMonth(u32 Month)
{
	       // Assigning Directing the value to the SFR'S.
	       MONTH     =  Month;
}

void  SetRTCYear(u32 Year)
{
	       // Assigning Directing the value to the SFR'S.
	       YEAR      =  Year;
}

void  DisplayRTCTime(u32 Hour , u32 Minute , u32 Second)
{
    // From the GLOBAL Variables I am going to Display through the LCD.
    // Yes, we can directly use the SFR'S for the Display Operation.
    // But while Displaying them on LCD, we are trying to display Character by character.
	  // Like  12:23:12 in 24 hours format means Doing the Digit Extraction Method.  	
	  // So if I perform Them on the SFR'S Means Every Time we are Trying For Accessing that Particular SFR.
	  // Which Increases Extra and UseLess Burden On the CPU or PROCCESSOR.
	  // So that's the main Reason of using GLOBAL Variables.
	  
	  /****** ON THIS FORMAT DISPLYING_____ HOUR:MIN:SEC_______******************************/
	 
	  Setcursor(1,0);
	  CharLCD(2);
	  Setcursor(1,4);          // ON 16 x 2 LCD (confusion on this line refer function  "setcursor()" defininton.
	  CharLCD((Hour/10)+48);   // +48 because of Hour is the Integer for LCD Displaying we need to give the ASCII charcter so that's the reason +48 (Integer to ASCII) conversion.
    CharLCD((Hour%10)+48);
	  CharLCD(':');
    CharLCD((Minute/10)+48);
    CharLCD((Minute%10)+48);
	  CharLCD(':');
    CharLCD((Second/10)+48);
    CharLCD((Second%10)+48);
}

void  DisplayRTCDate(u32 Date , u32 Month , u32 Year)
{
	  /****** ON THIS FORMAT DISPLYING_____ DATE/MONTH/YEAR_______******************************/
	  
	  Setcursor(2,0);          // ON 16 x 2 LCD (confusion on this line refer function  "setcursor()" defininton.
	  CharLCD((Date/10)+48);
	  CharLCD((Date%10)+48);
	  CharLCD('/');
	  CharLCD((Month/10)+48);
	  CharLCD((Month%10)+48);
	  CharLCD('/');
	  U32LCD(Year);            // Digit Extraction not preferable option.
}
void DisplayRTCDay(s8 dow)
{
	
	StrLCD(DayOfWeek[dow]);
}

//uart.c
// Uart.c Function defintions.
unsigned char i=0,ch,r_flag;
char buff[200]="hello",dummy;


void UART_ISR(void)__irq
{
	  if((U0IIR & 0x04))
		{
			 ch = U0RBR;
			 if(i<200)
			 {
				 buff[i++] = ch;
			 }
		}
		else
		{ 
			  dummy = U0IIR;
		}
		VICVectAddr = 0;
}


void  Init_UART(void)
{
	 // Initialization of the UART Pins or(configguring the Pins and the Loading the Baud Value).
	 
	 CfgPortPinFunc(0,0,PIN_FUNC2);
	 CfgPortPinFunc(0,1,PIN_FUNC2);
	 
	 U0LCR = 1<<DLAB_BIT | WORD_LENGTH;
	
	 U0DLL = DIVISOR;
	 U0DLM = DIVISOR>>8;
	
	 U0LCR &=~(1<<DLAB_BIT); 
	 
	 VICIntEnable |= 1 << 6;
	 VICVectAddr0 = (unsigned int)UART_ISR;
	 VICVectCntl0 = 0x20|6;
	 
	 U0IER = 1<<0;
	
}
// Now transmit 1 Byte.

void UART_TxChar(unsigned char TxByte)
{
	U0THR = TxByte;
	while(((U0LSR >> TEMT)&1)==0);
}

unsigned char UART_RxChar(void)
{
	while(((U0LSR >> RDR)&1)==0);
	return U0RBR;
}

void UART_TxString(char *str)
{
	while(*str)
	{
		UART_TxChar(*str);
		
		str++;
	}
}

void UART_U32(unsigned int num)
{
	int i=0;
	char str_num[10];
	
	while(num)
	{
		str_num[i++] = (num%10) + 48;
		num/=10;
	}
	str_num[i]='\0';
	
	for(--i;i>=0;i--)
	{
		UART_TxChar(str_num[i]);
	}
}

void UART_S32(int num)
{
	if(num<0)
	{
		UART_TxChar('-');
		num = -num;
	}
	UART_U32(num);
}

void UART_F32(float fnum,unsigned char Ndp)
{
	// Ndp ---> Upto the N Decimal Points.
	int n,i;
	if(fnum < 0.0)
	{
		UART_TxChar('-');
		fnum = -fnum;
	}
	n = fnum;
	UART_U32(n);
	UART_TxChar('.');
	for(i=0;i<Ndp;i++)
	{
		fnum = (fnum - n)*10;
		n=fnum;
		UART_TxChar(n+48);
	}
}

void  UART_RxString(char *str,unsigned int MaxLen)
{
	     int i=0;  // indexing purpose.
	     unsigned char ch;     // reading character by character.
	     while(1)
			 {
				 ch = UART_RxChar();
				 if(ch=='\n'||ch=='\r')
				 {
					 str[i]='\0';
					 break;
				 }
				 else if(i>=MaxLen-1)
				 {
					 str[i]='\0';
					 break;
				 }
				 else
				 {
					 str[i++]=ch;
				 }
			 }
}
