/*
--------------------------------------------------------------------
--------------------------------------------------------------------
* uart.c
* setup serial 
* use 11.0592MHz
* Bound Rate:9600
* data format:8bit,1 stop bit,0 check bit
--------------------------------------------------------------------
--------------------------------------------------------------------
*/

#include "reg52.h"
#include "uart.h"
//initial serial
void init_uart(void)	
{
	SCON = 0x50;      //serial mode use 1	       	   
	TMOD|= 0x20;      //timer work for mode 2                 
	PCON|= 0x00;      //SMOD=0,bound rate no boubling                                                  
	TH1  = 0xFD;		    //load timer primary number	
  TL1  = 0xFD;          
	TR1  = 1;         //enable timer1 interrupt                                                         
	ES   = 1;         //enable serial interrupt                
	EA   = 1;         //enale total interrupt 
} 

