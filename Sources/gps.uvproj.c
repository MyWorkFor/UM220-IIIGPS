/*
--------------------------------------------------------------------
--------------------------------------------------------------------
* gps.c
* main.c
* get date from serial
* display the data
--------------------------------------------------------------------
--------------------------------------------------------------------
*/
#include <REG52.H>
#include <stdio.h>
#include <intrins.h>
#include <lcd1602.h>
#include <uart.h>
#include <delay.h>
#include "string.h"
#include <stdlib.h>


unsigned char  flag_rec=0;    //ReceiveDataFlag
unsigned char num_rec=0;      //count flag
                          
//char code TIME_AREA= 8;		//time zone,we need not it
unsigned char flag_data;	//date flag

//GPS date array
//only displaty cmd $GPGGA information
unsigned char JD[16];		//longitude
unsigned char JD_a;		//longitude direction
unsigned char WD[15];		//latitude
unsigned char WD_a;		//latitude direction
unsigned char date[6];		//date
unsigned char time[6];		//date
unsigned char time1[6];		//date
unsigned char speed[5]={'0','0','0','0','0'};		//speed
unsigned char high[6];		//height
unsigned char angle[5];		//azimuth
unsigned char use_sat[2];	//satellinte count
unsigned char total_sat[2];	//total satellinte
unsigned char lock;			//location state

//date handing variable
unsigned char seg_count;	//comma counter
unsigned char dot_count;	//decimal point counter
unsigned char byte_count;	//bit counter
unsigned char cmd_number;	//cmd mode
unsigned char mode;			//0£ºend up £¬1£ºcmd mode£¬2£ºdata mode
unsigned char buf_full;		//1£ºdate effective 0£ºdate fault
unsigned char cmd[5];		//store cmd mode

//serial disconnect timer
unsigned  long int tt=0;

//main
void main () 
{	
  int jd_second,wd_second;  //intermediate variable
	init_uart();	     //initial serial
  lcd_init() ;       //initial lcd1602
  delay(200);
  LCD_Write_String(0,0,"Please Waiting...");   //display "Please Waiting" when it is boot up
  delay(200);
  delay(200);
  delay(200);
  delay(200);
  delay(200);
  delay(200);
  delay(200);
  delay(200);
  delay(200);
  delay(200);                             //delayed for display
  write_com(0x01);			//clear screen
	while(1)
	{
    tt++;
    if(tt>10000)
      {
        tt=10000;
        write_com(0x01);
        LCD_Write_String(3,0,"No Data!");
        LCD_Write_String(3,1,"No Data!");
        delay(200);
        delay(200);
        delay(200);
        delay(200);
        delay(200);
       }
     if(flag_rec==1)             //get gps date
      {
        flag_rec=0;               //clear flag
        if (lock==1)              //get location
		  	{  
          //
          LCD_Write_String(0,0,"JinDu:");    //display "Jindu:"
          LCD_Write_String(6,0,JD);           //display date
          LCD_Write_String(9,0,".");           //decimal 
          LCD_Write_String(10,0,JD+3);           //minute

          jd_second=60*atof((char *)(JD+5));     //seconds
          LCD_Write_Char(13,0,jd_second/10+'0');  //chang number to char
          LCD_Write_Char(14,0,jd_second%10+'0');  
          LCD_Write_Char(15,0,' ');                //filling space

          delay(200);                         //protect flash
          LCD_Write_String(0,1,"Weidu:");     //display next row
          LCD_Write_String(6,1,WD);
          LCD_Write_String(8,1,"."); 
          LCD_Write_String(9,1,WD+2);           //decimal point
          wd_second=60*atof((char *)(WD+4));
          LCD_Write_Char(12,1,wd_second/10+'0');
          LCD_Write_Char(13,1,wd_second%10+'0');
          LCD_Write_String(14,1,"  ");
          delay(200);
       }
      }
     }
		}
//serial interruupt service function
void ser_int (void) interrupt 4
{
 	
 	unsigned char tmp;
	if(RI)
	{
    tt=0;
		RI=0;
		tmp=SBUF;            //receive date from buffer
		switch(tmp)   //if $GPGGA,$GNGSW,$GNRMC,get data then processing it
		{
      //date start with $
			case '$':
				cmd_number=0;		//clear cmd mode
				mode=1;				//choice cmd receive mode
				byte_count=0;		//clear bit counter 
				flag_data=1;     //set data flag
				flag_rec=1;		//set data receive flag
			break;

			case ',':         //Eg:$GNRMC,134645.000,A,2603.964436,N,11912.410232,E,0.000,15.744,030718,,E,A*0B
				seg_count++;		//comma counter increase
				byte_count=0;
				break;

			case '*':
				switch(cmd_number)
				{
					case 1:
						buf_full|=0x01;   //00000001
						break;
					case 2:
						buf_full|=0x02;  //00000010
						break;
					case 3:
						buf_full|=0x04;  //00000100
						break;
				}

				mode=0;         //clear mode
				break;
			default:

// receive date cmd
				if(mode==1)	
				{
					cmd[byte_count]=tmp;			//get date and store buffer
					if(byte_count>=4)          //overlook cmd which less 4 bit
					{			
						if(cmd[0]=='G')           //first char 
						{
							if(cmd[1]=='N')
							{
								if(cmd[2]=='G')
								{
									if(cmd[3]=='G')
									{
										if(cmd[4]=='A')//judge $GNGGA
										{
											cmd_number=1;      //data type
											mode=2;            //date receive
											seg_count=0;       //comma counter clear
											byte_count=0;      //bit counter clear
										}
									}
									else if(cmd[3]=='S')       //cmd $GNGSV
									{
										if(cmd[4]=='V')
										{
											cmd_number=2;
											mode=2;                //get data
											seg_count=0;
											byte_count=0;
										}
									}
								}
								else if(cmd[2]=='R')   //cmd $GNRMC
								{
									if(cmd[3]=='M')
									{
										if(cmd[4]=='C')
										{
											cmd_number=3;
											mode=2;         //store data
											seg_count=0;
											byte_count=0;
										}
									}
								}
							}
						}
					}
				}
//date processing
				else if(mode==2)
				{
					
					switch (cmd_number)  //if receive data
					{
						case 1:				//get and store data,$GPGGA,[],[],[],[],[],[],[],[],[].....
							switch(seg_count)   //comma counter
							{
								case 2:		//latitude after 2rd comma
									if(byte_count<9)
									{
										WD[byte_count]=tmp;   //get latitude
									}
									break;
								case 3:		//latitude direction
									if(byte_count<1)
									{
										WD_a=tmp;
									}
									break;
								case 4:		//longtitude
									if(byte_count<10)
									{
										JD[byte_count]=tmp; //store it
									}
									break;
								case 5:		//longtitude direction
									if(byte_count<1)
									{
										JD_a=tmp;
									}
									break;
								case 6:		//location
									if(byte_count<1)
									{
										lock=tmp;
									}
									break;
								case 7:		//satellite number
									if(byte_count<2)
									{
										use_sat[byte_count]=tmp;
									}
									break;
								case 9:		//height
									if(byte_count<6)
									{
										high[byte_count]=tmp;
									}
									break;
							}
							break;

//
						case 2:				//cmd $GPGSV
							switch(seg_count)
							{
								case 3:		//satellite total
									if(byte_count<2)
									{
										total_sat[byte_count]=tmp;
									}
									break;
							}
							break;


//cmd mode 3:no sue
						case 3:				//$GPRMC
							switch(seg_count)
							{
								case 1:		//time
									if(byte_count<6)
									{				
										time[byte_count]=tmp;	
									}
									break;
								case 2:		//location				
									if(byte_count<1)
									{
									  if (tmp=='V') {lock=0;}
									  else
									  {
									    lock=1;
									   }
									}
									break;
								case 3:		//lititude			
//									if(byte_count<9)
//									{
//										WD[byte_count]=tmp;//we just need get once
//									}
									break;
								case 4:		//					
									if(byte_count<1)
									{
										WD_a=tmp;
									}
									break;
								case 5:		//longtitude				
//									if(byte_count<10)
//									{
//										JD[byte_count]=tmp;  //do not get again
//									}
									break;
								case 6:		//lititude direction		
									if(byte_count<1)
									{
										JD_a=tmp;
									}
									break;
								case 7:		//speed processing			
									if(byte_count<5)
									{
										speed[byte_count]=tmp;
									}
									break;
								case 8:		//direction angle				
									if(byte_count<5)
									{
										angle[byte_count]=tmp;
									}
									break;
								case 9:		//other			
									if(byte_count<6)
									{
										date[byte_count]=tmp;
									}
									break;

							}
							break;
					}
				}
				byte_count++;		//bit counter ++
				break;
		}
	}
}




